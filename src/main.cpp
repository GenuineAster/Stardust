#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/quaternion.hpp>

#include <GLFW/glfw3.h>

#include "OpenGL/Shader.hpp"
#include "OpenGL/ShaderProgram.hpp"

#include <array>
#include <thread>
#include <iomanip>
#include <iostream>
#include <functional>

#include "Util/FrameTimer.hpp"
#include "Planet/SphereGrid.hpp"


int main() {
	// Window/context setup
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DEPTH_BITS, 8);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Stardust", NULL, NULL);
	if (!window) {
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit()) {
		return -1;
	}

	glEnable(GL_DEPTH_TEST);


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
	glm::mat4 projection = glm::perspective(glm::radians(90.f), 640.f/480.f, 0.1f, 10000.f);
	auto projection_uniform = shader.getUniformLocation("projection");
	shader.setUniformData(projection_uniform, projection);

	glm::mat4 model = glm::translate(glm::mat4(1.f), {0.f, 0.f, 0.f});
	auto model_uniform = shader.getUniformLocation("model");
	shader.setUniformData(model_uniform, model);


	glm::vec3 eye {0.f, 2050.f, 0.f};
	glm::vec3 center {0.0f, 0.0f, 0.0f};
	glm::vec3 up {1.0, 0.f, 0.0};

	glm::mat4 view = glm::lookAt(eye, center, up);
	auto view_uniform = shader.getUniformLocation("view");
	shader.setUniformData(view_uniform, view);

	glm::mat4 grid_trans = glm::mat4(1.f);
	auto grid_trans_uniform = shader.getUniformLocation("grid");
	shader.setUniformData(grid_trans_uniform, grid_trans);

	// Planet sphere grid setup
	Planet::SphereGrid cube(16, 1000.f);

	Util::FrameTimer frame_timer;

	while (!glfwWindowShouldClose(window)) {
		// Only time the actual frame code
		frame_timer.start = std::chrono::high_resolution_clock::now();

		{
			glfwPollEvents();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			view = glm::lookAt(eye, center, up);
			shader.setUniformData(view_uniform, view);

			model = glm::rotate(model, glm::two_pi<float>() / 2000.f, {0.1, 0.2, 0.3});
			shader.setUniformData(model_uniform, model);
			
			cube.buildFromPoint(glm::vec3(glm::inverse(glm::mat3(model)) * eye));
			cube.draw([&](auto m){
				shader.setUniformData(grid_trans_uniform, m);
			});

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
