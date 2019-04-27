uniform mat4 SkyboxMVP;
attribute vec3 vpos;
varying vec3 uvw; 

void main() {
	uvw = vpos;
	gl_Position = SkyboxMVP * vec4(vpos, 1.0);
}