#include "Planet/GridTree.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

namespace Planet
{
	std::size_t GridTree::countLeaves() const {
		std::size_t accumulator = 0;
		if (!nodes.empty()) {
			for (auto &node : nodes) {
				accumulator += node.countLeaves();
			}
		} else {
			++accumulator;
		}
		return accumulator;
	}

	void GridTree::getAttribs(const std::function<void(const glm::mat4&)> &add_attribs) const {
		if (!nodes.empty()) {
			for (auto &node : nodes) {
				node.getAttribs(add_attribs);
			}
		} else {
			// The grid has a size of [2, 2], so the scale can remain the half_size
			glm::vec3 scale = glm::vec3{half_size.x, half_size.y, half_size.y};

			// Upload uniform and draw
			add_attribs(glm::scale(glm::translate(glm::mat4(1.f), glm::vec3{origin, 0.0}), scale));
		}
	}

	GridTree::GridTree(const int max_depth, const int depth, const glm::vec2 &position, const glm::vec2 &origin, const glm::vec2 &half_size, const GridTree *parent) :
		origin(origin),
		half_size(half_size),
		parent(parent)
	{
		// Check whether the position is within half_size distance of the node's center or four corners
		// This is much simpler to implement than real square-to-sphere collision testing
		bool collision = false;
		const auto length = glm::length2(4.f*half_size);
		const auto offset = position - origin;
		if (glm::length2(position - origin) < length) {
			collision = true;
		} else if(glm::length2(offset + half_size*glm::vec2{ 1.f, 1.f}) < length) {
			collision = true;
		} else if(glm::length2(offset + half_size*glm::vec2{ 1.f,-1.f}) < length) {
			collision = true;
		} else if(glm::length2(offset + half_size*glm::vec2{-1.f, 1.f}) < length) {
			collision = true;
		} else if(glm::length2(offset + half_size*glm::vec2{-1.f,-1.f}) < length) {
			collision = true;
		}

		// If there's a collision, subdivide the tree
		if (depth < max_depth && collision) {
			nodes = std::vector<GridTree>{
				{max_depth, depth+1, position, origin + half_size*glm::vec2{ 0.5f, 0.5f}, half_size*0.5f, this},
				{max_depth, depth+1, position, origin + half_size*glm::vec2{ 0.5f,-0.5f}, half_size*0.5f, this},
				{max_depth, depth+1, position, origin + half_size*glm::vec2{-0.5f, 0.5f}, half_size*0.5f, this},
				{max_depth, depth+1, position, origin + half_size*glm::vec2{-0.5f,-0.5f}, half_size*0.5f, this}
			};
		}
	}
}
