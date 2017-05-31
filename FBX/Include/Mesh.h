#ifndef __MESH_H_INCLUDED__
#define __MESH_H_INCLUDED__

#include <cstdint>
#include <vector>

#include "Material.h"

class Mesh
{
public:
	void CreateMesh(const std::vector<float>& vertices, const std::vector<uint16_t>& indices);
	void DestroyMesh();
	void Draw();

private:
	std::vector<Material> _materials;

	uint32_t _ibo;
	uint32_t _vbo;
	uint32_t _vao;
	uint32_t _nb_indices;
};

#endif // !__MESH_H_INCLUDED__
