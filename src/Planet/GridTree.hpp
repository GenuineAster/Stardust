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
		const glm::vec2 origin;
		const glm::vec2 half_size;
		const GridTree *parent;
		std::vector<GridTree> nodes;

	public:
		std::size_t countLeaves() const;
		void getAttribs(const std::function<void(const glm::mat4&)> &add_attribs) const;
		GridTree(int max_depth, int depth, const glm::vec2 &position, const glm::vec2 &origin, const glm::vec2 &half_size, const GridTree *parent=nullptr);
	};
}
