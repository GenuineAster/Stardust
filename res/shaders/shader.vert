#version 330

layout(location=0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 grid;
uniform int deform;

out vec3 normal;

void main() {
	vec4 final_pos;
	if (deform == 1) {
		vec3 grid_pos = vec3(grid * vec4(position, 1.0));
		float theta = acos(grid_pos.z / length(grid_pos));
		float phi   = atan(grid_pos.y, grid_pos.x+0.0001);

		vec3 sphere_position;
		float height = 1000.0 + 500.0 * sin(grid_pos.x*grid_pos.y*grid_pos.z);
		sphere_position.x = height * sin(theta) * cos(phi);
		sphere_position.y = height * sin(theta) * sin(phi);
		sphere_position.z = height * cos(theta);

		normal = normalize(mat3(model) * sphere_position);

		final_pos = model * vec4(sphere_position, 1.0);
	}
	else {
		normal = normalize(mat3(model) * vec3(grid * vec4(0.0, 0.0, 1.0, 1.0)));

		final_pos = model * grid * vec4(position, 1.0);
	}

	gl_Position = projection * final_pos;
}
