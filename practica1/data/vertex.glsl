uniform mat4 MVP;
attribute vec3 vpos;
attribute vec3 vcolor;
varying vec3 fcolor;

void main() {
	gl_Position =  MVP * vec4(vpos, 1);
	fcolor = vcolor;
}