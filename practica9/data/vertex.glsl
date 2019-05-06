uniform mat4 MVP;
uniform mat4 Model;
uniform mat4 ModelView;
uniform mat4 Normal;
uniform mat4 depthBiasMatrix;
uniform vec3 eyePos;
uniform bool useReflectionTexture;
uniform bool useRefractionTexture;
uniform float refractionCoef;
uniform bool useShadows;
uniform bool skinned;
uniform mat4 animMatrices[64];

attribute vec3 vpos;
attribute vec3 vnormal;
attribute vec3 vtangent;
attribute vec2 vtex;
attribute vec4 vboneIndices;
attribute vec4 vboneWeights;

varying vec3 fpos;
varying vec3 fnormal;
varying vec2 ftex;
varying vec3 uvw;
varying vec3 uvwReflection;
varying vec3 uvwRefraction;
varying mat3 TBN;
varying vec3 depthCoord;

void main() {
	fpos = vec3(ModelView * vec4(vpos, 1.0f));
	fnormal = normalize(vec3(Normal * vec4(vnormal, 0.0f)));

	vec3 T = normalize(vec3(Normal * vec4(vtangent, 0.0f)));
	vec3 B = cross(fnormal, T);
	TBN = mat3(T, B, fnormal);

	ftex = vtex;

	uvw = vpos;
	vec3 worldEye = normalize(vec3(Model * vec4(vpos, 1.0f)) - eyePos);
	vec3 worldNormal = normalize(vec3(Model * vec4(vnormal, 0.0f)));

	// Reflection
	if(useReflectionTexture) {
		uvwReflection = normalize(reflect(worldEye, worldNormal));
	}

	// Refraction
	if(useRefractionTexture) {
		uvwRefraction = normalize(refract(worldEye, worldNormal, refractionCoef));
	}
	
	if (useShadows) {
		depthCoord = vec3(depthBiasMatrix * vec4(vpos, 1.0f));
	}

	vec4 vpos4 = vec4(vpos, 1);
	if (skinned) {
		mat4 boneTransform = mat4(1);
		for (int i = 0; i < 4; ++i) {
			int index = int(vboneIndices[i]);
			if (index > -1) boneTransform += animMatrices[index] * vboneWeights[i];
		}
		vpos4 = boneTransform * vpos4;
	}

	gl_Position =  MVP * vpos4;
}