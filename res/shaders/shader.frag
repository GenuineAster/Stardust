#version 330

in vec3 vNormal;
in float vHeight;

out vec4 fColor;

void main() {
	float height = (vHeight + 1.0) / 2.0;

	vec4 color = vec4(
		mod(height+0.3, 1.0),
		mod(pow(height+0.3, 2), 1.0),
		clamp(-vHeight, 0, 1),
		1.0
	);

	float light = dot(normalize(vNormal), normalize(vec3(0.0, 0.0, 8.0)));

	fColor = color * light;
}
