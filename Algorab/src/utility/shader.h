#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader { //adapted from
public:
	unsigned int ID;

	Shader(std::string vertexPath, std::string fragmentPath) {

		std::string vertexCode, fragmentCode;
		std::ifstream vertShaderFile, fragShaderFile;

		vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit); //ensure ifstream objects can throw exceptions
		fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vertShaderFile.open(vertexPath);
			fragShaderFile.open(fragmentPath);
			std::stringstream vsStream, fsStream;

			vsStream << vertShaderFile.rdbuf();
			fsStream << fragShaderFile.rdbuf();

			vertShaderFile.close();
			fragShaderFile.close();

			vertexCode = vsStream.str();
			fragmentCode = fsStream.str();

		} catch (std::ifstream::failure e) {
			std::cout << "Error: Shader file not successfully read" << std::endl;
		}

		const char* vertShaderCode = vertexCode.c_str();
		const char* fragShaderCode = fragmentCode.c_str();

		//BUILD
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		//vertex
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "Error: Vertex shader compilation failed\n" << infoLog << std::endl;
		};

		//fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "Error: Fragment shader compilation failed\n" << infoLog << std::endl;
		};

		//shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "Error: Shader linking failed\n" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}




	void use() const { //activate shader
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const {
		use();
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int& value) const {
		use();
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setUInt(const std::string& name, unsigned int& value) const {
		use();
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float& value) const {
		use();
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setMat4(const std::string& name, glm::mat4& value) const {
		use();
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}
	void setVec3(const std::string& name, glm::vec3& value) const {
		use();
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}
};
