#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE125_GAME";

//objects to render
vector<Character*> Window::chars; //all the characters players get to control
vector<EnvElement*> Window::envs; //all the environmental static objects

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// projection Matrices 
glm::mat4 Window::projection;

//this is the position of the camera
glm::vec3 Window::eyePos(0, 5, 5);
// this is the direction where the camera is staring at
glm::vec3 Window::lookAtPoint(0, 0, 0);
// this is the upward direction for the camera. Think of this as the angle where your head is
//tilted
glm::vec3 Window::upVector(0, 1, 0);
//view matrix with all the above stuff combined
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);


bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader/shader.vert", "shaders/shader/shader.frag");
	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
	return true;
}

//bool Window::initializeObjects(char * file, char * file1, char* file2)
bool Window::initializeObjects()
{
	chars.push_back(new Character("shaders/character/cube.obj", projection, view, shaderProgram));
	envs.push_back(new EnvElement("shaders/environment/ground.obj", projection, view, shaderProgram));
	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.

	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
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
	// Set swap interval to 1 if you want buffer 
	glfwSwapInterval(0);

	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(80.0), double(width) / (double)height, 0.5, 1000.0);
	// update projection matrix for all the objects
	int i;
	for (i = 0; i < chars.size(); i++) {
		
	}
}

void Window::idleCallback()
{
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw all the characters and environmental elements
	int i;
	for (i = 0; i < chars.size(); i++) {
		chars[i]->draw();
	}

	for (i = 0; i < envs.size(); i++) {
		envs[i]->draw();
	}

	glfwPollEvents();
	glfwSwapBuffers(window);
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case(GLFW_KEY_W):
			cout << "pressed W" << endl;
		default:
			break;
		}

	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {

	}
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	if (LeftDown) {
		return;
	}
	if (RightDown) {
		return;
	}
}
