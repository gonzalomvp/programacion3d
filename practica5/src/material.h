#pragma once

#include "texture.h"
#include "shader.h"
#include "state.h"

class Material;

typedef std::shared_ptr<Material> MaterialPtr;

class Material {
public:
	static MaterialPtr create(const TexturePtr& tex = nullptr, const ShaderPtr& shader = nullptr) { return MaterialPtr(new Material(tex, shader), [](Material* p) { delete p; }); }
	const ShaderPtr&   getShader() const                 { return (m_shader) ? m_shader : State::defaultShader; }
	ShaderPtr&         getShader()                       { return (m_shader) ? m_shader : State::defaultShader; }
	void               setShader(ShaderPtr& shader)      { m_shader = shader;                                   }
	const TexturePtr&  getTexture() const                { return m_texture;                                    }
	void               setTexture(const TexturePtr& tex) { m_texture = tex;                                     }
	const glm::vec4&   getColor() const                  { return m_color;                                      }
	void			   setColor(const glm::vec4& color)  { m_color = color;                                     }
	uint8_t		       getShininess() const              { return m_shininess;                                  }
	void			   setShininess(uint8_t shininess)   { m_shininess = shininess;                             }

	void prepare();



private:
	Material(const TexturePtr& tex = nullptr, const ShaderPtr& shader = nullptr, glm::vec4 color = glm::vec4(1), uint8_t shininess = 1.0f)
		: m_texture(tex), m_shader(shader), m_color(color), m_shininess(shininess){}
	~Material() {}

	TexturePtr m_texture;
	ShaderPtr  m_shader;
	glm::vec4  m_color;
	uint8_t    m_shininess;
};