uniform sampler2D texSampler;
uniform bool useTexture;
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

void main() {
	vec3 diffuseLight = vec3(0.0f);
	vec3 specularLight = vec3(0.0f);

	vec3 N = normalize(fnormal);
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
	vec4 fragColor = materialColor;
	if(useTexture) {
		fragColor = texture2D(texSampler, ftex) * fragColor;
	}

	// Apply illumination only if there is at least one light in the scene
	if(numLights > 0) {
		fragColor = vec4((ambientLight + diffuseLight) * fragColor.rgb + specularLight, fragColor.a);
	}

	gl_FragColor = fragColor;
}