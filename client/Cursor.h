#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "Object.h"

class Cursor : public Object {
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;
	std::vector<glm::vec2> textCoord;
	// std::vector<Mesh> meshes;

	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;

	glm::mat4 model;
	glm::mat4 * view;
	glm::mat4 * projection;
	glm::mat4 scaleMtx;
	GLuint shader;

	bool hasTexture;
	GLuint textId;

	glm::vec3 *eyep;
	glm::vec3 color;


public:

	/*
	constructor usage:
	projection p, view v, and shader s are taken cared of in Window class.
	translation trnas is the initial position where you want to place this object;
	rotAxis is the the axis you want to rotate about;
	rotRad  is the amount of rotation you want in RADIAN;
	scale is a factor you want to scale the initial object;
	color c is the initial model color; default is black
*/
	Cursor(string fileName, glm::mat4 * proj, glm::mat4 * view, GLuint shader, glm::vec3* eyePos,
		glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale,
		glm::vec3 c = glm::vec3(0.f, 0.f, 0.f), char * textFile = "");
	void draw(glm::mat4 c = glm::mat4(1));
	void drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c = glm::mat4(1));
	void update();
	void updateView(glm::mat4, glm::vec3);
	bool loadTexture(char* texturePath);
	void setScale(float);
};

#endif