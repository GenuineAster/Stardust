#version 330

in vec3 gNormal;
in float gHeight;
in vec3 gPosition;

uniform mat4 view;

out vec4 fColor;

void main() {
	float height = (gHeight + 1.0) / 2.0;

	vec4 color = vec4(
		mod(height+0.3, 1.0),
		mod(pow(height+0.3, 2), 1.0),
		clamp(-gHeight, 0, 1),
		1.0
	);

	if (gHeight == -2.0) color = vec4(0.4, 0.4, 0.4, 1.0);

	vec3 light_pos = vec3(0.0, 10.0, -10.0);
	vec3 to_surface = normalize(light_pos - gPosition);
	float brightness = abs(dot(to_surface, normalize(gNormal)));

	fColor = vec4(color.rgb * brightness, 1.0);
}
