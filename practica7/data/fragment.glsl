uniform sampler2D texSampler;
uniform sampler2D normalTexSampler;
uniform samplerCube cubeSampler;
uniform bool useTexture;
uniform mat4 ModelView;
uniform vec4 color;
uniform int shininess;
uniform int numLights;
uniform vec3 ambient;
uniform vec4[8] lightVectors;
uniform vec3[8] lightColors;
uniform float[8] lightAttenuations;
varying vec2 ftex;
varying vec4 fpos;
varying vec4 fpos2;
varying vec4 fN;
varying mat3 TBN;
varying vec3 uvw; 
varying vec3 light;

void main() {
	gl_FragColor = vec4(light, 1);	
}