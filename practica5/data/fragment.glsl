uniform sampler2D texSampler;
uniform bool useTexture;
varying vec3 fcolor;
varying vec2 ftex;
varying vec3 diffuse;
varying vec3 specular;

void main() {
	if (useTexture) {
		gl_FragColor = texture2D(texSampler, ftex) * vec4(fcolor, 1);
	}
	else {
		gl_FragColor = vec4(fcolor, 1);
	}
}