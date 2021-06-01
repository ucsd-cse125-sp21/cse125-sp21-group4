#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "Object.h"
#include "../common/game/GameConstants.h"
#include <time.h>

#define IDLE_ANIMATION_INTERVAL 0.2
#define ATTACK_ANIMATION_INTERVAL 0.075
#define UNIQUE_ATTACK_ANIMATION_INTERVAL 0.075
#define MOVING_ANIMATION_INTERVAL 0.1

#define CHARACTER_DAMAGE_TAKEN_FLASHING_TIME_MS 600
#define CHARACTER_DAMAGE_TAKEN_FLASHING_INTERVAL_MS 100

enum CharState {
	idle,
	moving,
	attacking,
	uniqueAttacking,
	spectating
	//add more for other actions
};

class Character : public Object {
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
	Direction currDirec; // current direction t

	time_t currTime;
	time_t prevTime;

	bool hasTexture;
	GLuint textId;

	std::vector<std::vector<std::vector<GLuint>*>*> animSequence; // hold animation sequences
	std::vector<std::vector<GLuint>*> idleTex;					  // idle animation sequences
	std::vector<std::vector<GLuint>*> moveTex;					  // movement animation sequences
	std::vector<std::vector<GLuint>*> attackTex;				  // attack animation sequences
	std::vector<std::vector<GLuint>*> uniqueAttackTex;				  // attack animation sequences

	std::vector<GLuint> idle_west;		// idle west animation
	std::vector<GLuint> idle_east;		// idle east animation
	std::vector<GLuint> idle_north;		// idle north animation
	std::vector<GLuint> idle_south;		// idle south animation

	std::vector<GLuint> move_west;		// move west animation
	std::vector<GLuint> move_east;		// move east animation
	std::vector<GLuint> move_north;		// move north animation
	std::vector<GLuint> move_south;		// move south animation
	
	std::vector<GLuint> attack_west;	// attack west animation
	std::vector<GLuint> attack_east;	// attack east animation
	std::vector<GLuint> attack_north;	// attack north animation
	std::vector<GLuint> attack_south;	// attack south animation
	
	std::vector<GLuint> unique_attack_west;	// unique attack west animation
	std::vector<GLuint> unique_attack_east;	// unique attack east animation
	std::vector<GLuint> unique_attack_north;	// unique attack north animation
	std::vector<GLuint> unique_attack_south;	// unique attack south animation

	
	int frameIdx;	// current frame in animation sequence
	int viewingSpecID;  // ID of the player this player is spectating
	//add more for attack and other actions
	float saturation;
	float redSaturation;
	float greenSaturation;
	float transparentAlpha;

	glm::vec3 * eyep;
	glm::vec3 color;


	// used for taking damage
	bool isVisible;
	time_t lastDamageFlash;
	time_t damageFlashUntil;

public:
	//character specific status such as positions, stats etc

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
	void drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c = glm::mat4(1));
	void update();
	void move(int); // no longer used
	void moveTo(glm::vec3);
	void updateView(glm::mat4, glm::vec3);
	void moveToGivenDelta(float deltaX, float deltaY);
	GLuint loadTexture(string texturePath);
	bool loadAnimation(CharState state, Direction d, string animFile);
	bool loadAnimationAssets(string assetFolder);
	void setState(CharState state);
	CharState getState();
	void flashDamage();
	void setDirection(Direction d);
	int getViewingSpecID();
	void setViewingSpecID(int id);
	void setSaturationLevel(float level);
	void setRedSaturationLevel(float redSat);
	void setGreenSaturationLevel(float greenSat);
	void setTransparentAlpha(float transAlpha);
};

#endif
