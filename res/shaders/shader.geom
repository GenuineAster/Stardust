#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform int uWater;
uniform float uWaterLevel;
uniform float uNear, uFar;

out vec3 gNormal;
out vec3 gPosition;
out float gHeight;

float anoise(vec3 P);

void fixDepth(inout vec4 P) {
	P.z = 2.0*log(P.w/uNear)/log(uFar/uNear) - 1; 
    P.z *= P.w;
}

void main() {
	vec3 positions[3];
	vec3 heights;

	mat4 transform = uView * uModel;

	for (int i = 0; i < 3; ++i) {
		vec3 grid_pos = vec3(gl_in[i].gl_Position);
		positions[i] = normalize(grid_pos);
		heights[i] = anoise(positions[i]*1.5);
	}

	if (uWater == 0) {
		vec4 calc_positions[3];
		for (int i = 0; i < 3; ++i) {
			calc_positions[i] = transform * vec4(positions[i] * (1000.0 + 50.0 * heights[i]), 1.0);
		}

		gNormal = normalize(cross(
			vec3(calc_positions[1] - calc_positions[0]),
			vec3(calc_positions[2] - calc_positions[0])
		));

		gNormal = gNormal * sign(gNormal.z);

		for (int i = 0; i < 3; ++i) {
			gHeight = heights[i];
			gPosition = vec3(calc_positions[i]);
			gl_Position = uProjection * calc_positions[i];
			fixDepth(gl_Position);
			EmitVertex();
		}
		EndPrimitive();
	} else {
		float wlvl = uWaterLevel + 0.04;
		bool water = any(lessThan(heights, vec3(
			wlvl, wlvl, wlvl
		)));

		if (water) {
			gHeight = uWaterLevel;
			for (int i = 0; i < 3; ++i) {
				gNormal = positions[i];
				vec4 tmp = transform * vec4(positions[i] * (1000.0 + 50.0 * gHeight), 1.0);
				gPosition = vec3(tmp);
				gl_Position = uProjection * tmp;
				fixDepth(gl_Position);
				EmitVertex();
			}
			EndPrimitive();
		}
	}
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
