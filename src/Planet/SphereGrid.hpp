#pragma once
#include <array>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include "OpenGL/Texture.hpp"

#include "Planet/Grid.hpp"
#include "Planet/GridTree.hpp"

namespace Planet
{
	class SphereGrid
	{
	private:
		static std::array<glm::mat4, 6> getPlaneTransforms();

		float radius;
		Grid grid;
		std::array<std::unique_ptr<GridTree>, 6> tree;
		std::array<Graphics::OpenGL::Texture, 6> textures;

	public:
		std::size_t countLeaves() const;
		void draw(const std::function<void(const glm::mat4&)> &set_model) const;
		void buildFromPoint(const glm::vec3 &camera_pos);

		SphereGrid(int divs, float radius);
	};
}
