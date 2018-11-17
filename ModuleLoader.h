#ifndef __ModuleLoader_h__
#define __ModuleLoader_h__

#include <vector>

#include "MathGeoLib.h"

#include "Module.h"
#include "GL/glew.h"
#include "SDL.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/mesh.h>

struct aiScene;

class ModuleLoader : public Module
{
public:

	ModuleLoader();
	~ModuleLoader();

	bool Init();
	bool CleanUp();
	bool LoadModel(const char* pathFile);

private:

	void GenerateMeshes(const aiScene* scene);
	void GenerateMaterials(const aiScene* scene);

public:

	struct Mesh
	{
		unsigned vbo = 0;
		unsigned ibo = 0;
		unsigned material = 0;
		unsigned num_vertices = 0;
		unsigned num_indices = 0;
	};

	struct Material
	{
		unsigned texture0 = 0;
	};

	std::vector<Mesh>     meshes;
	std::vector<Material> materials;
	math::float4x4        transform = math::float4x4::identity;
};

#endif // __ModuleLoader_h__ 
