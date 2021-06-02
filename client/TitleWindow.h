#ifndef _TITLEWINDOW_H_
#define _TITLEWINDOW_H_

#define KEYBOARD_SIZE 350

#include "Main.h"
#include "gui/GUIManager.h"
#include "AudioProgram.h"

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <cmath>


class TitleWindow
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;

	// Keyboard keys that are being held down or pressed
	static bool keyboard[KEYBOARD_SIZE];
	static bool mouse[MOUSE_SIZE];

	// GUI Manager (HUD)
	static GUIManager* guiManager;
	static AudioProgram* audioProgram;

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

};

#endif
