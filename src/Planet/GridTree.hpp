#pragma once
#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "Planet/Grid.hpp"

namespace Planet
{
	class GridTree
	{
	private:
		glm::vec3 origin;
		glm::vec3 half_size;
		GridTree *parent;
		std::vector<GridTree> nodes;

	public:
		std::size_t countLeaves() const;
		void draw(const Grid &grid, const std::function<void(const glm::mat4&)> &set_model) const;
		GridTree(int max_depth, int depth, glm::vec3 position, glm::vec3 origin, glm::vec3 half_size, GridTree *parent=nullptr);
	};
}