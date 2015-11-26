#version 330

in vec3 normal;

out vec4 fColor;


void main() {
	float light = (dot(normalize(normal), normalize(vec3(0.0, 0.0, 8.0))) + 1.0)/2.0;

	fColor = vec4(0.5, 0.5, 0.5, 1.0) * light;
}
