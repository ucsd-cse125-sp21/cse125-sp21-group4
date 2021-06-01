#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Core.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Our own Material Struct.
typedef struct {
  std::string name;

  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 transmittance;
  glm::vec3 emission;
  GLfloat shininess;
  GLfloat ior;       // index of refraction
  GLfloat dissolve;  // 1 == opaque; 0 == fully transparent
  // illumination model (see http://www.fileformat.info/format/material/)
  int illum;

  std::string ambient_texname;             // map_Ka
} Material;

class MaterialManager {
private:
	std::vector<std::map<std::string, Material> *> * materialLists;
	std::map<std::string, int> * materialListMap;
	
public:
	MaterialManager();

	int loadMaterial(std::string fileName);
	std::map<std::string, Material> *  getMaterialList(int);
};

#endif