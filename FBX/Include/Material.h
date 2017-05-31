#ifndef __MATERIAL_H_INCLUDED__
#define __MATERIAL_H_INCLUDED__

#include <cstdint>

class Material
{
public:
	float emissive[3]; 
	float ambient[3]; 
	float diffuse[3]; 
	float specular[3]; 

	int has_diffuse;
	int has_specular;
	float has_normal;
	float shininess;

	uint32_t _diffuse_tex;
	uint32_t _specular_tex;
	uint32_t _normal_tex;
};

#endif // !__MATERIAL_H_INCLUDED__