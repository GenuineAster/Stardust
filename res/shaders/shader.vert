#version 330

layout(location=0) in vec3 position;

uniform mat4 grid;

void main() {
	gl_Position = grid * vec4(position, 1.0);
}
