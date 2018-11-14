uniform sampler2D texSampler;
uniform bool useTexture;
uniform int numLights;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 N;
varying vec4 fpos;

uniform vec3 ambientLightColor;
uniform vec4[8] lightVectors;
uniform vec3[8] lightColors;
uniform float[8] lightAttenuations;

void main() {
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	if (numLights > 0) {
		diffuse = ambientLightColor;
	}
	
	vec3 N2 = vec3(N.x, N.y, N.z);
	N2 = normalize(N2);

	for (int i = 0; i < numLights; ++i) {
		vec3 L2 = vec3(lightVectors[i].x, lightVectors[i].y, lightVectors[i].z);
		float att = 1.0f;
		if(lightVectors[i].w == 1.0f) {
			L2 = L2 - vec3(fpos.x, fpos.y, fpos.z);
			att = 1.0f /(1 + lightAttenuations[i] * length(L2));
		}
		L2 = normalize(L2);
		vec3 H = vec3(fpos.x, fpos.y, fpos.z);
		H = normalize(H);
		H = L2 - H;
		H = normalize(H);
		float NdotL = max(0.0f, dot(N2, L2));
		float NdotH = max(0.0f, dot(N2, H));
		diffuse += NdotL * lightColors[i] * att;;
		specular += pow(NdotH, 255) * lightColors[i] * att;;
	}

	if (useTexture) {
		gl_FragColor = texture2D(texSampler, ftex) * vec4(fcolor, 1);
	}
	else {
		
		gl_FragColor = vec4(diffuse * fcolor + specular, 1);
	}
}