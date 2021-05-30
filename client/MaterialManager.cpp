#include "MaterialManager.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

using namespace tinyobj;

MaterialManager::MaterialManager() {
	materialLists = new std::vector<std::map<std::string, Material> *>();
	materialListMap = new std::map<std::string, int>();
}

// Conver the material to something we actually understand
void convertMaterial(material_t& their_material, Material& our_material) {

	// Material name
	our_material.name = their_material.name;

	for (int i = 0; i < 3; i++) {
		// Ka Ambient
		our_material.ambient[i] = their_material.ambient[i];

		// Kd Diffuse
		our_material.diffuse[i] = their_material.diffuse[i];

		// Ks Specular
		our_material.specular[i] = their_material.specular[i];

		// Ke Emission
		our_material.emission[i] = their_material.emission[i];
	}
	// Ns Shininess
	our_material.shininess = their_material.shininess;

	// Ni Index of Refraction
	our_material.ior = their_material.ior;
	
	// d Dissolve
	our_material.dissolve = their_material.dissolve;

	// illum Illum Model
	our_material.illum = their_material.illum;

	// Ambient Texture map
	our_material.ambient_texname = their_material.ambient_texname;
}

int MaterialManager::loadMaterial(std::string fileName) {

	// If it already exists, just return the handle.
	if(materialListMap->find(fileName) != materialListMap->end()) {
		return materialListMap->at(fileName);
	}

	// Using tinyobjloader, load the materials
	std::vector<material_t>  t_materials;
	std::map<std::string, int> t_materialMap;
	std::string err;

	// Code is taken from tinyobjloader.h
	std::ifstream matIStream(fileName.c_str());
	if (!matIStream) {
		std::stringstream ss;
		ss << "WARN: Material file [ " << fileName << " ] not found." << std::endl;
		err += ss.str();
		printf("Error: %s\n", err.c_str());
		return -1;
	}

	std::string warning;
	LoadMtl(&t_materialMap, &t_materials, &matIStream, &warning);

	if (!warning.empty()) {
		err += warning;
	}

	// printf("Warning string: %s\n", err.c_str());


	// Parse the materials and convert it to something we understand
	std::map<std::string, Material> * materialList = new std::map<std::string, Material>();
	for(int i = 0; i < t_materials.size(); i++) {
		Material material;
		convertMaterial(t_materials[i], material);
		printf("[File: %s] Material Loaded: %s\n", fileName.c_str(), material.name.c_str());
		// printf("Ka: %f\n", material.ambient[0]);

		// insert the material (within mtl) into this list.
		materialList->insert(std::pair<std::string, Material>(material.name, material));
	}

	// Put the list onto the vector and return the index as a handle for this list of materials
	materialLists->push_back(materialList);
	int handleIndex = materialLists->size() - 1;

	// This mtl file must be inserted into the map so we don't have to parse it again.
	materialListMap->insert(std::pair<std::string, int>(fileName, handleIndex));
	return handleIndex; // returns index handle to the material list for an object
}

// Obtains the mtl list for a single mtl file given the handle to that file from loadMaterial.
std::map<std::string, Material> * MaterialManager::getMaterialList(int handle) {
	if (handle >= 0 && handle < materialLists->size()) {
		return materialLists->at(handle);
	} else {
		return nullptr;
	}
}