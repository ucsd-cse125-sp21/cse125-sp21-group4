#include "main.h"

void error_callback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(error_callback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);

	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);

	// Set the mouse and cursor callbacks
	glfwSetMouseButtonCallback(window, Window::mouse_callback);
	glfwSetCursorPosCallback(window, Window::cursor_callback);
	glfwSetScrollCallback(window, Window::mouse_scroll_callback);
	glfwSetWindowFocusCallback(window, Window::focus_callback);
}

void setup_opengl_settings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " <<
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}


/** 
 * The simple client architecture goes as follows:
 *  1. Handle input (client side)
 *  2. Send input to server
 *  3. Receive updated gamestate from server
 *  4. update local game state
 *  5. Render world (client side)
 * 
 */
int main(int argc, char* argv[])
{

	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(800, 600);

	if (!window) exit(EXIT_FAILURE);

	// Print OpenGL and GLSL versions.
	print_versions();
	// Setup callbacks.
	setup_callbacks(window);
	// Setup OpenGL settings.
	setup_opengl_settings();

	auto start = std::chrono::steady_clock::now();

	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);

	auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count();

	printf("It took %d seconds to initialize.\n", duration);
	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window))
	{
		// Idle callback. Updating objects, etc. can be done here.
		Window::idleCallback();

		// Main render display callback. Rendering of objects is done here.
		Window::displayCallback(window);
// #ifdef SERVER_ENABLED

// 		// 1 + 2. Get the latest input and send it to the server
// 		commClient->sendInput(Window::lastInput);

// 		// 3. Receive updated gamestate from server
// 		GameState gameState = commClient->receiveGameState();
// #endif

		// Window::lastInput = NO_MOVE;
		Sleep(5);

		// 4 + 5. TODO: Update local game state and render the world.
        // printf("Player %d is on x: %d, y: %d\n", gameState.playerPositions[0].id, gameState.playerPositions[0].x, gameState.playerPositions[0].y);
        // printf("Player %d is on x: %d, y: %d\n", gameState.playerPositions[1].id, gameState.playerPositions[1].x, gameState.playerPositions[1].y);
        // printf("Player %d is on x: %d, y: %d\n", gameState.playerPositions[2].id, gameState.playerPositions[2].x, gameState.playerPositions[2].y);
        // printf("Player %d is on x: %d, y: %d\n", gameState.playerPositions[3].id, gameState.playerPositions[3].x, gameState.playerPositions[3].y);
	}

	Window::cleanUp();
	// Destroy the window.
	glfwDestroyWindow(window);
	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
