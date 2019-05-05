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
			
			// Default Material values
			glm::vec4 color(1.0f);
			uint8_t shininess = 0;
			Material::BlendMode blendMode = Material::ALPHA;
			bool lighting = true;
			bool culling = true;
			bool depthWrite = true;
			TexturePtr texture = nullptr;
			std::vector<float> texcoords;
			std::vector<float> normals;

			// Load Material properties if they exist
			if (materialNode.child("color")) {
				std::string colorStr = materialNode.child("color").text().as_string();
				std::vector<float> colorValues = splitString<float>(colorStr, ',');
				color = glm::vec4(colorValues[0], colorValues[1], colorValues[2], colorValues[3]);
			}

			if (materialNode.child("shininess")) {
				shininess = materialNode.child("shininess").text().as_uint();
			}

			if (materialNode.child("blend")) {
				std::string blendModeStr = materialNode.child("blend").text().as_string();
				if (blendModeStr == "add") {
					blendMode = Material::ADD;
				}
				else if (blendModeStr == "mul") {
					blendMode = Material::MUL;
				}
			}

			if (materialNode.child("lighting")) {
				lighting = materialNode.child("lighting").text().as_bool();
			}

			if (materialNode.child("culling")) {
				culling = materialNode.child("culling").text().as_bool();
			}

			if (materialNode.child("depthwrite")) {
				depthWrite = materialNode.child("depthwrite").text().as_bool();
			}

			if (materialNode.child("texture")) {
				std::string textureStr = materialNode.child("texture").text().as_string();
				std::string textureFile = extractPath(std::string(filename)) + textureStr;
				texture = Texture::load(textureFile.c_str());
				std::string texcoordsStr = bufferNode.child("texcoords").text().as_string();
				texcoords = splitString<float>(texcoordsStr, ',');
			}
			
			// Load indexes and coords
			std::string indexesStr = bufferNode.child("indices").text().as_string();
			std::vector<uint16_t> indexes = splitString<uint16_t>(indexesStr, ',');
			std::string coordsStr = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitString<float>(coordsStr, ',');
			
			// Load normals if they exist
			if (bufferNode.child("normals")) {
				std::string normalsStr = bufferNode.child("normals").text().as_string();
				normals = splitString<float>(normalsStr, ',');
			}
			
			// Create vertices
			std::vector<Vertex> vertices;
			size_t verticesSize = coords.size() / 3;
			for (size_t i = 0; i < verticesSize; ++i) {
				glm::vec3 pos(glm::vec3(coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]));
				glm::vec2 tex(0.0f);
				glm::vec3 normal(0.0f);

				if (texcoords.size() > (i * 2 + 1)) {
					tex = glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]);
				}

				if (normals.size() > (i * 3 + 2)) {
					normal = glm::vec3(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
				}

				Vertex vertex(pos, tex, normal);
				vertices.push_back(vertex);
			}
			mesh->addBuffer(Buffer::create(vertices, indexes), Material::create(texture, shader, color, shininess, blendMode, lighting, culling, depthWrite));
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