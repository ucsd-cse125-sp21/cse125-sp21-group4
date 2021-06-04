#ifndef _WINDOW_H_
#define _WINDOW_H_

#define KEYBOARD_SIZE 350
#define MOUSE_SIZE 2
#define MOUSE_LEFT_INDEX 0
#define MOUSE_RIGHT_INDEX 1
#define SERVER_ENABLED
#define RENDER_MAP
#define RENDER_CHARACTERS
#define _USE_MATH_DEFINES
#define SPATIAL_HASH_SEARCH_DISTANCE 20.0


// camera offsets for player
#define MIN_CAMERA_X_OFFSET 0.f
#define MIN_CAMERA_Y_OFFSET 10.f
#define MIN_CAMERA_Z_OFFSET 6.f
#define MAX_CAMERA_X_OFFSET 0.f
#define MAX_CAMERA_Y_OFFSET 20.f
#define MAX_CAMERA_Z_OFFSET 16.f


#include "Main.h"
#include "shader.h"
#include "Character.h"
#include "Ground.h"
#include "EnvElement.h"
#include "ProjectileElement.h"
#include "ScreenElement.h"
#include "ObjElement.h"
#include "Cursor.h"
#include "SpatialHashTable.h"
//#include "../common/constants.h"
#include "CommunicationClient.h"
#include "../common/networking/CommunicationConstants.h"
#include "../common/game/Projectile.h"
#include "gui/GUIManager.h"
#include "AudioProgram.h"
#include "MaterialManager.h"

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <chrono>

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
	static unordered_map<PlayerType, Character*> Window::playerTypeToCharacterMap;

	static vector<EnvElement*> envs;
	static Ground * ground;
	static unordered_map<int, ProjectileElement*> projectiles;
	static vector<ScreenElement*> selectScreenElements;
	static map<int, ObjElement*> objectiveMap; 
	static Character* clientChar;

	static Cursor* cursor;
	static float cursorOffsetX;
	static float cursorOffsetY;

	// Shader Program 
	static GLuint shaderProgram;
	static GLuint phongTexShader;
	static GLuint texShader;
	static GLuint groundShader;
	static GLuint satTextureShader;
	static GLuint phongSaturatedTexShader;

	// Audio Program
	static AudioProgram* audioProgram;
	static std::vector<PlayerType> playerJobs;

	// Material Manager
	static MaterialManager materialManager;


	// Camera Matrices 
	// Projection matrix:
	static glm::mat4 projection;

	// View Matrix:
	static glm::vec3 eyePos, lookAtPoint, upVector;
	static glm::mat4 view;
	static glm::vec3 cameraOffset;

	// Last CLIENT_INPUT recorded
	static CLIENT_INPUT lastInput;
	static float lastAngle; // used for projectile angle

	// Keyboard keys that are being held down or pressed
	static bool keyboard[KEYBOARD_SIZE];
	static bool mouse[MOUSE_SIZE];

	// Used to determine whether or not camera should be looking at player or select screen
	static bool gameStarted;
	static bool doneInitialRender;
	static bool gameEnded;
	static steady_clock::time_point endTime;

	// GUI Manager (HUD)
	static GUIManager* guiManager;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	//static bool initializeObjects(char * file, char* file1, char* file2);
	static bool initializeObjects(GLFWwindow*);
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height, AudioProgram* audioProgram);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	//static void updateGame(GameState);

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
	static void focus_callback(GLFWwindow* window, int focused);

	// Used to handle GameUpdates from the CommunicationClient
	static void handleUpdates(std::vector<GameUpdate> updates);
	static void handleRoleClaim(GameUpdate update);
	static void handleUpdate(GameUpdate update);
	static void Window::handleAttack(GameUpdate update);
	static void Window::handleUniqueAttack(GameUpdate update);

	// Used to set lastInput based on keyboard inputs
	static void Window::updateLastInput(); 

	// Used to connect to server
	static bool Window::connectCommClient(std::string);

	// Used to spawn objectives
	static void Window::initializeObjective(int id, ObjectiveType type, Restriction restriction, float x, float y);

	// Used to remove objectives
	static void Window::removeObj(int objectiveID);

	// Used to determine which objective should tell the user to do something
	static void Window::checkNearObjectiveText(ObjElement*);

	static void Window::initCharacters(GLFWwindow*);
	static void Window::initMap(GLFWwindow *);
	static void Window::initSelectScreenElements();

	static void Window::endGame();

	static void Window::handleSpectateRequest(GameUpdate update);

	static void shuffleLoadingScreen(GLFWwindow *);
	static void checkNearDeadHunter();

};

#endif
