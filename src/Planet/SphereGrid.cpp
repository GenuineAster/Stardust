#include "Planet/SphereGrid.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Planet
{
	std::array<glm::mat4, 6> SphereGrid::getPositionTransforms() {
		return {
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), {-0.5, 0.0, 0.0}), glm::radians(90.f), glm::vec3{0.0, 1.0, 0.0})),
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), {+0.5, 0.0, 0.0}), glm::radians(90.f), glm::vec3{0.0, 1.0, 0.0})),
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), { 0.0,-0.5, 0.0}), glm::radians(90.f), glm::vec3{1.0, 0.0, 0.0})),
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), { 0.0,+0.5, 0.0}), glm::radians(90.f), glm::vec3{1.0, 0.0, 0.0})),
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), { 0.0, 0.0,-0.5}), glm::radians(90.f), glm::vec3{0.0, 0.0, 1.0})),
			glm::mat4(glm::rotate(glm::translate(glm::mat4(1.f), { 0.0, 0.0,+0.5}), glm::radians(90.f), glm::vec3{0.0, 0.0, 1.0}))
		};
	}

	std::size_t SphereGrid::countLeaves() const {
		std::size_t accumulator = 0;
		for (auto &node : tree) {
			accumulator += node->countLeaves();
		}
		return accumulator;
	}

	void SphereGrid::draw(const std::function<void(const glm::mat4&)> &set_model) const {
		grid.bind();
		for (auto i = 0u; i < tree.size(); ++i) {
			tree[i]->draw(grid, [&](auto model){
				set_model(getPositionTransforms()[i]*model);
			});
		}
	}

	void SphereGrid::buildFromPoint(const glm::vec3 &camera_pos) {
		const auto scaled = camera_pos / radius / 2.f;
		for (auto i = 0u; i < tree.size(); ++i) {
			glm::vec3 transformed_pos = glm::vec3(glm::inverse(getPositionTransforms()[i])*glm::vec4(scaled, 1.0));
			tree[i] = std::make_unique<GridTree>(10, 0, transformed_pos, glm::vec3{0.0, 0.0, 0.0}, glm::vec3{0.5, 0.5, 0.0});
		}
	}

	SphereGrid::SphereGrid(int divs, float radius) :
		radius(radius),
		grid(divs)
	{}
}
