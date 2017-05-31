#include "FBXLoader.h"

FBXLoader::FBXLoader()
{
}

FBXLoader::~FBXLoader()
{
}

void FBXLoader::Init()
{
	_manager = FbxManager::Create();

	FbxIOSettings* settings = FbxIOSettings::Create(_manager, IOSROOT);
	_manager->SetIOSettings(settings);
}

void FBXLoader::ConvertAxisSystem()
{
	FbxAxisSystem scene_axis_system = _scene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem actual_axis_system(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	if (scene_axis_system != actual_axis_system)
		actual_axis_system.ConvertScene(_scene);
}

void FBXLoader::UseCentimeter()
{
	FbxSystemUnit SceneSystemUnit = _scene->GetGlobalSettings().GetSystemUnit();
	if (fabs(SceneSystemUnit.GetScaleFactor() - 1.0) > 0.00001)
	{
		FbxSystemUnit actual_system_unit(1.0);
		actual_system_unit.ConvertScene(_scene);
	}
}

void FBXLoader::ProcessNode(const FbxNode * node, const FbxNode * parent)
{
	const FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute) {
		FbxNodeAttribute::EType type;
		type = attribute->GetAttributeType();
		switch (type)
		{
		case FbxNodeAttribute::eMesh:
			LoadMesh(node);
			break;
		case FbxNodeAttribute::eSkeleton:
			break;
		case FbxNodeAttribute::eCamera:
			break;
		case FbxNodeAttribute::eLight:
			break;
		default: break;
		}
	}

	int child_count = node->GetChildCount();
	for (int i = 0; i < child_count; i++)
	{
		const FbxNode* child = node->GetChild(i);
		ProcessNode(child, node);
	}
}

void FBXLoader::LoadMesh(const FbxNode * node)
{
}

void FBXLoader::LoadScene(const char * path)
{
	_scene = FbxScene::Create(_manager, path);
	FbxImporter* importer = FbxImporter::Create(_manager, "");

	bool status = importer->Initialize(path, -1, _manager->GetIOSettings());
	status = importer->Import(_scene);
	importer->Destroy();

	ConvertAxisSystem();
	UseCentimeter();
}

void FBXLoader::Draw()
{
}
