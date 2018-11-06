#include "mesh.h"

#include "../lib/pugixml/pugixml.hpp"
#include "state.h"
#include "utils.h"

//Comprobar que vienen los datos!!!!!!!!!!!!!!!

MeshPtr Mesh::load(const char* filename, const ShaderPtr& shader) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result) {
		MeshPtr mesh = Mesh::create();
		glm::vec3 color(1);

		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");
		for (pugi::xml_node bufferNode = buffersNode.child("buffer"); bufferNode; bufferNode = bufferNode.next_sibling("buffer")) {
			pugi::xml_node materialNode = bufferNode.child("material");
			std::string colorStr = materialNode.child("color").text().as_string();
			if (colorStr != "") {
				std::vector<float> colorValues = splitString<float>(colorStr, ',');
				color = glm::vec3(colorValues[0], colorValues[1], colorValues[2]);
			}
			
			std::string textureStr = materialNode.child("texture").text().as_string();
			std::string textureFile = extractPath(std::string(filename)) + textureStr;
			std::string indexesStr = bufferNode.child("indices").text().as_string();
			std::vector<uint16_t> indexes = splitString<uint16_t>(indexesStr, ',');
			std::string coordsStr = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitString<float>(coordsStr, ',');
			std::string texcoordsStr = bufferNode.child("texcoords").text().as_string();
			std::vector<float> texcoords = splitString<float>(texcoordsStr, ',');
			std::string normalsStr = bufferNode.child("normals").text().as_string();
			std::vector<float> normals = splitString<float>(normalsStr, ',');
			
			std::vector<Vertex> vertices;
			size_t vericesSize = coords.size() / 3;
			for (size_t i = 0; i < vericesSize; ++i) {
				glm::vec3 pos(glm::vec3(coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]));
				glm::vec2 tex(0);
				glm::vec3 normal(0);
				if (normalsStr != "") {
					
					normal = glm::vec3(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
				}
				

				if (texcoordsStr != "") {
					tex = glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]);
				}

				Vertex vertex(pos, color, tex, normal);
				vertices.push_back(vertex);
			}
			mesh->addBuffer(Buffer::create(vertices, indexes), Material::create(Texture::load(textureFile.c_str()), nullptr));
		}
		return mesh;
	}
	else {
		std::cout << result.description() << std::endl;
		return nullptr;
	}
}

void Mesh::addBuffer(const BufferPtr& buffer, const ShaderPtr& shader) {
	addBuffer(buffer, Material::create(nullptr, shader));
}

void Mesh::addBuffer(const BufferPtr& buffer, const MaterialPtr& material) {
	m_buffers.push_back(std::make_pair(buffer, material));
}

void Mesh::draw() {
	for (size_t i = 0; i < m_buffers.size(); ++i) {
		BufferPtr buffer = m_buffers[i].first;
		MaterialPtr material = m_buffers[i].second;

		material->prepare();
		
		buffer->draw(*(material->getShader()));
	}
}