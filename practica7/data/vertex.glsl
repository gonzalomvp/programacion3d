uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat4 Normal;
attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;
attribute vec3 vtangent;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fpos2;
varying vec4 fN;
varying mat3 TBN;

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
}