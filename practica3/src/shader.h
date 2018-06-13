#pragma once

#include "common.h"

class Shader;

typedef std::shared_ptr<Shader> ShaderPtr;

class Shader {
public:
	static ShaderPtr create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

	// Devuelve el identificador de OpenGL del programa
	uint32_t getId() const { return m_Id; }

	// Obtiene el mensaje de error generado al compilar o enlazar
	static const char* getError() { return errorLog; }

	// Activa el uso de este programa
	void use() const;

	// Activa la escritura de las variables attribute,
	// y especifica su formato
	void setupAttribs() const;

	// Obtiene la localización de una variable uniform
	int getLocation(const char* name) const;

	// Da valor a una variable uniform
	void setInt(int loc, int val);
	void setFloat(int loc, float val);
	void setVec3(int loc, const glm::vec3& vec);
	void setVec4(int loc, const glm::vec4& vec);
	void setMatrix(int loc, const glm::mat4& matrix);

private:
	static char errorLog[1024];

	Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
	~Shader();

	uint32_t m_Id;
	int      m_vposLoc;
	int      m_vcolorLoc;
	int      m_vtexLoc;
};