#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 gNormal;
out vec3 gPosition;
out float gHeight;

float anoise(vec3 P);

vec3 getSpherePos(const in float theta, const in float phi) {
	return vec3(
		sin(theta) * cos(phi),
		sin(theta) * sin(phi),
		cos(theta)
	);
}

void main() {
	vec3 grid_pos[3];
	vec4 positions[3];
	float heights[3];

	for (int i = 0; i < 3; ++i) {
		grid_pos[i] = vec3(gl_in[i].gl_Position);
		float theta = acos(grid_pos[i].z / length(grid_pos[i]));
		float phi   = atan(grid_pos[i].y, grid_pos[i].x+0.0001);
		vec3 sphere_position = getSpherePos(theta, phi);
		heights[i] = anoise(sphere_position*1.5);
		float height = 1000.0 + 50.0 * heights[i];
		sphere_position *= height;
		positions[i] = view * model * vec4(sphere_position, 1.0);
	}

	gNormal = normalize(cross(
		vec3(positions[1] - positions[0]),
		vec3(positions[2] - positions[0])
	));
	gNormal = gNormal * sign(gNormal.z);

	for (int i = 0; i < 3; ++i) {
		gHeight = heights[i];
		gPosition = vec3(positions[i]);
		gl_Position = projection * positions[i];
		EmitVertex();
	}
	EndPrimitive();
}

// From https://gist.github.com/patriciogonzalezvivo/670c22f3966e662d2f83
#define NUM_OCTAVES 11

float mod289(float x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 mod289(vec4 x){return x - floor(x * (1.0 / 289.0)) * 289.0;}
vec4 perm(vec4 x){return mod289(((x * 34.0) + 1.0) * x);}

float noise(vec3 p){
    vec3 a = floor(p);
    vec3 d = p - a;
    d = d * d * (3.0 - 2.0 * d);

    vec4 b = a.xxyy + vec4(0.0, 1.0, 0.0, 1.0);
    vec4 k1 = perm(b.xyxy);
    vec4 k2 = perm(k1.xyxy + b.zzww);

    vec4 c = k2 + a.zzzz;
    vec4 k3 = perm(c);
    vec4 k4 = perm(c + 1.0);

    vec4 o1 = fract(k3 * (1.0 / 41.0));
    vec4 o2 = fract(k4 * (1.0 / 41.0));

    vec4 o3 = o2 * d.z + o1 * (1.0 - d.z);
    vec2 o4 = o3.yw * d.x + o3.xz * (1.0 - d.x);

    return o4.y * d.y + o4.x * (1.0 - d.y);
}

float fbm(vec3 x) {
	float v = 0.0;
	float a = 0.5;
	vec3 shift = vec3(100);
	for (int i = 0; i < NUM_OCTAVES; ++i) {
		v += a * noise(x/2.0);
		x = x * 2.0 + shift;
		a *= 0.5;
	}
	return v * 2.0 - 1.0;
}

float anoise(vec3 P) {
	return fbm(P/0.33);
}
