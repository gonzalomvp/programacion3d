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
	void prepare();



private:
	Material(const TexturePtr& tex = nullptr, const ShaderPtr& shader = nullptr) : m_texture(tex), m_shader(shader) {}
	~Material() {}

	TexturePtr m_texture;
	ShaderPtr  m_shader;
};