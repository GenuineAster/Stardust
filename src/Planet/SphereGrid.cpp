#include "Planet/SphereGrid.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace Planet
{
	std::array<glm::mat4, 6> SphereGrid::getPlaneTransforms() {
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
				set_model(getPlaneTransforms()[i]*model);
			});
		}
	}

	void SphereGrid::buildFromPoint(const glm::vec3 &camera_pos) {
		const glm::vec3 &segment_end = camera_pos;

		for (auto i = 0u; i < tree.size(); ++i) {
			const glm::vec3 plane_origin = glm::vec3(getPlaneTransforms()[i] * glm::vec4(0.f, 0.f, 0.f, 1.f));
			const glm::vec3 plane_normal = glm::mat3(getPlaneTransforms()[i]) * glm::vec3{0.f, 0.f, 1.f};

			const glm::vec3 &u = segment_end;
			const glm::vec3 w = -plane_origin;

			const float N = -dot(plane_normal, w);
			const float D =  dot(plane_normal, u);

			float sI = N / D;

			if (sI < 0.0) {
				sI *= 10000.0;
			}

			const glm::vec3 intersect = sI * u;
			const glm::vec2 transformed_pos = glm::vec2(glm::inverse(getPlaneTransforms()[i])*glm::vec4(intersect, 1.f));

			const float max_depth = std::floor(11.0 - std::sqrt((glm::length(camera_pos) - radius) / 50.f));
			tree[i] = std::make_unique<GridTree>(max_depth, 0, transformed_pos, glm::vec2{0.0, 0.0}, glm::vec2{0.5, 0.5});
		}
	}

	SphereGrid::SphereGrid(int divs, float radius) :
		radius(radius),
		grid(divs)
	{}
}
