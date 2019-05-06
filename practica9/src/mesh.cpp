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
			TexturePtr normalTexture = nullptr;
			TexturePtr reflectionTexture = nullptr;
			TexturePtr refractionTexture = nullptr;
			float refractionCoef = 0.0f;
			std::vector<float> texcoords;
			std::vector<float> normals;
			std::vector<float> tangents;
			std::vector<float> vboneIndices;
			std::vector<float> vboneWeights;

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
				std::vector<std::string> textureFiles = splitString<std::string>(textureStr, ',');

				if (textureFiles.size() == 6) {
					texture = Texture::load(  (extractPath(filename) + textureFiles[0]).c_str()
											, (extractPath(filename) + textureFiles[1]).c_str()
											, (extractPath(filename) + textureFiles[2]).c_str()
											, (extractPath(filename) + textureFiles[3]).c_str()
											, (extractPath(filename) + textureFiles[4]).c_str()
											, (extractPath(filename) + textureFiles[5]).c_str());
				}
				else {
					std::string textureFile = extractPath(std::string(filename)) + textureStr;
					texture = Texture::load(textureFile.c_str());
				}

			}

			if (materialNode.child("normal_texture")) {
				std::string textureStr = materialNode.child("normal_texture").text().as_string();
				std::string textureFile = extractPath(std::string(filename)) + textureStr;
				normalTexture = Texture::load(textureFile.c_str());
			}

			if (materialNode.child("reflect_texture")) {
				std::string textureStr = materialNode.child("reflect_texture").text().as_string();
				std::vector<std::string> textureFiles = splitString<std::string>(textureStr, ',');

				if (textureFiles.size() == 6) {
					reflectionTexture = Texture::load((extractPath(filename) + textureFiles[0]).c_str()
						, (extractPath(filename) + textureFiles[1]).c_str()
						, (extractPath(filename) + textureFiles[2]).c_str()
						, (extractPath(filename) + textureFiles[3]).c_str()
						, (extractPath(filename) + textureFiles[4]).c_str()
						, (extractPath(filename) + textureFiles[5]).c_str());
				}
			}

			if (materialNode.child("refract_texture")) {
				std::string textureStr = materialNode.child("refract_texture").text().as_string();
				std::vector<std::string> textureFiles = splitString<std::string>(textureStr, ',');

				if (textureFiles.size() == 6) {
					refractionTexture = Texture::load((extractPath(filename) + textureFiles[0]).c_str()
						, (extractPath(filename) + textureFiles[1]).c_str()
						, (extractPath(filename) + textureFiles[2]).c_str()
						, (extractPath(filename) + textureFiles[3]).c_str()
						, (extractPath(filename) + textureFiles[4]).c_str()
						, (extractPath(filename) + textureFiles[5]).c_str());
				}

				if (materialNode.child("refract_coef")) {
					refractionCoef = materialNode.child("refract_coef").text().as_float();
				}
			}

			// Load indexes and coords
			std::string indexesStr = bufferNode.child("indices").text().as_string();
			std::vector<uint16_t> indexes = splitString<uint16_t>(indexesStr, ',');
			std::string coordsStr = bufferNode.child("coords").text().as_string();
			std::vector<float> coords = splitString<float>(coordsStr, ',');

			// Load texture coords if they exist
			if (bufferNode.child("texcoords")) {
				std::string texcoordsStr = bufferNode.child("texcoords").text().as_string();
				texcoords = splitString<float>(texcoordsStr, ',');
			}

			// Load normals if they exist
			if (bufferNode.child("normals")) {
				std::string normalsStr = bufferNode.child("normals").text().as_string();
				normals = splitString<float>(normalsStr, ',');
			}

			// Load tangents if they exist
			if (bufferNode.child("tangents")) {
				std::string tangentsStr = bufferNode.child("tangents").text().as_string();
				tangents = splitString<float>(tangentsStr, ',');
			}

			// Load bone indices if they exist
			if (bufferNode.child("bone_indices")) {
				std::string boneIndicesStr = bufferNode.child("bone_indices").text().as_string();
				vboneIndices = splitString<float>(boneIndicesStr, ',');
			}

			// Load bone weights if they exist
			if (bufferNode.child("bone_weights")) {
				std::string boneWeightsStr = bufferNode.child("bone_weights").text().as_string();
				vboneWeights = splitString<float>(boneWeightsStr, ',');
			}
			
			// Create vertices
			std::vector<Vertex> vertices;
			size_t verticesSize = coords.size() / 3;
			for (size_t i = 0; i < verticesSize; ++i) {
				glm::vec3 pos(glm::vec3(coords[i * 3], coords[i * 3 + 1], coords[i * 3 + 2]));
				glm::vec2 tex(0.0f);
				glm::vec3 normal(0.0f);
				glm::vec3 tangent(0.0f);
				glm::vec4 bonesIndices(-1.0f);
				glm::vec4 boneWeights(0.0f);

				if (texcoords.size() > (i * 2 + 1)) {
					tex = glm::vec2(texcoords[i * 2], texcoords[i * 2 + 1]);
				}

				if (normals.size() > (i * 3 + 2)) {
					normal = glm::vec3(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
				}

				if (tangents.size() > (i * 3 + 2)) {
					tangent = glm::vec3(glm::vec3(tangents[i * 3], tangents[i * 3 + 1], tangents[i * 3 + 2]));
				}

				if (vboneIndices.size() > (i * 4 + 2)) {
					bonesIndices = glm::vec4(vboneIndices[i * 4], vboneIndices[i * 4 + 1], vboneIndices[i * 4 + 2], vboneIndices[i * 4 + 3]);
				}

				if (vboneWeights.size() > (i * 4 + 2)) {
					boneWeights = glm::vec4(vboneWeights[i * 4], vboneWeights[i * 4 + 1], vboneWeights[i * 4 + 2], vboneWeights[i * 4 + 3]);
				}

				Vertex vertex(pos, tex, normal, tangent, bonesIndices, boneWeights);
				vertices.push_back(vertex);
			}
			MaterialPtr material = Material::create(texture, shader, color, shininess, blendMode, lighting, culling, depthWrite);
			material->setNormalTexture(normalTexture);
			material->setReflectionTexture(reflectionTexture);
			material->setRefractionTexture(refractionTexture);
			material->setRefractionCoef(refractionCoef);
			mesh->addBuffer(Buffer::create(vertices, indexes), material);
		}

		if (meshNode.child("last_frame")) {
			mesh->m_lastFrame = meshNode.child("last_frame").text().as_uint();
		}

		pugi::xml_node bonesNode = meshNode.child("bones");
		for (pugi::xml_node boneNode = bonesNode.child("bone"); boneNode; boneNode = boneNode.next_sibling("bone")) {
			std::string boneName = boneNode.child("name").text().as_string();
			std::string parent = "";
			std::vector<float> invPose;

			if (boneNode.child("parent")) {
				parent = boneNode.child("parent").text().as_string();
			}

			std::string invPoseStr = boneNode.child("inv_pose").text().as_string();
			invPose = splitString<float>(invPoseStr, ',');

			BonePtr bone = Bone::create(boneName.c_str(), mesh->getBoneIndex(parent.c_str()));
			bone->setInvPoseMatrix(glm::mat4(invPose[0],  invPose[1],  invPose[2],  invPose[3]
				                           , invPose[4],  invPose[5],  invPose[6],  invPose[7]
				                           , invPose[8],  invPose[9],  invPose[10], invPose[11]
				                           , invPose[12], invPose[13], invPose[14], invPose[15]));

			if (boneNode.child("positions")) {
				std::string positionsStr = boneNode.child("positions").text().as_string();
				std::vector<float> positions = splitString<float>(positionsStr, ',');
				size_t positionsSize = positions.size() / 4;
				for (size_t i = 0; i < positionsSize; ++i) {
					uint16_t frame = static_cast<uint16_t>(positions[i * 4]);
					glm::vec3 position = glm::vec3(glm::vec3(positions[i * 4 + 1], positions[i * 4 + 2], positions[i * 4 + 3]));
					bone->addPosition(frame, position);
				}
			}

			if (boneNode.child("rotations")) {
				std::string rotationsStr = boneNode.child("rotations").text().as_string();
				std::vector<float> rotations = splitString<float>(rotationsStr, ',');
				size_t rotationsSize = rotations.size() / 5;
				for (size_t i = 0; i < rotationsSize; ++i) {
					uint16_t frame = static_cast<uint16_t>(rotations[i * 5]);
					glm::quat rotation = glm::quat(rotations[i * 5 + 1], rotations[i * 5 + 2], rotations[i * 5 + 3], rotations[i * 5 + 4]);
					bone->addRotation(frame, rotation);
				}
			}

			if (boneNode.child("scales")) {
				std::string scalesStr = boneNode.child("scales").text().as_string();
				std::vector<float> scales = splitString<float>(scalesStr, ',');
				size_t scalesSize = scales.size() / 4;
				for (size_t i = 0; i < scalesSize; ++i) {
					uint16_t frame = static_cast<uint16_t>(scales[i * 4]);
					glm::vec3 scale = glm::vec3(glm::vec3(scales[i * 4 + 1], scales[i * 4 + 2], scales[i * 4 + 3]));
					bone->addScale(frame, scale);
				}
			}

			mesh->addBone(*bone);
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

int Mesh::getBoneIndex(const std::string& name) const {
	int index = -1;
	for (size_t i = 0; i < m_bones.size(); ++i) {
		if (m_bones[i].getName() == name) {
			index = static_cast<int>(i);
			break;
		}
	}
	return index;
}

void Mesh::draw() {
	for (size_t i = 0; i < m_buffers.size(); ++i) {
		BufferPtr buffer = m_buffers[i].first;
		MaterialPtr material = m_buffers[i].second;

		material->prepare();
		
		buffer->draw(*(material->getShader()));
	}
}