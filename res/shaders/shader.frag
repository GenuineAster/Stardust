#version 330

in vec3 gPosition;
in vec3 gNormal;
in float gHeight;

uniform mat4 uView;
uniform int uWater;
uniform sampler1D uPlanetGradient;

out vec4 fColor;

void main() {
	vec3 light_pos = vec3(0.0, 10.0, -10.0);
	vec3 to_surface = normalize(light_pos - gPosition);
	float brightness = abs(dot(to_surface, gNormal));
	brightness = clamp(brightness, 0.0, 1.0);

	float height = gHeight/2.0+0.5;
	vec4 color = texture(uPlanetGradient, uWater == 1 ? 0.0 : height);
	color.rgb *= brightness;
	color.a = uWater == 1 ? color.a : 1.0;
	fColor = color;
}
