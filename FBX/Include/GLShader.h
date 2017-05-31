#ifndef __GLSHADER_H_INCLUDED__
#define __GLSHADER_H_INCLUDED__

#include <cstdint>

class GLShader
{
public:
	GLShader() = default;
	~GLShader() = default;

	GLShader(GLShader const&&) = delete;
	GLShader(GLShader &&) = delete;
	GLShader operator=(GLShader const&) = delete;
	GLShader operator=(GLShader &&) = delete;

	void LoadShader(uint32_t type, const char* path);
	void Create();
	void Bind();
	void Unbind();

	inline uint32_t GetProgram() { return _program; }

private:
	void CheckShaderType(uint32_t type);

	uint32_t _program;
	uint32_t _vertex_shader;
	uint32_t _geometry_shader;
	uint32_t _fragment_shader;
	uint32_t _compute_shader;
};

#endif // !__GLSHADER_H_INCLUDED__
