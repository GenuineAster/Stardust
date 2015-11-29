#pragma once
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>


namespace Planet
{
	class Grid
	{
	private:
		GLuint vao, vbo, ebo;
		std::vector<glm::vec3> vertices;
		std::vector<uint32_t> indices;

	public:
		void bind() const;
		void draw() const;
		void drawInstanced(const uint32_t instances) const;
		Grid(int divs);
		~Grid();
	};
}
