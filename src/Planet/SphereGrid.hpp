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
		std::size_t count_data_cache;
		bool instance_data_cached;
		GLuint grid_instance_vbo;
		std::array<std::unique_ptr<GridTree>, 6> tree;
		std::array<Graphics::OpenGL::Texture, 6> textures;

		void uploadInstanceData();
		std::size_t countLeavesInternal() const;

	public:
		std::size_t countLeaves();
		std::size_t countLeaves() const;
		void draw();
		void buildFromPoint(const glm::vec3 &camera_pos);

		SphereGrid(int divs, float radius);
	};
}
