#include "Window.h"

// Tile IDs
#define SPACE_ID   -1
#define OBST_ID     2
#define BEAC_ID   200

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE125_GAME";
CommunicationClient* Window::client;
bool Window::keyboard[KEYBOARD_SIZE];
bool Window::gameStarted;
bool Window::doneInitialRender;

//objects to render
vector<Character*> Window::chars; //all the characters players get to control
vector<EnvElement*> Window::envs; //all the environmental static objects
vector<ScreenElement*> Window::selectScreenElements; 
Character* Window::clientChar;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::texShader;

// projection Matrices 
glm::mat4 Window::projection;

//this is the position of the camera
glm::vec3 Window::eyePos(0, 20, 20); // x y z
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

	Window::gameStarted = false;
	Window::doneInitialRender = false;

	return true;
}

/*
initialize all the objects
which need (fileName, projection matrix, view matrix , shader id, translation vector, 
rotation axis, rotation in radian, scale factor in float, model color)
*/
bool Window::initializeObjects()
{

	//  ==========  Select Screen  ========== 
	glm::vec3 selectScreenLocation = eyePos + glm::vec3(0.f, -100.f, 0.f);
	float rotateAmount = glm::radians(-45.f);
	lookAtPoint = selectScreenLocation;
	selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/character_select_background.png"));
	

	// Each Job Buttons
	// (1) Fighter
	selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/fighter_unselected.png"));
		
	// (2) Mage
	selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/mage_unselected.png"));
		
	// (3) Cleric
	selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/cleric_unselected.png"));
		
	// (4) Rogue
	selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/rogue_unselected.png"));

	//  ==========  End of Select Screen  ========== 

	//  ==========  Environment Initialization  ========== 
	envs.push_back(new EnvElement("shaders/environment/ground.obj", &projection, &view, shaderProgram,
		glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f)));
	
	
	/*
	ifstream map_file("../assets/layout/map.csv");
    string line;
    string id;

    int i = 0, j = 0;

	int x = 0, y = 0, z = 0;
    while(getline(map_file, line)) {
        stringstream ss(line);
        
        while(getline(ss, id, ',')) {
			//std::cout << std::stoi(id) << '\n';
			// (horiz - pos right, vert - pos up, screen - pos towards you)
			//std::cout << "i: " << i << "j: " << j << '\n';
            switch(std::stoi(id)) {
				
                case OBST_ID:
					envs.push_back(new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram, 
						glm::vec3(2.*j, 1.f, 2.*i), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, .5f, .5f))); // blocks are 2 wide
					break;
                case BEAC_ID:
					envs.push_back(new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram, 
						glm::vec3(2.*j, 1.f, 2.*i), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, 1.f, 1.f))); // blocks are 2 wide
					break;
				case SPACE_ID:
                    break;

                default:
					std::cout << "Invalid id " << id << '\n';
					std::cout << "i: " << i << "j: " << j << '\n';
					return false;
			}
			
			++j;
		}
		++i;
		j = 0;
	} */


	//  ==========  End of Environment Initialization  ========== 

	//  ==========  Character Initialization   ========== 

	// Characters on the map now (scaled 3x)
	chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(5.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE"));	
	chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(15.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE"));	
	chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(5.f, 1.f, 15.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE"));	
	chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(70.f, 1.f, 70.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE"));

	#ifdef SERVER_ENABLED
	clientChar = chars[client->getId()];
	#else 
	clientChar = chars[0];
	gameStarted = true;
	#endif

	//  ==========  End of Character Initialization ========== 

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
	//update camera location
	if(Window::gameStarted) {
		lookAtPoint = clientChar->pos;
	}
	eyePos = lookAtPoint + glm::vec3(0.f, 5.f, 5.f);
	view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

	int i;
	for (i = 0; i < chars.size(); i++) {
		chars[i]->update();
	}
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//draw all the characters and environmental elements
	int i;
	for (i = 0; i < selectScreenElements.size(); i++) {
		selectScreenElements[i]->draw();
	}

	for (i = 0; i < envs.size(); i++) {
		envs[i]->draw();
	}

	for (i = 0; i < chars.size(); i++) {
		chars[i]->draw();
	}

	glfwPollEvents();
	glfwSwapBuffers(window);
	if(!doneInitialRender) {
	#ifdef SERVER_ENABLED
		// send update that we've finished rendering to the server
		client->sendInput(DONE_RENDERING);
		Sleep(TICK_TIME);
	#endif
		doneInitialRender = true;
	}
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
		printf("Update received: type: %d, id: %d, gridX: %d, gridY: %d, floatX: %f, floatY: %f\n", updates[i].updateType, updates[i].id, updates[i].gridPos.x, updates[i].gridPos.y, updates[i].floatDeltaX, updates[i].floatDeltaY);
    }
}

void Window::handleRoleClaim(GameUpdate update) {
	switch(update.roleClaimed) {
		case FIGHTER:
			selectScreenElements[1]->loadTexture("shaders/select_screen/fighter_selected.png");
			break;
		case MAGE:
			selectScreenElements[2]->loadTexture("shaders/select_screen/mage_selected.png");
			break;
		case CLERIC:
			selectScreenElements[3]->loadTexture("shaders/select_screen/cleric_selected.png");
			break;
		case ROGUE:
			selectScreenElements[4]->loadTexture("shaders/select_screen/rogue_selected.png");
			break;
	}
}

// Handles specific update on the graphics side.
void Window::handleUpdate(GameUpdate update) {
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN:
            break;
		case PLAYER_HP_INCREMENT:
			break;
        case PLAYER_MOVE:
		{
			chars[update.id]->moveToGivenDelta(update.floatDeltaX, update.floatDeltaY);
			printf("Character %d moved with deltaX: %f, deltaY: %f\n", update.id, update.floatDeltaX, update.floatDeltaY);
            break;
        
		}
		case PROJECTILE_MOVE:
            break;
		case GAME_STARTED:
			Window::gameStarted = true;
			break;
		case ROLE_CLAIMED:
			Window::handleRoleClaim(update);
            break;
        default:
            printf("Not Handled Update Type: %d\n", update.updateType);
            break;
    }
}


// This function checks if a certain key is being pressed or held down.
void Window::updateLastInput() {

	// E key
	if (keyboard[GLFW_KEY_E]) {
		lastInput = INTERACT;

	// J key
	} else if (keyboard[GLFW_KEY_J]) {
		lastInput = ATTACK;

	// W key
	} else if (keyboard[GLFW_KEY_W]) {
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

	// 1 key (claim fighter)
	} else if(keyboard[GLFW_KEY_1]) {
		lastInput = CLAIM_FIGHTER;

	// 2 key (claim mage)
	} else if(keyboard[GLFW_KEY_2]) {
		lastInput = CLAIM_MAGE;

	// 3 key (claim cleric)
	} else if(keyboard[GLFW_KEY_3]) {
		lastInput = CLAIM_CLERIC;

	// 4 key (claim rogue)
	} else if(keyboard[GLFW_KEY_4]) {
		lastInput = CLAIM_ROGUE;
		
	}
}

