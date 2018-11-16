uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat4 Normal;
attribute vec3 vpos;
attribute vec2 vtex;
attribute vec3 vnormal;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fN;

void main() {
	gl_Position =  MVP * vec4(vpos, 1);
	fpos = ModelView * vec4(vpos, 1);
	fN = Normal * vec4(vnormal, 0);
	ftex = vtex;
}