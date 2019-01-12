uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 ModelView;
uniform mat4 Normal;
uniform vec3 eyePos;
attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;
attribute vec3 vtangent;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fpos2;
varying vec4 fN;
varying mat3 TBN;
varying vec3 uvw;
uniform vec4 color;
uniform int shininess;
uniform int numLights;
uniform vec3 ambient;
uniform vec4[8] lightVectors;
uniform vec3[8] lightColors;
uniform float[8] lightAttenuations;
varying vec3 light;

void main() {
	gl_Position =  MVP * vec4(vpos, 1);
	fpos = ModelView * vec4(vpos, 1);
	fpos2 = vec4(vpos, 1);
	fN = Normal * vec4(vnormal, 0);
	ftex = vtex;

	vec3 v3Normal = vec3(fN.x, fN.y, fN.z);
	vec4 v4Tangent = Normal * vec4(vtangent, 0);
	vec3 v3Tangent = vec3(v4Tangent.x, v4Tangent.y, v4Tangent.z);
	vec3 v3bitangent = cross (v3Normal, v3Tangent);
	TBN = mat3(v3Tangent, v3bitangent, v3Normal);

	vec3 eye = normalize(vec3(Model * vec4(vpos, 1)) - eyePos);
	vec3 normal = vec3(Model * vec4(vnormal, 0));
	uvw = normalize(reflect(eye, normal));

	//eye = normalize(vec3(Model * vec4(vpos, 1)) - eyePos);
	//normal = vec3(Model * vec4(vnormal, 0));
	uvw = normalize(refract(eye, normal, 0.98f));

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	if (numLights > 0) {
		diffuse = ambient;
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

		if (dot(N, L) > 0.0f && shininess > 0) {
			vec3 H = vec3(fpos.x, fpos.y, fpos.z);
			H = normalize(H);
			H = L - H;
			H = normalize(H);
			float NdotH = max(0.0f, dot(N, H));
			specular += pow(NdotH, shininess) * lightColors[i] * att;
		}
	}
	light = diffuse * vec3(color) + specular;
}