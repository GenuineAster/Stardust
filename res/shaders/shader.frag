#version 330

in vec3 gPosition;
in vec3 gNormal;
in float gHeight;

uniform mat4 view;
uniform int uWater;

out vec4 fColor;

const vec3 water = vec3(0.00, 0.00, 1.00);
const vec3 sand  = vec3(0.75, 0.75, 0.00);
const vec3 grass = vec3(0.20, 0.70, 0.20);
const vec3 rock  = vec3(0.40, 0.40, 0.10);
const vec3 snow  = vec3(0.80, 0.80, 0.80);

void main() {
	float height = gHeight/2.0+0.5;

	vec3 color;
	if (uWater == 1) color = vec3(0.7, 0.7, 1.0);
	else if (height > 0.80) color = snow;
	else if (height > 0.70) color = mix(snow, rock,  (0.80 - height) / (0.80 - 0.70));
	else if (height > 0.55) color = mix(rock, grass, (0.70 - height) / (0.70 - 0.55));
	else if (height > 0.50) color = mix(grass, sand, (0.55 - height) / (0.55 - 0.50));
	else if (height > 0.49) color = mix(sand, water, (0.50 - height) / (0.50 - 0.49));
	else if (height >= 0.0) color = water;
	else color = water;

	vec3 light_pos = vec3(0.0, 10.0, -10.0);
	vec3 to_surface = normalize(light_pos - gPosition);
	float brightness = abs(dot(to_surface, gNormal));
	brightness = clamp(brightness, 0.0, 1.0);

	fColor = vec4(color * brightness, (uWater == 1) ? 0.8 : 1.0);
}
