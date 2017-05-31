#ifndef __MATERIAL_H_INCLUDED__
#define __MATERIAL_H_INCLUDED__

#include <cstdint>

#include "fbxsdk.h"

class Material
{
public:
	FbxDouble3 emissive[3];
	FbxDouble3 ambient[3];
	FbxDouble3 diffuse[3];
	FbxDouble3 specular[3];

	int has_diffuse;
	int has_specular;
	float has_normal;
	float shininess;

	uint32_t _diffuse_tex;
	uint32_t _specular_tex;
	uint32_t _normal_tex;
};

#endif // !__MATERIAL_H_INCLUDED__