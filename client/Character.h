#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Object.h"
#include "../common/game/GameConstants.h"
#include <time.h>

#define IDLE_ANIMATION_INTERVAL 0.2
#define ATTACK_ANIMATION_INTERVAL 0.075
#define MOVING_ANIMATION_INTERVAL 0.1

#define CHARACTER_DAMAGE_TAKEN_FLASHING_TIME_MS 600
#define CHARACTER_DAMAGE_TAKEN_FLASHING_INTERVAL_MS 100

enum CharState {
	idle,
	moving,
	attacking
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
	Direction currDirec;

	time_t currTime;
	time_t prevTime;

	bool hasTexture;
	GLuint textId;

	std::vector<std::vector<std::vector<GLuint>*>*> animSequence;
	std::vector<std::vector<GLuint>*> idleTex;
	std::vector<std::vector<GLuint>*> moveTex;
	std::vector<std::vector<GLuint>*> attackTex;

	std::vector<GLuint> idle_west;
	std::vector<GLuint> idle_east;
	std::vector<GLuint> idle_north;
	std::vector<GLuint> idle_south;

	std::vector<GLuint> move_west;
	std::vector<GLuint> move_east;
	std::vector<GLuint> move_north;
	std::vector<GLuint> move_south;
	
	std::vector<GLuint> attack_west;
	std::vector<GLuint> attack_east;
	std::vector<GLuint> attack_north;
	std::vector<GLuint> attack_south;

	
	int frameIdx;
	//add more for attack and other actions

	glm::vec3 * eyep;
	glm::vec3 color;


	// used for taking damage
	bool isVisible;
	time_t lastDamageFlash;
	time_t damageFlashUntil;

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
	bool loadAnimation(CharState state, Direction d, string animFile);
	bool loadAnimationAssets(string assetFolder, Direction d);
	void setState(CharState state);
	void flashDamage();
	void setDirection(Direction d);
};

#endif
