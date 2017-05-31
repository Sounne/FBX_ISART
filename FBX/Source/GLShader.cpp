#include <fstream>
#include <iostream>

#include "GLShader.h"
#include "GL/glew.h"

void GLShader::LoadShader(uint32_t type, const char * path)
{
	uint32_t shader_id = CheckShaderType(type);

	std::ifstream fileStream;
	fileStream.open(path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "FAILED TO LOAD SHADER" << std::endl;
		return;
	}

	std::streampos begin = fileStream.tellg();
	fileStream.seekg(0, std::ios::end);
	std::streamoff file_len = fileStream.tellg() - begin;
	char* buffer = new char[file_len + 1];
	fileStream.seekg(0, std::ios::beg);
	fileStream.read(buffer, file_len);
	fileStream.close();

	buffer[file_len] = '\0';
	glShaderSource(shader_id, 1, &buffer, nullptr);
	delete[] buffer;

	glCompileShader(shader_id);
	int compileSuccess = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileSuccess);
	if (!compileSuccess)
	{
		char error_buffer[4096];
		int error_len;
		glGetShaderInfoLog(shader_id, 4096, &error_len, error_buffer);
		const char* shader_type = GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT";
		std::cout << "[ERROR] : " << shader_type << " : " << error_buffer << std::endl;
	}
}

void GLShader::Create()
{
}

void GLShader::Bind()
{
}

void GLShader::Unbind()
{
}

uint32_t GLShader::CheckShaderType(uint32_t type)
{
	uint32_t shader_id;
	shader_id = glCreateShader(type);
	switch (type) {
	case GL_VERTEX_SHADER:
		_vertex_shader = shader_id;
		break;
	case GL_FRAGMENT_SHADER:
		_fragment_shader = shader_id;
		break;
	case GL_GEOMETRY_SHADER:
		_geometry_shader = shader_id;
		break;
	case GL_COMPUTE_SHADER:
		_compute_shader = shader_id;
		break;
	default:
		break;
	}

	return shader_id;
}
