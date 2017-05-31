#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "stb_image.h"
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

GLuint FBXLoader::LoadTexture(const char * path, bool linear)
{
	GLuint texture_id = 0;
	int w, h, comp;

	unsigned char* image = stbi_load(path, &w, &h, &comp, STBI_default);
	if (!image) {
		std::cerr << "Unable to load texture: " << path << std::endl;
		return 0;
	}
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	if (comp == 3) {
		glTexImage2D(GL_TEXTURE_2D, 0, !linear ? GL_RGB8 : GL_SRGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	else if (comp == 4) {
		glTexImage2D(GL_TEXTURE_2D, 0, !linear ? GL_RGBA8 : GL_SRGB8_ALPHA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(image);

	return texture_id;
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
	Material meshMaterial;

	FbxNode* mutable_node = const_cast<FbxNode*>(node);
	FbxMesh* mesh = mutable_node->GetMesh();

	//auto MaterialCount = node->GetMaterialCount();
	auto MaterialCount = mesh->GetElementMaterialCount();
	FbxSurfaceMaterial* material = nullptr;
	FbxLayerElementMaterial* matLayerElement = mesh->GetElementMaterial(0);
	if (matLayerElement->GetMappingMode() == FbxLayerElement::eAllSame) {
		int indexMat = matLayerElement->GetIndexArray()[0];
		material = node->GetMaterial(0);
	}
	else {
		// FbxLayerElement::eByPolygon
	}

	FbxString type = material->ShadingModel.Get();

	const FbxProperty diffuse_property =
		material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	if (diffuse_property.IsValid()) {
		FbxDouble3 color = diffuse_property.Get<FbxDouble3>();

		const FbxProperty factorDiffuse =
			material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
		if (factorDiffuse.IsValid()) {
			double factor = factorDiffuse.Get<double>();
			color[0] *= factor; color[1] *= factor; color[2] *= factor;
		}

		int textureCount = diffuse_property.GetSrcObjectCount<FbxFileTexture>();
		FbxFileTexture* texture = diffuse_property.GetSrcObject<FbxFileTexture>(0);
		if (texture) {
			const char* filename = texture->GetFileName();
			meshMaterial._diffuse_tex = LoadTexture(filename, true);
			meshMaterial.has_diffuse = true;
		}
		meshMaterial.diffuse[0] = color[0]; meshMaterial.diffuse[1] = color[1];
		meshMaterial.diffuse[2] = color[2];
	}

	const FbxProperty specular_property =
		material->FindProperty(FbxSurfaceMaterial::sSpecular);
	if (specular_property.IsValid()) {
		FbxDouble3 color = specular_property.Get<FbxDouble3>();

		const FbxProperty factorSpecular =
			material->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
		if (factorSpecular.IsValid()) {
			double factor = factorSpecular.Get<double>();
			color[0] *= factor; color[1] *= factor; color[2] *= factor;
		}

		int textureCount = specular_property.GetSrcObjectCount<FbxFileTexture>();
		FbxFileTexture* texture = specular_property.GetSrcObject<FbxFileTexture>(0);
		if (texture) {
			const char* filename = texture->GetFileName();
			meshMaterial._specular_tex = LoadTexture(filename, true);
			meshMaterial.has_specular = true;
		}
		meshMaterial.specular[0] = color[0]; meshMaterial.specular[1] = color[1];
		meshMaterial.specular[2] = color[2];
	}

	const FbxProperty bump_property =
		material->FindProperty(FbxSurfaceMaterial::sBump);
	if (bump_property.IsValid()) {
		const FbxProperty factorBump =
			material->FindProperty(FbxSurfaceMaterial::sBumpFactor);
		if (factorBump.IsValid()) {
			double factor = factorBump.Get<double>();
			// TODO: stocker ce facteur de scale du bump
		}

		int textureCount = bump_property.GetSrcObjectCount<FbxFileTexture>();
		FbxFileTexture* texture = bump_property.GetSrcObject<FbxFileTexture>(0);
		if (texture) {
			const char* filename = texture->GetFileName();
			meshMaterial._normal_tex = LoadTexture(filename);
			meshMaterial.has_normal = true;
		}
	}

	 //Transform placant l'objet dans la scene.
	 //Passer par l'Animation Evaluator (global) accumule les transform des parents
	FbxAMatrix globalTransform =
		node->GetScene()->GetAnimationEvaluator()->GetNodeGlobalTransform(mutable_node);

	// Matrice geometrique locale.
	FbxVector4 translation = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 rotation = node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 scale = node->GetGeometricScaling(FbxNode::eSourcePivot);
	FbxAMatrix geometryTransform;
	geometryTransform.SetTRS(translation, rotation, scale);

	FbxAMatrix finalGlobalTransform = globalTransform * geometryTransform;

	//auto VertexCount = mesh->GetControlPointsCount();
	std::vector<float> vertices;

	// lecture des indices/elements
	auto IndexCount = mesh->GetPolygonVertexCount();
	std::vector<uint16_t> indices;
	//indices.reserve(IndexCount);

	// recupere la liste des canaux UV (texcoords)
	FbxStringList UVChannelNames;
	mesh->GetUVSetNames(UVChannelNames);
	auto UVChannelCount = UVChannelNames.GetCount();

	auto PolyCount = mesh->GetPolygonCount();
	// methode 1: duplication des attributs
	indices.reserve(PolyCount * 3);

	for (auto poly = 0; poly < PolyCount; poly++) {
		// faces (triangles)
		for (auto i = 0; i < 3; i++) {
			// elements
			auto index = mesh->GetPolygonVertex(poly, i);

			// methode1 - duplication des attributs
			indices.emplace_back((uint16_t)(poly * 3 + i));

			// attributs: 
			// position (x,y,z)
			FbxVector4 point = mesh->GetControlPointAt(index);
			point = finalGlobalTransform.MultT(point);
			vertices.push_back((float)point.mData[0]);
			vertices.push_back((float)point.mData[1]);
			vertices.push_back((float)point.mData[2]);
			// normal (x,y,z)
			FbxVector4 normal;
			mesh->GetPolygonVertexNormal(poly, i, normal);
			normal = finalGlobalTransform.MultT(normal);
			vertices.push_back((float)normal.mData[0]);
			vertices.push_back((float)normal.mData[1]);
			vertices.push_back((float)normal.mData[2]);
			// tex coords (s,t)
			auto uv_channel = UVChannelNames.GetStringAt(0);
			FbxVector2 texcoords;
			bool isUnMapped = false;
			bool hasUV = mesh->GetPolygonVertexUV(poly, i,
				uv_channel,
				texcoords, isUnMapped);
			vertices.push_back((float)texcoords.mData[0]);
			vertices.push_back((float)texcoords.mData[1]);
		}
	}

	Mesh meshObject;
	meshObject.GetMaterials().push_back(meshMaterial);
	meshObject.CreateMesh(vertices, indices);
	_meshes.push_back(meshObject);
}

void FBXLoader::LoadScene(const char * path)
{
	_scene = FbxScene::Create(_manager, path);
	FbxImporter* importer = FbxImporter::Create(_manager, "");

	bool status = importer->Initialize(path, -1, _manager->GetIOSettings());
	status = importer->Import(_scene);
	importer->Destroy();

	FbxNode* root = _scene->GetRootNode();
	FbxAxisSystem SceneAxisSystem = _scene->GetGlobalSettings().GetAxisSystem();
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd,
		FbxAxisSystem::eRightHanded);
	if (SceneAxisSystem != OurAxisSystem) 
		OurAxisSystem.ConvertScene(_scene);

	FbxSystemUnit SceneSystemUnit = _scene->GetGlobalSettings().GetSystemUnit();
	if (fabs(SceneSystemUnit.GetScaleFactor() - 1.0) > 0.00001) 
	{
		FbxSystemUnit OurSystemUnit(1.0);
		OurSystemUnit.ConvertScene(_scene);
	}

	ProcessNode(root, nullptr);

}

void FBXLoader::Render()
{
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_program->Bind();
	uint32_t program_id = _program->GetProgram();

	for (auto& mesh : _meshes)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh.GetMaterials()[0]._normal_tex);
		mesh.Draw();

		std::cout << "TAMERE LE MESH" << std::endl;
	}

	glBindVertexArray(0);
}

void FBXLoader::Shutdown()
{
	_scene->Destroy();
	_manager->Destroy();
}
