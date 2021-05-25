#ifndef _GROUNDTILE_H_
#define _GROUNDTILE_H_

#include "Object.h"

class GroundTile : public Object{
private:
	std::vector<glm::vec3> * normal;
	std::vector<glm::vec3> * points;
	std::vector<glm::ivec3> * triangles;
	std::vector<glm::vec2> * textCoord;
	int triangleSize;

	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;

	glm::mat4 model;
	glm::mat4* view;
	glm::mat4* projection;
	GLuint shader;

	GLuint textId;

	glm::vec3 eyep;

public:
	//glm::vec3 pos;
	GroundTile(glm::mat4* proj, glm::mat4* view, GLuint shader,
		glm::vec3 trans, float scale, GLuint tid, vector<glm::vec3> * n, vector<glm::vec3> * p,
		vector<glm::ivec3> * tri, vector<glm::vec2> * tc);
	void draw(glm::mat4 c = glm::mat4(1));
	void drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c = glm::mat4(1));
	void update();
	void updateView(glm::mat4, glm::vec3);

};

#endif