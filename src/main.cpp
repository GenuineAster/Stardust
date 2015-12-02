#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/io.hpp>

#include <GLFW/glfw3.h>

#include "OpenGL/Shader.hpp"
#include "OpenGL/ShaderProgram.hpp"
#include "OpenGL/Framebuffer.hpp"

#include <array>
#include <thread>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <exception>
#include <functional>

#include <json/json.h>

#include "Planet/Planet.hpp"
#include "Util/FrameTimer.hpp"

constexpr struct {float x,y;} resolution {1280, 720};

int main() {
	// Window/context setup
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DEPTH_BITS, 32);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(resolution.x, resolution.y, "Stardust", NULL, NULL);
	if (!window) {
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		return -1;
	}

	if (!glewIsSupported("GL_VERSION_3_3")) {
		std::cout<<"OpenGL 3.3 is required to run Stardust!"<<std::endl;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);


	// Shader setup
	Graphics::OpenGL::Shader vertex(GL_VERTEX_SHADER);
	vertex.setSourceFromFile("res/shaders/shader.vert");
	vertex.compile();

	Graphics::OpenGL::Shader geometry(GL_GEOMETRY_SHADER);
	geometry.setSourceFromFile("res/shaders/shader.geom");
	geometry.compile();

	Graphics::OpenGL::Shader fragment(GL_FRAGMENT_SHADER);
	fragment.setSourceFromFile("res/shaders/shader.frag");
	fragment.compile();

	Graphics::OpenGL::ShaderProgram shader;
	shader.create();
	shader.attach(vertex);
	shader.attach(geometry);
	shader.attach(fragment);
	shader.link();
	shader.bindFragDataLocation("fColor", 0);
	shader.use();

	// Uniform setup/defaults
	float near = 0.1f;
	auto near_uniform = shader.getUniformLocation("uNear");
	shader.setUniformData(near_uniform, near);

	float far = 10000.f;
	auto far_uniform = shader.getUniformLocation("uFar");
	shader.setUniformData(far_uniform, far);

	glm::mat4 projection = glm::perspective(glm::radians(90.f), resolution.x/resolution.y, near, far);
	auto projection_uniform = shader.getUniformLocation("uProjection");
	shader.setUniformData(projection_uniform, projection);

	glm::mat4 model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
	auto model_uniform = shader.getUniformLocation("uModel");
	shader.setUniformData(model_uniform, model);


	glm::vec3 eye {0.f, 1020.f, 0.f};
	glm::vec3 center {5.f, 1015.f, -5.f};
	glm::vec3 up {0.f, 1.f, 0.f};

	glm::mat4 view = glm::lookAt(eye, center, up);
	auto view_uniform = shader.getUniformLocation("uView");
	shader.setUniformData(view_uniform, view);

	auto water_uniform = shader.getUniformLocation("uWater");
	auto water_level_uniform = shader.getUniformLocation("uWaterLevel");
	auto gradient_uniform = shader.getUniformLocation("uPlanetGradient");

	// Read planet list from file
	Json::Value planet_list;
	{
		std::ifstream stream("res/planets.json");
		if (!stream) {
			throw std::runtime_error("File res/planets.json not found.");
		}
		Json::Reader{}.parse(stream, planet_list);
		planet_list = planet_list["planets"];
	}

	// Load listed planets
	std::vector<std::unique_ptr<Planet::Planet>> planets;
	for (auto &planet : planet_list) {
		planets.push_back(std::make_unique<Planet::Planet>());
		planets.back()->loadFromFile(planet["file"].asString());
	}

	// Planet sphere grid setup
	Planet::SphereGrid cube(16, 1000.f);

	Util::FrameTimer frame_timer;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window)) {
		// Only time the actual frame code
		frame_timer.start = std::chrono::high_resolution_clock::now();

		{
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			eye.y *= 1.00009;
			view = glm::lookAt(eye, center, up);
			shader.setUniformData(view_uniform, view);

			model = glm::rotate(model, glm::two_pi<float>() / 20000.f, {0.1, 0.2, 0.3});
			shader.setUniformData(model_uniform, model);
			
			for (auto &planet : planets) {
				cube.buildFromPoint(glm::vec3(glm::inverse(glm::mat3(model)) * eye));

				shader.setUniformData(water_level_uniform, planet->water_level);
				shader.setUniformData(gradient_uniform, planet->gradient_tex);
				shader.setUniformData(water_uniform, 0);
				cube.draw();
				shader.setUniformData(water_uniform, 1);
				cube.draw();
			}

			glfwSwapBuffers(window);
		}

		frame_timer.end = std::chrono::high_resolution_clock::now();

		frame_timer.sum += frame_timer.getSeconds();
		frame_timer.count++;

		frame_timer.sleepForFPS(100);

		if (glfwGetTime() > 2.0) {
			double avg = frame_timer.sum / frame_timer.count;
			std::cout<<"avg frametime: "<<std::setw(12)<<static_cast<std::size_t>(1e9*avg)<<"ns"<<std::endl;

			frame_timer = Util::FrameTimer();
			glfwSetTime(0.0);
		}
	}
}
