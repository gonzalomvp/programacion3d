#include "shader.h"

char Shader::errorLog[1024];

Shader* Shader::createShader(const std::string vertexShaderSource, const std::string fragmentShaderSource)
{
	std::shared_ptr<Shader> s(new Shader(vertexShaderSource, fragmentShaderSource), [](Shader* p) { delete p; });
	//comprobar si Id = 0
}

Shader::Shader(const std::string vertexShaderSource, const std::string fragmentShaderSource) : m_Id(0) {
	// create vertex shader
	int retCode;
	const char* cVertexShaderSource = vertexShaderSource.c_str();
	uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE)
	{
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
	if (retCode == GL_FALSE)
	{
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
	if (retCode == GL_FALSE)
	{
		glGetProgramInfoLog(m_Id, sizeof(errorLog), nullptr, errorLog);
		glDeleteProgram(m_Id);
	}
}

void Shader::use() const{
	glUseProgram(m_Id);
}