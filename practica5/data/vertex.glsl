uniform mat4 MVP;
attribute vec3 vpos;
attribute vec3 vcolor;
attribute vec2 vtex;
varying vec3 fcolor;
varying vec2 ftex;

void main() {
	gl_Position =  MVP * vec4(vpos, 1);
	fcolor = vcolor;
	ftex = vtex;
}