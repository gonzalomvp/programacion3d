uniform sampler2D texSampler;
uniform sampler2D normalTexSampler;
uniform samplerCube cubeSampler;
uniform bool useTexture;
uniform vec4 color;
uniform int shininess;
uniform int numLights;
uniform vec3 ambient;
uniform vec4[8] lightVectors;
uniform vec3[8] lightColors;
uniform float[8] lightAttenuations;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fpos2;
varying vec4 fN;
varying mat3 TBN;
varying vec3 uvw; 

void main() {
	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	if (numLights > 0) {
		diffuse = ambient;
	}
	
	vec3 N = vec3(fN.x, fN.y, fN.z);
	vec4 v4NormalTex = texture2D(normalTexSampler, ftex);
	vec3 v3NormalTex = vec3(v4NormalTex.x, v4NormalTex.y, v4NormalTex.z);
	v3NormalTex = (v3NormalTex - vec3(0.5f)) * 2.0f;

	N = TBN * v3NormalTex;
	//N = normalize(N);

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

		if (dot(N, L) > 0.0f && shininess > 0) {
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
		color = textureCube(cubeSampler, uvw);
	}
	else {
		vec3 H = vec3(fpos2.x, fpos2.y, fpos2.z);
		H = normalize(H);
		//color = textureCube(cubeSampler, H);
		//color = textureCube(cubeSampler, uvw);
	}
	if (numLights > 0 && true) {
		//specular = vec3(0.0f);
		gl_FragColor = vec4(diffuse, 1) * color + vec4(specular, 1);
	}
	else {
		gl_FragColor = color;
	}
	
	
}