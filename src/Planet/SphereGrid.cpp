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

	std::size_t SphereGrid::countLeavesInternal() const {
		if (!count_data_cache) {
			std::size_t accumulator = 0;
			for (auto &node : tree) {
				accumulator += node->countLeaves();
			}
			return accumulator;
		} else {
			return count_data_cache;
		}
	}

	std::size_t SphereGrid::countLeaves() const {
		return countLeavesInternal();
	}

	std::size_t SphereGrid::countLeaves() {
		count_data_cache = countLeavesInternal();
		return count_data_cache;
	}

	void SphereGrid::uploadInstanceData() {
		if (!instance_data_cached) {
			auto count = this->countLeaves();

			std::vector<glm::mat4> grid_displacements;
			grid_displacements.reserve(count);

			for (unsigned int i = 0; i < tree.size(); ++i) {
				tree[i]->getAttribs([&](const glm::mat4 &t){grid_displacements.push_back(getPlaneTransforms()[i]*t);});
			}

			glBindBuffer(GL_ARRAY_BUFFER, grid_instance_vbo);
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), grid_displacements.data(), GL_STATIC_DRAW);

			instance_data_cached = true;
		}
	}

	void SphereGrid::draw() {
		this->uploadInstanceData();

		auto count = this->countLeaves();
		grid.bind();
		grid.drawInstanced(count);
	}

	void SphereGrid::buildFromPoint(const glm::vec3 &camera_pos) {
		const glm::vec3 &segment_end = camera_pos;

		instance_data_cached = false;
		count_data_cache = 0;

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
		grid(divs),
		count_data_cache(0),
		instance_data_cached(false)
	{
		grid.bind();
		glGenBuffers(1, &grid_instance_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, grid_instance_vbo);
		for (int i = 0; i < 4; ++i) {
			glEnableVertexAttribArray(1+i);
			glVertexAttribPointer(1+i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)*i));
			glVertexAttribDivisor(1+i, 1);
		}
	}
}
