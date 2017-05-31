#include "Mesh.h"
#include "GL/glew.h"

#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#endif

void Mesh::CreateMesh(const std::vector<float>& vertices, const std::vector<uint16_t>& indices)
{
	_nb_indices = indices.size();

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER
		, vertices.size() * sizeof(float), vertices.data()
		, GL_STATIC_DRAW);

	glGenBuffers(1, &_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		, _nb_indices * sizeof(uint16_t), indices.data()
		, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::DestroyMesh()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ibo);

	std::vector<Material>::iterator first = _materials.begin();
	std::vector<Material>::iterator last = _materials.end();
	uint16_t i = 0;

	for (; first != last; ++first)
	{
		if (_materials[i]._diffuse_tex)
			glDeleteTextures(1, &_materials[i]._diffuse_tex);
		if (_materials[i]._normal_tex)
			glDeleteTextures(1, &_materials[i]._normal_tex);
		if (_materials[i]._specular_tex)
			glDeleteTextures(1, &_materials[i]._specular_tex);
	}
}

void Mesh::Draw()
{
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _nb_indices, GL_UNSIGNED_SHORT, nullptr);
}
