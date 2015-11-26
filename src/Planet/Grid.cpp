#include "Planet/Grid.hpp"

namespace Planet
{
	void Grid::bind() const {
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	void Grid::draw() const {
		glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, nullptr);
	}

	Grid::Grid(int divs) {
		vertices.reserve((divs+1) * (divs+1));
		indices.reserve((divs+1) * divs * 4);

		for (auto i=0; i <= divs; ++i) {
			for (auto j=0; j <= divs; ++j) {
				auto vertex = 2.f * glm::vec3{i, j, 0.f} / glm::vec3{divs, divs, divs};
				vertex -= glm::vec3{1.0, 1.0, 0.0};
				vertices.push_back(vertex);

				if (i < divs) {
					indices.push_back((i+0) + (j+0) * (divs+1));
					indices.push_back((i+1) + (j+0) * (divs+1));
				}

				if (j < divs) {
					indices.push_back((i+0) + (j+0) * (divs+1));
					indices.push_back((i+0) + (j+1) * (divs+1));
				}
			}
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(decltype(vertices)::value_type) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(decltype(indices)::value_type) * indices.size(), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	Grid::~Grid() {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
	}
}