#include "shader.h"

char Shader::errorLog[1024];

ShaderPtr Shader::create(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	ShaderPtr shader(new Shader(vertexShaderSource, fragmentShaderSource), [](Shader* p) { delete p; });
	
	if (shader->m_Id == 0) {
		shader = nullptr;
	}
	return shader;
}

Shader::Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) : m_Id(0), m_vposLoc(-1), m_vcolorLoc(-1) {
	
	// create vertex shader
	int retCode;
	const char* cVertexShaderSource = vertexShaderSource.c_str();
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vertexShader, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(vertexShader);
		return;
	}

	// create fragment shader
	const char* cFragmentShaderSource = fragmentShaderSource.c_str();
	uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fragmentShader, sizeof(errorLog), nullptr, errorLog);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return;
	}

	// create and link program
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertexShader);
	glAttachShader(m_Id, fragmentShader);
	glLinkProgram(m_Id);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glGetProgramiv(m_Id, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(m_Id, sizeof(errorLog), nullptr, errorLog);
		glDeleteProgram(m_Id);
		m_Id = 0;
		return;
	}
	m_vposLoc   = glGetAttribLocation(m_Id, "vpos");
	m_vcolorLoc = glGetAttribLocation(m_Id, "vcolor");
	m_vtexLoc   = glGetAttribLocation(m_Id, "vtex");
}

Shader::~Shader() {
	if (m_Id != 0) {
		glDeleteProgram(m_Id);
	}
}

void Shader::use() const {
	glUseProgram(m_Id);
}

void Shader::setupAttribs() const {
	if (m_vposLoc != -1) {
		glEnableVertexAttribArray(m_vposLoc);
		glVertexAttribPointer(m_vposLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, pos)));
	}

	if (m_vcolorLoc != -1) {
		glEnableVertexAttribArray(m_vcolorLoc);
		glVertexAttribPointer(m_vcolorLoc, 3, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color)));
	}

	if (m_vtexLoc != -1) {
		glEnableVertexAttribArray(m_vtexLoc);
		glVertexAttribPointer(m_vtexLoc, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, tex)));
	}
}

int Shader::getLocation(const char* name) const {
	return glGetUniformLocation(m_Id, name);
}

void Shader::setInt(int loc, int val) {
	if (loc != -1) {
		glUniform1i(loc, val);
	}
}

void Shader::setFloat(int loc, float val) {
	if (loc != -1) {
		glUniform1f(loc, val);
	}
}

void Shader::setVec3(int loc, const glm::vec3& vec) {
	if (loc != -1) {
		glUniform3f(loc, vec.x, vec.y, vec.z);
	}
}

void Shader::setVec4(int loc, const glm::vec4& vec) {
	if (loc != -1) {
		glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
	}
}

void Shader::setMatrix(int loc, const glm::mat4& matrix) {
	if (loc != -1) {
		glUniformMatrix4fv(loc, 1, false, glm::value_ptr(matrix));
	}
}