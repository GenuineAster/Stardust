#pragma once
#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "Planet/SphereGrid.hpp"

namespace Planet
{
	class Planet
	{
	public:
		float water_level;
		Graphics::OpenGL::Texture gradient_tex;
		std::unique_ptr<SphereGrid> grid;

		void loadFromFile(const std::string &file);
	};
}
