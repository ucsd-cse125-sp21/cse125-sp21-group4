#include "Window.h"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE125_GAME";
CommunicationClient* Window::client;
bool Window::keyboard[KEYBOARD_SIZE];


//objects to render
vector<Character*> Window::chars; //all the characters players get to control
vector<EnvElement*> Window::envs; //all the environmental static objects

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::texShader;

// projection Matrices 
glm::mat4 Window::projection;

//this is the position of the camera
glm::vec3 Window::eyePos(0, 10, 10); // x y z
// this is the direction where the camera is staring at
glm::vec3 Window::lookAtPoint(0, 0, 0);
// this is the upward direction for the camera. Think of this as the angle where your head is
//tilted
glm::vec3 Window::upVector(0, 1, 0);
//view matrix with all the above stuff combined
glm::mat4 Window::view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

// last input from the window
CLIENT_INPUT Window::lastInput = NO_MOVE;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader/shader.vert", "shaders/shader/shader.frag");
	texShader = LoadShaders("shaders/shader/texture.vert", "shaders/shader/texture.frag");
	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

#ifdef SERVER_ENABLED
	client = new CommunicationClient();
	cout << "Communication Established" << endl;
#endif // SERVER_ENABLED

	// Setup the keyboard.
	for(int i = 0; i < KEYBOARD_SIZE; i++) {
		keyboard[i] = false;
	}

	return true;
}

/*
initialize all the objects
which need (fileName, projection matrix, view matrix , shader id, translation vector, 
rotation axis, rotation in radian, scale factor in float, model color)
*/
bool Window::initializeObjects()
{
	//chars
	/*chars.push_back(new Character("shaders/character/cube.obj", &projection, &view, shaderProgram, 
		glm::vec3(-5.f, 1.f, -5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, .5f, .5f)));
	chars.push_back(new Character("shaders/character/cube.obj", &projection, &view, shaderProgram,
		glm::vec3(5.f, 1.f, -5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(45.f), 1.f, glm::vec3(.5f, 1.f, .5f)));
	chars.push_back(new Character("shaders/character/cube.obj", &projection, &view, shaderProgram,
		glm::vec3(-5.f, 1.f, 5.f), glm::vec3(0.f, 0.f, 1.f), glm::radians(45.f), 1.6f, glm::vec3(.5f, .5f, 1.f)));
	chars.push_back(new Character("shaders/character/cube.obj", &projection, &view, shaderProgram,
		glm::vec3(5.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(60.f), 0.5f, glm::vec3(1.f, .3f, 1.f)));
	//env
	envs.push_back(new EnvElement("shaders/environment/ground.obj", &projection, &view, shaderProgram, 
	glm::vec3(0.f,0.f,0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f)));*/

	chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, texShader,
		glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/square2.png"));
	envs.push_back(new EnvElement("shaders/environment/ground.obj", &projection, &view, shaderProgram,
		glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f)));
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

	Window::updateLastInput();
#ifdef SERVER_ENABLED
	// 1 + 2. Get the latest input and send it to the server
	client->sendInput(Window::lastInput);
	// 3. Receive updated gamestate from server
	std::vector<GameUpdate> updates = client->receiveGameUpdates();
	lastInput = NO_MOVE;

	// cout << "updating game" << endl;
	Window::handleUpdates(updates);
#endif
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
		keyboard[key] = true;

	 // Check for a key release.
	} else if (action == GLFW_RELEASE) {
		keyboard[key] = false;
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


/* =========================================================================
   methods used to process updates in the Graphics Client.
========================================================================= */

// handleUpdates gets the updates from the ClientCommunication and performs handleUpdate on each one
void Window::handleUpdates(std::vector<GameUpdate> updates) {
    int numOfUpdates = updates.size();
    if(numOfUpdates <= 0) {
        return;
    }

    for(int i = 0; i < numOfUpdates; i++) {
        Window::handleUpdate(updates[i]);
		printf("Update received: type: %d, id: %d, gridX: %d, gridY: %d, floatX: %f, floatY: %f", updates[i].updateType, updates[i].id, updates[i].gridPos.x, updates[i].gridPos.y, updates[i].floatDeltaX, updates[i].floatDeltaY);
    }
}

// Handles specific update on the graphics side.
void Window::handleUpdate(GameUpdate update) {
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN:
            break;
        case PLAYER_MOVE:
		{
			chars[update.id]->moveToGivenDelta(update.floatDeltaX, update.floatDeltaY);
			printf("Character %d moved with deltaX: %f, deltaY: %f", update.id, update.floatDeltaX, update.floatDeltaY);
            break;
        
		}
		case PROJECTILE_MOVE:
            break;
        case OBJECTIVE_BEING_TAKEN:
            break;
        case OBJECTIVE_TAKEN:
            break;
        default:
            printf("Not Handled Update Type: %d", update.updateType);
            break;
    }
}


// This function checks if a certain key is being pressed or held down.
void Window::updateLastInput() {

	// W key
	if(keyboard[GLFW_KEY_W]) {
		lastInput = MOVE_FORWARD;

	// A key
	} else if(keyboard[GLFW_KEY_A]) {
		lastInput = MOVE_LEFT;

	// S key
	} else if(keyboard[GLFW_KEY_S]) {
		lastInput = MOVE_BACKWARD;
		
	// D key
	} else if(keyboard[GLFW_KEY_D]) {
		lastInput = MOVE_RIGHT;
	}
}