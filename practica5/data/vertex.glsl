uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat4 Normal;
attribute vec3 vpos;
attribute vec3 vcolor;
attribute vec2 vtex;
attribute vec3 vnormal;
varying vec3 fcolor;
varying vec2 ftex;
varying vec4 N;
varying vec4 fpos;

void main() {
	gl_Position =  MVP * vec4(vpos, 1);
	fcolor = vcolor;
	ftex = vtex;
	N = Normal * vec4(vnormal, 0);
	fpos = ModelView * vec4(vpos, 1);
}