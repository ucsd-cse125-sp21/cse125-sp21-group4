#ifndef _ENVELEMENT_H_
#define _ENVELEMENT_H_

#include "Object.h"

class EnvElement : Object {
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;

	GLuint VAO;
	GLuint VBO[2];
	GLuint EBO;

	glm::mat4 view;
	glm::mat4 projection;
	GLuint shader;

	glm::vec3 eyep;
	glm::vec3 color;

public:
	EnvElement(string fileName, glm::mat4 proj, glm::mat4 view, GLuint shader);
	void draw(glm::mat4 c = glm::mat4(1));
	void update();
	void updateView(glm::mat4, glm::vec3);
};

#endif