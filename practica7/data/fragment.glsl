uniform sampler2D texSampler;
uniform samplerCube texCubeSampler;
uniform sampler2D texNormalsSampler;
uniform bool useTexture;
uniform bool isCubeTexture;
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
varying mat3 TBN;

void main() {
	vec3 v3NormalTex = vec3(texture2D(texNormalsSampler, ftex));
	v3NormalTex = v3NormalTex * 2.0f - 1.0f;
	//v3NormalTex = vec3(1.f, 0.f, 1.f);
	vec3 N = normalize(TBN * v3NormalTex);


	vec3 diffuseLight = vec3(0.0f);
	vec3 specularLight = vec3(0.0f);

	//N = normalize(fnormal);
	vec3 Eye = normalize(-fpos);

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
	if(useTexture) {
		if(isCubeTexture) {
			materialColor = texture(texCubeSampler, normalize(uvw)) * materialColor;
		}
		else {
			materialColor = texture(texSampler, ftex) * materialColor;
		}
	}

	// Apply illumination only if there is at least one light in the scene
	if(numLights > 0) {
		//materialColor = vec4((ambientLight + diffuseLight) * materialColor.rgb + specularLight, materialColor.a);

		// Not using ambient and diffuse as in the given example
		materialColor = vec4(materialColor.rgb + specularLight, materialColor.a);
	}

	gl_FragColor = materialColor;
}