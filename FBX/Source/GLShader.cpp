#include <fstream>
#include <iostream>

#include "GLShader.h"

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
	_program = glCreateProgram();
	glAttachShader(_program, _vertex_shader);
	glAttachShader(_program, _geometry_shader);
	glAttachShader(_program, _fragment_shader);
	glAttachShader(_program, _compute_shader);

	if (LinkCallback != nullptr)
		LinkCallback(_program);

	glLinkProgram(_program);
	int linkSuccess = 0;
	glGetProgramiv(_program, GL_LINK_STATUS, &linkSuccess);
	if (!linkSuccess)
	{
		char errorBuffer[4096];
		int error_len;
		glGetProgramInfoLog(_program, 4096, &error_len,
			errorBuffer);
		std::cout << "[LING ERROR] : " << errorBuffer << std::endl;
	}
}

void GLShader::Destroy()
{
	glDeleteShader(_compute_shader);
	glDeleteShader(_vertex_shader);
	glDeleteShader(_geometry_shader);
	glDeleteShader(_fragment_shader);
	glDeleteProgram(_program);
}

uint32_t GLShader::CheckShaderType(uint32_t type)
{
	uint32_t shader_id = glCreateShader(type);

	switch (type) 
	{
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
