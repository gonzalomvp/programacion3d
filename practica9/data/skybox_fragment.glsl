uniform samplerCube cubeTexSampler;

varying vec3 uvw;

void main() {
	gl_FragColor = texture(cubeTexSampler, normalize(uvw));
}