#include "TitleWindow.h"

//// TitleWindow Properties
int TitleWindow::width;
int TitleWindow::height;
const char* TitleWindow::windowTitle = "CSE125_GAME";
bool TitleWindow::keyboard[KEYBOARD_SIZE];
bool TitleWindow::mouse[MOUSE_SIZE];

// GUI manager
GUIManager* TitleWindow::guiManager;

// Audio Program
AudioProgram* TitleWindow::audioProgram;

//
bool TitleWindow::launchGame = false;



bool TitleWindow::initializeProgram() {
	return true;
}

/*
initialize all the objects
which need (fileName, projection matrix, view matrix , shader id, translation vector, 
rotation axis, rotation in radian, scale factor in float, model color)
*/
bool TitleWindow::initializeObjects(GLFWwindow* window) {
	guiManager->setConnectingScreenVisible(false);
	guiManager->setHUDVisible(false);
	guiManager->setLoadingScreenVisible(false);
	guiManager->setSplashScreenVisible(false);

	// only make the title screen visible
	guiManager->titleScreen->setVisible(true);
	return true;
}

// End of init methods

void TitleWindow::cleanUp()
{
	// delete gui manager??
}

GLFWwindow* TitleWindow::createWindow(int width, int height, AudioProgram* audioProgram)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}
	
	// Old windowed mode
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
	
	// Fullscreen mode
	// GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	// const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
 
	// glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	// glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	// glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	// glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	
	// GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, windowTitle, primaryMonitor, NULL);
	// width = mode->width;
	// height = mode->height;

	// Disables the cursor
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}
	// Make the context of the window.
	glfwMakeContextCurrent(window);
	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}

	// Initialize the GUI Manager
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	TitleWindow::guiManager = new GUIManager(width, height, fbWidth, fbHeight);


	// setup audio program
	TitleWindow::audioProgram = audioProgram;
	audioProgram->setMusicVolume(0.55);


	// Display once to show splash screen, then we can deal with connecting window.
	audioProgram->playAudioWithLooping(TITLE_MUSIC);

	// Set swap interval to 1 if you want buffer 
	glfwSwapInterval(0);

	/* ===== end of #ifndef (no-server client) code ==== */
	TitleWindow::resizeCallback(window, width, height);

	return window;
}

void TitleWindow::resizeCallback(GLFWwindow* window, int width, int height)
{
	TitleWindow::width = width;
	TitleWindow::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Fixes window crashing when resizing or minimizing
	if (height > 0 && width > 0) {

		// Update the GUIManager's window height/width
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		guiManager->resizeWindow(width, height, fbWidth, fbHeight);
	}

}

void TitleWindow::idleCallback()
{


}

void TitleWindow::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	guiManager->draw();


	glfwPollEvents();
	glfwSwapBuffers(window);
}


void TitleWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		launchGame = true;
		glfwSetWindowShouldClose(window, 1); // closes the title screen.
	}
}
void TitleWindow::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) mouse[MOUSE_LEFT_INDEX] = (action == GLFW_PRESS);
	if (button == GLFW_MOUSE_BUTTON_RIGHT)  mouse[MOUSE_RIGHT_INDEX] = (action == GLFW_PRESS);
}


void TitleWindow::cursor_callback(GLFWwindow* window, double currX, double currY) {

}
