uniform samplerCube texCubeSampler;

varying vec3 uvw;

void main() {
	gl_FragColor = texture(texCubeSampler, normalize(uvw));
}