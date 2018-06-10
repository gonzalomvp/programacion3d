uniform sampler2D texSampler;
uniform bool useTexture;
varying vec3 fcolor;
varying vec2 ftex;

void main() {
	if (useTexture) {
		gl_FragColor = texture2D(texSampler, ftex);
	}
	else {
		gl_FragColor = vec4(fcolor, 1);
	}
}