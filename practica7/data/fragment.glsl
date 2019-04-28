uniform sampler2D baseTexSampler;
uniform samplerCube cubeTexSampler;
uniform sampler2D normalsTexSampler;
uniform samplerCube reflectionTexSampler;
uniform samplerCube refractionTexSampler;
uniform bool useBaseTexture;
uniform bool isCubeTexture;
uniform bool useNormalsTexture;
uniform bool useReflectionTexture;
uniform bool useRefractionTexture;
uniform vec4 materialColor;
uniform int materialShininess;
uniform int numLights;
uniform vec3 ambientLight;
uniform vec4[8] lights;
uniform vec3[8] lightsColors;
uniform float[8] lightsAttenuations;

varying vec3 fpos;
varying vec3 fnormal;
varying vec2 ftex;
varying vec3 uvw;
varying vec3 uvwReflection;
varying vec3 uvwRefraction;
varying mat3 TBN;

void main() {
	vec3 N = vec3(0.0f);
	if(useNormalsTexture) {
		vec3 v3NormalTex = vec3(texture2D(normalsTexSampler, ftex));
		v3NormalTex = v3NormalTex * 2.0f - 1.0f;
		N = normalize(TBN * v3NormalTex);
	}
	else {
		N = normalize(fnormal);
	}
	vec3 Eye = normalize(-fpos);

	vec3 diffuseLight = vec3(0.0f);
	vec3 specularLight = vec3(0.0f);
	for (int i = 0; i < numLights; ++i) {
		vec3 L = lights[i].xyz;
		float att = 1.0f;

		// Calculate L and attenuation for Point lights
		if(lights[i].w == 1.0f) {
			L = lights[i].xyz - fpos;
			att = 1.0f /(1.0f + lightsAttenuations[i] * length(L));
		}
		L = normalize(L);
		float NdotL = max(0.0f, dot(N, L));
		
		// Calculate diffuse light
		diffuseLight += NdotL * lightsColors[i] * att;

		// Calculate specular light
		if (dot(N, L) > 0.0f && materialShininess > 0) {
			vec3 H = normalize(Eye + L);
			float NdotH = max(0.0f, dot(N, H));
			specularLight += pow(NdotH, materialShininess) * att;

			// Alternative specular light calculation using the light color instead of a white specular color
			//specularLight += pow(NdotH, materialShininess) * lightsColors[i] * att;
		}
	}

	// Combine material texture and material color
	vec4 fragColor = materialColor;
	if(useBaseTexture) {
		if(isCubeTexture) {
			fragColor = texture(cubeTexSampler, normalize(uvw)) * fragColor;
		}
		else {
			fragColor = texture(baseTexSampler, ftex) * fragColor;
		}
	}

	if(useReflectionTexture) {
		fragColor = texture(reflectionTexSampler, normalize(uvwReflection)) * fragColor;
	}

	if(useRefractionTexture) {
		fragColor = texture(refractionTexSampler, normalize(uvwRefraction)) * fragColor;
	}

	// Apply illumination only if there is at least one light in the scene
	if(numLights > 0) {
		//fragColor = vec4((ambientLight + diffuseLight) * fragColor.rgb + specularLight, fragColor.a);

		// Not using ambient and diffuse as in the given example
		fragColor = vec4(fragColor.rgb + specularLight, fragColor.a);
	}

	gl_FragColor = fragColor;
}