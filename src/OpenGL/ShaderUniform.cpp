#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGL/ShaderProgram.hpp"

namespace Graphics
{
	namespace OpenGL
	{
		/*! \brief Uploads a 3D vector of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::vec3>(
			Uniform &uniform,
			const glm::vec3 &data
		) {
			glUniform3f(uniform, data.x, data.y, data.z);
		}

		/*! \brief Uploads a 3D vector of int to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::ivec3>(
			Uniform &uniform,
			const glm::ivec3 &data
		) {
			glUniform3i(uniform, data.x, data.y, data.z);
		}

		/*! \brief Uploads a 3D 3x3 matrix of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::mat3>(
			Uniform &uniform,
			const glm::mat3 &data
		) {
			glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(data));
		}

		/*! \brief Uploads a 3D 4x4 matrix of float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<glm::mat4>(
			Uniform &uniform,
			const glm::mat4 &data
		) {
			glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(data));
		}


		/*! \brief Uploads a texture sampler number to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<Texture>(
			Uniform &uniform,
			const Texture &data
		) {
			glUniform1i(uniform, data.getTextureNum()-GL_TEXTURE0);
		}

		/*! \brief Uploads an integer to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLint>(
			Uniform &uniform,
			const GLint &data
		) {
			glUniform1i(uniform, data);
		}

		/*! \brief Uploads an unsigned integer to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLuint>(
			Uniform &uniform,
			const GLuint &data
		) {
			glUniform1ui(uniform, data);
		}

		/*! \brief Uploads a float to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLfloat>(
			Uniform &uniform,
			const GLfloat &data
		) {
			glUniform1f(uniform, data);
		}

		/*! \brief Uploads a double to the given uniform
		 */
		template<>
		void ShaderProgram::setUniformData<GLdouble>(
			Uniform &uniform,
			const GLdouble &data
		) {
			glUniform1d(uniform, data);
		}
	}
}
