#ifndef __FBXLOADER_H_INCLUDED__
#define __FBXLOADER_H_INCLUDED__

#include <memory>
#include <vector>

#include "fbxsdk.h"
#include "Mesh.h"

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	FBXLoader(FBXLoader const&) = delete;
	FBXLoader(FBXLoader &&) = delete;
	FBXLoader operator=(FBXLoader&&) = delete;
	FBXLoader operator=(FBXLoader const&) = delete;

	void Init();
	void Draw();

private:
	FbxManager* _manager;
	FbxScene* _scene;

	std::vector<Mesh> _meshes;
};

#endif // !__FBXLOADER_H_INCLUDED__
