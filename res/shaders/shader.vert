#version 330

layout(location=0) in vec3 position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 grid;
uniform int deform;

out vec3 vNormal;
out float vHeight;

float snoise(vec3 v);
float anoise(vec3 P);

vec3 getSpherePos(const in float theta, const in float phi) {
	return vec3(
		sin(theta) * cos(phi),
		sin(theta) * sin(phi),
		cos(theta)
	);
}

void main() {
	vec4 final_pos;
	if (deform == 1) {
		vec3 grid_pos = vec3(grid * vec4(position, 1.0));
		float theta = acos(grid_pos.z / length(grid_pos));
		float phi   = atan(grid_pos.y, grid_pos.x+0.0001);
		vec3 sphere_position = getSpherePos(theta, phi);
		vNormal = sphere_position;
		vHeight = anoise(sphere_position*1.5);
		float height = 1000.0 + 50.0 * vHeight;
		sphere_position *= height;
		final_pos = model * vec4(sphere_position, 1.0);
	}
	else {
		vNormal = normalize(mat3(model) * vec3(grid * vec4(0.0, 0.0, 1.0, 1.0)));
		vHeight = -2.0;
		final_pos = model * grid * vec4(position, 1.0);
	}

	gl_Position = projection * final_pos;
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

float anoise_(vec3 P) {
	return snoise(P);
}

float anoise(vec3 P) {
	return fbm(P/0.33);
}
