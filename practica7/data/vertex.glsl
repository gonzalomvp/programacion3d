uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat4 Normal;

attribute vec3 vpos;
attribute vec3 vnormal;
attribute vec3 vtangent;
attribute vec2 vtex;

varying vec3 fpos;
varying vec3 fnormal;
varying vec2 ftex;
varying vec3 uvw;

varying mat3 TBN;

void main() {
	fpos = vec3(ModelView * vec4(vpos, 1.0f));
	fnormal = normalize(vec3(Normal * vec4(vnormal, 0.0f)));

	vec3 T = normalize(vec3(Normal * vec4(vtangent, 0.0f)));
	vec3 B = cross(fnormal, T);
	TBN = mat3(T, B, fnormal);

	ftex = vtex;
	uvw = vpos;
	gl_Position =  MVP * vec4(vpos, 1.0f);
}