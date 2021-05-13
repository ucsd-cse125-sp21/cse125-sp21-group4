#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Object.h"
#include <time.h>

#define ANIMATION_INTERVAL 0.1

enum CharState {
	idle,
	moving
	//add more for other actions
};

class Character : Object {
private:
	std::vector<glm::vec3> normal;
	std::vector<glm::vec3> points;
	std::vector<glm::ivec3> triangles;
	std::vector<glm::vec2> textCoord;

	GLuint VAO;
	GLuint VBO[3];
	GLuint EBO;

	glm::mat4 model;
	glm::mat4* view;
	glm::mat4* projection;
	glm::mat4 scaleMtx;
	GLuint shader;

	CharState currState;
	CharState prevState;

	time_t currTime;
	time_t prevTime;

	bool hasTexture;
	GLuint textId;
	std::vector<GLuint> idleTex;
	std::vector<GLuint> moveTex;
	std::vector<std::vector<GLuint>*> animSequence;
	int frameIdx;
	//add more for attack and other actions

	glm::vec3 * eyep;
	glm::vec3 color;

public:
	//character specific status such as positions, stats etc
	glm::vec3 pos;
	//...

	/*
	constructor usage:
	projection p, view v, and shader s are taken cared of in Window class.
	translation trnas is the initial position where you want to place this object;
	rotAxis is the the axis you want to rotate about;
	rotRad  is the amount of rotation you want in RADIAN;
	scale is a factor you want to scale the initial object;
	color c is the initial model color; default is black
*/
	Character(string fileName, glm::mat4* proj, glm::mat4* view, glm::vec3* viewPos,
		GLuint shader, glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale,
		glm::vec3 c = glm::vec3(0.f, 0.f, 0.f), char* textFile = "");
	void draw(glm::mat4 c = glm::mat4(1));
	void update();
	void move(int); // no longer used
	void moveTo(glm::vec3);
	void updateView(glm::mat4, glm::vec3);
	void moveToGivenDelta(float deltaX, float deltaY);
	GLuint loadTexture(string texturePath);
	bool loadAnimation(CharState state, string animFile);
	bool loadAnimationAssets(string assetFolder);
};

#endif
