#include "mesh.h"

#include "../lib/pugixml/pugixml.hpp"
#include "state.h"
#include "utils.h"

MeshPtr Mesh::load(const char* filename, const ShaderPtr& shader) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if (result) {
		MeshPtr mesh = Mesh::create();

		pugi::xml_node meshNode = doc.child("mesh");
		pugi::xml_node buffersNode = meshNode.child("buffers");
		for (pugi::xml_node bufferNode = buffersNode.child("buffer"); bufferNode; bufferNode = bufferNode.next_sibling("buffer")) {
			pugi::xml_node materialNode = bufferNode.child("material");
			std::string textureStr = materialNode.child("texture").text().as_string();
			std::string textureFile = extractPath(std::string(filename)) + textureStr;
			std::string indexesStr = bufferNode.child("indices").text().as_string();
			std::vector<uint16_t> indexes = splitString<uint16_t>(indexesStr, ',');
			std::string coordsStr = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitString<float>(coordsStr, ',');
			std::string texcoordsStr = bufferNode.child("texcoords").text().as_string();
			std::vector<float> texcoords = splitString<float>(texcoordsStr, ',');
			std::vector<Vertex> vertices;
			size_t vericesSize = coords.size() / 3;
			for (size_t i = 0; i < vericesSize; ++i) {
				Vertex vertex(glm::vec3(coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]), glm::vec3(1), glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]));
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