uniform sampler2D texSampler;
uniform bool useTexture;
uniform int numLights;
uniform vec4[7] L;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 N;
varying vec4 fpos;

void main() {
	vec3 N2 = vec3(N.x, N.y, N.z);
	N2 = normalize(N2);
	vec3 L2 = vec3(L[0].x, L[0].y, L[0].z);
	L2 = normalize(L2);
	vec3 H = vec3(fpos.x, fpos.y, fpos.z);
	H = normalize(H);
	H = L2 - H;
	H = normalize(H);
	float NdotL = max(0.0f, dot(N2, L2));
	float NdotH = max(0.0f, dot(N2, H));
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	vec4 diffuse = NdotL * vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec4 specular = pow(NdotH, 255) * vec4(1.0f, 1.0f, 1.0f, 1.0f);

	L2 = vec3(L[1].x, L[1].y, L[1].z) - vec3(fpos.x, fpos.y, fpos.z);
	float att = 1.0f /(1 + 0.2f * length(L2));
	L2 = normalize(L2);
	H = vec3(fpos.x, fpos.y, fpos.z);
	H = normalize(H);
	H = L2 - H;
	H = normalize(H);
	NdotL = max(0.0f, dot(N2, L2));
	NdotH = max(0.0f, dot(N2, H));
	diffuse += NdotL * vec4(1.0f, 0.0f, 0.0f, 1.0f) * att;
	specular += pow(NdotH, 255) * vec4(1.0f, 0.0f, 0.0f, 0.0f) * att;

	if (useTexture) {
		gl_FragColor = texture2D(texSampler, ftex) * vec4(fcolor, 1);
	}
	else {
		
		gl_FragColor = (ambient + diffuse) * vec4(fcolor, 1) + specular;
	}
}