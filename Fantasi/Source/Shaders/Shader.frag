#version 430

uniform sampler2D srcTex;
in vec2 texCoord;

out vec4 color;

void main() {
	color = texture(srcTex, texCoord);
}
