#include "Planet/Planet.hpp"

#include <fstream>
#include <json/json.h>

namespace Planet
{
	void Planet::loadFromFile(const std::string &file) {
		Json::Value planet_desc;
		{
			std::ifstream stream{file};
			Json::Reader{}.parse(stream, planet_desc);
		}

		std::vector<glm::vec4> gradient;
		for (auto &color : planet_desc["gradient"]) {
			gradient.emplace_back(
				color[0].asFloat(),
				color[1].asFloat(),
				color[2].asFloat(),
				color[3].asFloat()
			);
		}

		gradient_tex.create();
		gradient_tex.bind(GL_TEXTURE0, GL_TEXTURE_1D);
		gradient_tex.texImage1D(0, GL_RGBA, gradient.size(), GL_RGBA, GL_FLOAT, gradient.data());

		water_level = planet_desc["water_level"].asFloat();
	}
}

