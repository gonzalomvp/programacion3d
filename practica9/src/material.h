#pragma once

#include "texture.h"
#include "shader.h"
#include "state.h"
#include "light.h"

class Material;

typedef std::shared_ptr<Material> MaterialPtr;

class Material {
public:
	enum BlendMode {
		ALPHA,
		ADD,
		MUL
	};

	static MaterialPtr create(const TexturePtr& tex = nullptr, const ShaderPtr& shader = nullptr, glm::vec4 color = glm::vec4(1.0f), uint8_t shininess = 0.0f, BlendMode blendMode = ALPHA, bool lighting = true, bool culling = true, bool depthWrite = true) { 
		return MaterialPtr(new Material(tex, shader, color, shininess, blendMode, lighting, culling, depthWrite), [](Material* p) { delete p; }); }

	const ShaderPtr&   getShader() const                           { return (m_shader) ? m_shader : State::defaultShader; }
	ShaderPtr&         getShader()                                 { return (m_shader) ? m_shader : State::defaultShader; }
	void               setShader(ShaderPtr& shader)                { m_shader = shader;                                   }
	const TexturePtr&  getTexture() const                          { return m_texture;                                    }
	void               setTexture(const TexturePtr& tex)           { m_texture = tex;                                     }
	const TexturePtr&  getNormalTexture() const                    { return m_normalTexture;                              }
	void               setNormalTexture(const TexturePtr& tex)     { m_normalTexture = tex;                               }
	const TexturePtr&  getReflectionTexture() const                { return m_reflectionTexture;                          }
	void               setReflectionTexture(const TexturePtr& tex) { m_reflectionTexture = tex;                           }
	const TexturePtr&  getRefractionTexture() const                { return m_refractionTexture;                          }
	void               setRefractionTexture(const TexturePtr& tex) { m_refractionTexture = tex;                           }
	float              getRefractionCoef() const                   { return m_refractionCoef;                             }
	void               setRefractionCoef(float coef)               { m_refractionCoef = coef;                             }
	const glm::vec4&   getColor() const                            { return m_color;                                      }
	void			   setColor(const glm::vec4& color)            { m_color = color;                                     }
	uint8_t		       getShininess() const                        { return m_shininess;                                  }
	void			   setShininess(uint8_t shininess)             { m_shininess = shininess;                             }
	BlendMode          getBlendMode() const                        { return m_blendMode;                                  }
	void		       setBlendMode(BlendMode blendMode)           { m_blendMode = blendMode;                             }
	bool		       getLighting() const                         { return m_lighting;                                   }
	void		       setLighting(bool enable)                    { m_lighting = enable;                                 }
	bool		       getCulling() const                          { return m_culling;                                    }
	void		       setCulling(bool enable)                     { m_culling = enable;                                  }
	bool		       getDepthWrite() const                       { return m_depthWrite;                                 }
	void		       setDepthWrite(bool enable)                  { m_depthWrite = enable;                               }

	void prepare();

private:
	Material(const TexturePtr& tex, const ShaderPtr& shader, glm::vec4 color, uint8_t shininess, BlendMode blendMode, bool lighting, bool culling, bool depthWrite)
		: m_texture(tex), m_shader(shader), m_color(color), m_shininess(shininess), m_blendMode(blendMode), m_lighting(lighting), m_culling(culling), m_depthWrite(depthWrite), m_normalTexture(nullptr), m_reflectionTexture(nullptr), m_refractionTexture(nullptr), m_refractionCoef(0.0f) {}
	~Material() {}

	TexturePtr m_texture;
	ShaderPtr  m_shader;
	glm::vec4  m_color;
	uint8_t    m_shininess;
	BlendMode  m_blendMode;
	bool       m_lighting;
	bool       m_culling;
	bool       m_depthWrite;
	TexturePtr m_normalTexture;
	TexturePtr m_reflectionTexture;
	TexturePtr m_refractionTexture;
	float      m_refractionCoef;
};