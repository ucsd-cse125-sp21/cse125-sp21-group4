#ifndef _GROUNDTILE_H_
#define _GROUNDTILE_H_

#include "Object.h"

class GroundTile : public Object{
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;
	std::vector<glm::vec2> textCoord;
	std::vector<glm::vec3> * translations;
	int triangleSize;
	int instanceCount;

	GLuint VAO;
	GLuint VBO[4];
	GLuint EBO;

	glm::mat4* view;
	glm::mat4* projection;
	GLuint shader;
	glm::mat4 model;

	GLuint textId;

	glm::vec3 eyep;

public:
	GroundTile(string fileName, char* textFile, glm::mat4* proj, glm::mat4* v,
	GLuint s, float scale, vector<glm::vec3> * transArr);
	void draw(glm::mat4 c = glm::mat4(1));
	void drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c = glm::mat4(1));
	void update();
	void updateView(glm::mat4, glm::vec3);
	bool loadTexture(char* texturePath);
};

#endif