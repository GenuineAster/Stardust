#version 330

layout(location=0) in vec3 position;
layout(location=1) in mat4 grid;


void main() {
	gl_Position = grid * vec4(position, 1.0);
}
