#ifndef _WINDOW_H_
#define _WINDOW_H_

#define KEYBOARD_SIZE 350
#define SERVER_ENABLED
#define RENDER_MAP
#define _USE_MATH_DEFINES

#include "Main.h"
#include "shader.h"
#include "Character.h"
#include "EnvElement.h"
#include "ProjectileElement.h"
#include "ScreenElement.h"
#include "ObjElement.h"
#include "SpatialHashTable.h"
//#include "../common/constants.h"
#include "CommunicationClient.h"
#include "../common/networking/CommunicationConstants.h"
#include "../common/game/Projectile.h"
#include "gui/GUIManager.h"

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <cmath>



class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static CommunicationClient * client;
	static SpatialHashTable table;

	//objects to render
	static vector<Character*> chars;
	static vector<EnvElement*> envs;
	static unordered_map<int, ProjectileElement*> projectiles;
	static vector<ScreenElement*> selectScreenElements;
	static map<int, ObjElement*> objectiveMap; 
	static Character* clientChar;

	// Shader Program 
	static GLuint shaderProgram;
	static GLuint texShader;

	// Camera Matrices 
	// Projection matrix:
	static glm::mat4 projection;

	// View Matrix:
	static glm::vec3 eyePos, lookAtPoint, upVector;
	static glm::mat4 view;

	// Last CLIENT_INPUT recorded
	static CLIENT_INPUT lastInput;

	// Keyboard keys that are being held down or pressed
	static bool keyboard[KEYBOARD_SIZE];

	// Used to determine whether or not camera should be looking at player or select screen
	static bool gameStarted;
	static bool doneInitialRender;

	// GUI Manager (HUD)
	static GUIManager* guiManager;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	//static bool initializeObjects(char * file, char* file1, char* file2);
	static bool initializeObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	//static void updateGame(GameState);

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);

	// Used to handle GameUpdates from the CommunicationClient
	static void handleUpdates(std::vector<GameUpdate> updates);
	static void handleRoleClaim(GameUpdate update);
	static void handleUpdate(GameUpdate update);
	static void Window::handleAttack(GameUpdate update);

	// Used to set lastInput based on keyboard inputs
	static void Window::updateLastInput(); 

	// Used to connect to server
	static bool Window::connectCommClient(std::string);

	// Used to spawn objectives
	static void Window::initializeObjective(int id, ObjectiveType type, Restriction restriction, float x, float y);

	// Used to remove objectives
	static void Window::removeObj(int objectiveID);
};

#endif
