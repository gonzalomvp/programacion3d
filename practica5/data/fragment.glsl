uniform sampler2D texSampler;
uniform bool useTexture;
uniform int numLights;
uniform vec4[7] L;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 N;
varying vec4 fpos;

uniform vec4[9] lightVectors;
uniform vec4[9] lightColors;
uniform float[9] lightAttenuations;

void main() {
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = vec4(0.0f);
	vec4 specular = vec4(0.0f);
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
		
		gl_FragColor = (ambient + diffuse) * vec4(fcolor, 1) + specular;
	}
}