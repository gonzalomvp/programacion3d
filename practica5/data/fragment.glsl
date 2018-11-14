uniform sampler2D texSampler;
uniform bool useTexture;
uniform vec3 color;
uniform int shininess;
uniform int numLights;
uniform vec3 ambientLightColor;
uniform vec4[8] lightVectors;
uniform vec3[8] lightColors;
uniform float[8] lightAttenuations;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fN;

void main() {
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	if (numLights > 0) {
		diffuse = ambientLightColor;
	}
	
	vec3 N = vec3(fN.x, fN.y, fN.z);
	N = normalize(N);

	for (int i = 0; i < numLights; ++i) {
		vec3 L = vec3(lightVectors[i].x, lightVectors[i].y, lightVectors[i].z);
		float att = 1.0f;
		if(lightVectors[i].w == 1.0f) {
			L = L - vec3(fpos.x, fpos.y, fpos.z);
			att = 1.0f /(1 + lightAttenuations[i] * length(L));
		}
		L = normalize(L);
		float NdotL = max(0.0f, dot(N, L));
		diffuse += NdotL * lightColors[i] * att;

		if (dot(N, L) > 0.0f) {
			vec3 H = vec3(fpos.x, fpos.y, fpos.z);
			H = normalize(H);
			H = L - H;
			H = normalize(H);
			float NdotH = max(0.0f, dot(N, H));
			specular += pow(NdotH, shininess) * lightColors[i] * att;
		}
	}

	if (useTexture) {
		color = texture2D(texSampler, ftex) * color;
	}
	if (numLights > 0) {
		gl_FragColor = vec4(diffuse * color + specular, 1);
	}
	else {
		gl_FragColor = vec4(color, 1);
	}
	
	
}