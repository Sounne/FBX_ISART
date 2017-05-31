#ifndef __FBXLOADER_H_INCLUDED__
#define __FBXLOADER_H_INCLUDED__

#include <memory>
#include <vector>

#define FBXSDK_SHARED
#include "fbxsdk.h"

#ifdef FBXSDK_SHARED
#pragma comment(lib, "libfbxsdk.lib")
#else
#pragma comment(lib, "libfbxsdk-md.lib")
#endif

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
	void ConvertAxisSystem();
	void UseCentimeter();
	void ProcessNode(const FbxNode* node, const FbxNode* parent);
	void LoadMesh(const FbxNode* node);
	void LoadScene(const char* path);
	void Draw();

private:
	FbxManager* _manager;
	FbxScene* _scene;

	std::vector<Mesh> _meshes;
};

#endif // !__FBXLOADER_H_INCLUDED__
