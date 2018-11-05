uniform sampler2D texSampler;
uniform bool useTexture;
uniform int numLights;
uniform vec4[7] L;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 N;
varying vec4 fpos;

void main() {
	N = normalize(N);
	L[0] = normalize(L[0]);
	vec4 H = normalize(fpos);
	H = L[0] - H;
	H = normalize(H);
	float NdotL = max(0.0f, dot(N, L[0]));
	float NdotH = max(0.0f, dot(N, H));
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = NdotL * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specular = pow(NdotH, 255) * vec4(1.0f, 1.0f, 1.0f, 1.0f);

	L[1] = L[1] - fpos;
	float att = 1.0f /(1 + 0.2f * length(L[1]));
	L[1] = normalize(L[1]);
	H = normalize(fpos);
	H = L[1] - H;
	H = normalize(H);
	NdotL = max(0.0f, dot(N, L[1]));
	NdotH = max(0.0f, dot(N, H));
	diffuse += NdotL * vec4(1.0f, 0.0f, 0.0f, 1.0f) * att;
	specular += pow(NdotH, 255) * vec4(1.0f, 0.0f, 0.0f, 0.0f) * 1;

	if (useTexture) {
		gl_FragColor = texture2D(texSampler, ftex) * vec4(fcolor, 1);
	}
	else {
		
		gl_FragColor = (ambient + diffuse) * vec4(fcolor, 1) + specular;
	}
}