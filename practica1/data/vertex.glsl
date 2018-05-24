uniform mat4 MVP;
attribute vec2 vpos;
attribute vec3 vcolor;
varying vec3 fcolor;

void main() {
	gl_Position =  MVP * vec4(vpos, 0, 1);
	fcolor = vcolor;
}