#version 330

in vec3 gPosition;
in vec3 gNormal;
in float gHeight;

uniform mat4 uView;
uniform int uWater;
uniform sampler1D uPlanetGradient;

out vec4 fColor;

const vec3 water = vec3(0.00, 0.00, 1.00);
const vec3 sand  = vec3(0.75, 0.75, 0.00);
const vec3 grass = vec3(0.20, 0.70, 0.20);
const vec3 rock  = vec3(0.40, 0.40, 0.10);
const vec3 snow  = vec3(0.80, 0.80, 0.80);

void main() {
	float height = gHeight/2.0+0.5;

	vec4 color;
	if (uWater == 1) color = texture(uPlanetGradient, 0.0);
	else color = texture(uPlanetGradient, height);

	vec3 light_pos = vec3(0.0, 10.0, -10.0);
	vec3 to_surface = normalize(light_pos - gPosition);
	float brightness = abs(dot(to_surface, gNormal));
	brightness = clamp(brightness, 0.0, 1.0);

	color.rgb *= brightness;
	fColor = color;
}
