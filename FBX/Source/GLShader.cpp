#include "GLShader.h"
#include "GL/glew.h"

void GLShader::LoadShader(uint32_t type, const char * path)
{
	CheckShaderType(type);
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

void GLShader::CheckShaderType(uint32_t type)
{
	uint32_t shaderID;
	shaderID = glCreateShader(type);
	switch (type) {
	case GL_VERTEX_SHADER:
		_vertex_shader = shaderID;
		break;
	case GL_FRAGMENT_SHADER:
		_fragment_shader = shaderID;
		break;
	case GL_GEOMETRY_SHADER:
		_geometry_shader = shaderID;
		break;
	case GL_COMPUTE_SHADER:
		_compute_shader = shaderID;
		break;
	default:
		break;
	}
}
