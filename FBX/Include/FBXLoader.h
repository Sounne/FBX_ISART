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

#include "GL/glew.h"
#include "Mesh.h"
#include "GLShader.h"

class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	FBXLoader(FBXLoader const&) = delete;
	FBXLoader(FBXLoader &&) = delete;
	FBXLoader operator=(FBXLoader&&) = delete;
	FBXLoader operator=(FBXLoader const&) = delete;

	inline void SetProgram(GLShader* new_program) { _program = new_program; }

	GLuint LoadTexture(const char* path, bool linear = false);
	void Init();
	void ProcessNode(const FbxNode* node, const FbxNode* parent);
	void LoadMesh(const FbxNode* node);
	void LoadScene(const char* path);
	void Render();
	void Shutdown();

private:
	void ConvertAxisSystem();
	void UseCentimeter();

	FbxManager* _manager;
	FbxScene* _scene;
	GLShader* _program;

	std::vector<Mesh> _meshes;
};

#endif // !__FBXLOADER_H_INCLUDED__
