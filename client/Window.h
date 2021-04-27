#ifndef _WINDOW_H_
#define _WINDOW_H_

#define KEYBOARD_SIZE 350
//#define SERVER_ENABLED

#include "Main.h"
#include "shader.h"
#include "Character.h"
#include "EnvElement.h"
//#include "../common/constants.h"
#include "CommunicationClient.h"
#include "../common/networking/CommunicationConstants.h"

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
	static CommunicationClient * client;

	//objects to render
	static vector<Character*> chars;
	static vector<EnvElement*> envs;

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
	static void handleUpdate(GameUpdate update);

	// Used to set lastInput based on keyboard inputs
	static void Window::updateLastInput(); 
};

#endif
