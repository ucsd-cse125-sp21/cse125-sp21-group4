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
SpatialHashTable Window::table(5000, 8.f);
bool Window::keyboard[KEYBOARD_SIZE];
bool Window::gameStarted;
bool Window::doneInitialRender;

//objects to render
vector<Character*> Window::chars(4); //all the characters players get to control
vector<EnvElement*> Window::envs; //all the environmental static objects
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

// GUI manager
GUIManager* Window::guiManager;


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

	// CommunicationClient no longer blocks on constructor. It will block on connectTo().
	client = new CommunicationClient();

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
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/character_select_background.png"));
	

	// Each Job Buttons
	// (1) Fighter
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/fighter_unselected.png"));
		
	// (2) Mage
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/mage_unselected.png"));
		
	// (3) Cleric
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/cleric_unselected.png"));
		
	// (4) Rogue
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		selectScreenLocation, glm::vec3(1.f, 0.f, 0.f), rotateAmount, 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/rogue_unselected.png"));

	//  ==========  End of Select Screen  ========== 

	//  ==========  Environment Initialization  ========== 
	//NOTE: envs now only contain environment objects that are globally viewable. All other objects that require
	//proximity rendering should be inserted into "table"
	envs.push_back(new EnvElement("shaders/environment/ground.obj", &projection, &view, shaderProgram,
		glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f)));


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
				
			case OBST_ID: {
				EnvElement* e = new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram,
					glm::vec3(2. * j, 1.f, 2. * i), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, .5f, .5f));
				table.insert(e);
				break;
			}
			case BEAC_ID: {
				EnvElement* e = new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram,
					glm::vec3(2. * j, 1.f, 2. * i), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(1.f, 1.f, 1.f));
				table.insert(e);
				break;
			}
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
	} 


	//  ==========  End of Environment Initialization  ========== 

	//  ==========  Character Initialization   ========== 

	// Characters on the map now (scaled 3x)
	// chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
	// 	glm::vec3(5.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
	// 	"shaders/character/MAGE"));	
	// chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
	// 	glm::vec3(15.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
	// 	"shaders/character/MAGE"));	
	// chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
	// 	glm::vec3(5.f, 1.f, 15.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
	// 	"shaders/character/MAGE"));	
	// chars.push_back(new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
	// 	glm::vec3(70.f, 1.f, 70.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
	// 	"shaders/character/MAGE"));

	// #ifdef SERVER_ENABLED
	// clientChar = chars[client->getId()];

	// #else 
	// clientChar = chars[0];
	// gameStarted = true;
	// #endif

	/* ===== THIS #ifndef CODE IS ONLY FOR NON-CONNECTED CLIENTS TO IMPROVE GRAPHICS DEVELOPMENT ==== */
	#ifndef SERVER_ENABLED
	chars[0] = new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(5.f, 1.f, 5.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE");	
	clientChar = chars[0];
	Window::gameStarted = true;
	#endif
	/* ===== end of #ifndef (no-server client) code ==== */


	chars[3] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(SPAWN_POSITIONS[3][0], 1.f, SPAWN_POSITIONS[3][1]), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE"));

	//  ==========  End of Character Initialization ========== 

	return true;
}

// End of Nano GUI Methods

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

	// Initialize the GUI Manager
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	Window::guiManager = new GUIManager(width, height, fbWidth, fbHeight);
	guiManager->setConnectingScreenVisible(true);

	/* ===== THIS #ifndef CODE IS ONLY FOR NON-CONNECTED CLIENTS TO IMPROVE GRAPHICS DEVELOPMENT ==== */
#ifndef SERVER_ENABLED // Client-only (no server)	
	guiManager->setConnectingScreenVisible(false);
	guiManager->setHUDVisible(true);
	guiManager->beaconBar->setAmount(18.f);
#endif
	/* ===== end of #ifndef (no-server client) code ==== */

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



	// Update the GUIManager's window height/width
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	guiManager->resizeWindow(width, height, fbWidth, fbHeight);

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
	if(client->isConnected()) {
		// 1 + 2. Get the latest input and send it to the server
		client->sendInput(Window::lastInput);

		// 3. Receive updated gamestate from server
		std::vector<GameUpdate> updates = client->receiveGameUpdates();

		// clear out previous projectiles
		Window::projectiles.clear();
		// cout << "updating game" << endl;
		Window::handleUpdates(updates);
	}
	lastInput = NO_MOVE;

#endif
	//update camera location
	if(Window::gameStarted && clientChar != nullptr) {
		lookAtPoint = clientChar->pos;
	}
	eyePos = lookAtPoint + glm::vec3(0.f, 5.f, 5.f);
	view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

	int i;
	for (i = 0; i < chars.size() && Window::gameStarted; i++) {
		if (chars[i] != nullptr) {
			chars[i]->update();
		}
	}

}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//draw all the characters and environmental elements
	int i;

	//first draw all globally viewable objects
	for (i = 0; i < envs.size(); i++) {
		envs[i]->draw();
	}

	//then selectively draw objects nearby this player
	vector<EnvElement*> result;
	float h = 8.0f;
	int j;
	glm::vec3 base1(-1.f * h, 0.f, 0.f);
	for (j = 0; j < 3 && Window::gameStarted; j++) {
		int k;
		glm::vec3 base2(0.f, 0.f, -1.f * h);
		for (k = 0; k < 3; k++) {
			glm::vec3 loc = base1 + base2;
			if(clientChar != nullptr) {
				loc += clientChar->pos;
			}
			Cell* cell = table.getCell(loc);
			if (cell != NULL && cell->items.size() != 0)
				result.insert(result.end(), cell->items.begin(), cell->items.end());
			base2 += glm::vec3(0.f, 0.f, 1.f * h);
		}
		base1 += glm::vec3(1.f * h, 0.f, 0.f);
	}
	for (i = 0; i < result.size(); i++) {
		result[i]->draw();
	}

	for (i = 0; i < chars.size() && Window::gameStarted; i++) {
		if (chars[i] != nullptr) {
			chars[i]->draw();
		}
	}

	Window::guiManager->draw();


	glfwPollEvents();
	glfwSwapBuffers(window);
	if(!doneInitialRender && client->isConnected()) {
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
		if (!guiManager->connectingScreen->hasConnectedToServer) {
			guiManager->connectingScreen->handleKeyInput(key, window);
		}

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
	
	guiManager->selectScreen->handleRoleClaimed(update.roleClaimed);

	switch(update.roleClaimed) {
		case FIGHTER:
			chars[update.id] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
				glm::vec3(SPAWN_POSITIONS[update.id][0], 1.f, SPAWN_POSITIONS[update.id][1]), 
				glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/FIGHTER"));	
			chars[update.id]->loadAnimationAssets("shaders/character/FIGHTER");
			chars[update.id]->loadAnimationAssets("shaders/character/FIGHTER/ATTACK");
			break;
		case MAGE:
			chars[update.id] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
				glm::vec3(SPAWN_POSITIONS[update.id][0], 1.f, SPAWN_POSITIONS[update.id][1]), 
				glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/MAGE"));	
			chars[update.id]->loadAnimationAssets("shaders/character/MAGE");
			chars[update.id]->loadAnimationAssets("shaders/character/MAGE/ATTACK");
			break;
		case CLERIC:
			chars[update.id] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
				glm::vec3(SPAWN_POSITIONS[update.id][0], 1.f, SPAWN_POSITIONS[update.id][1]), 
				glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/CLERIC"));	
			chars[update.id]->loadAnimationAssets("shaders/character/CLERIC");
			chars[update.id]->loadAnimationAssets("shaders/character/CLERIC/ATTACK");
			break;
		case ROGUE:
			chars[update.id] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
				glm::vec3(SPAWN_POSITIONS[update.id][0], 1.f, SPAWN_POSITIONS[update.id][1]), 
				glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/ROGUE"));
			chars[update.id]->loadAnimationAssets("shaders/character/ROGUE");
			chars[update.id]->loadAnimationAssets("shaders/character/ROGUE/ATTACK");
			break;
	}

	// Do not need to check if client is connected because this method should only run when the client is connected to the server.
	#ifdef SERVER_ENABLED
	if(update.id == client->getId()) clientChar = chars[client->getId()];
	#endif

	// Initializes the hp bar to the given player's role
	// Also sets the player's role and id for the minimap
	if(update.id == client->getId()) {
		guiManager->healthBar->initGivenPlayerType(update.roleClaimed);
		guiManager->miniMap->setCurrentPlayer(client->getId(), update.roleClaimed);
		guiManager->selectScreen->hasClaimed = true;
	} else {
		guiManager->miniMap->setPlayerType(update.id, update.roleClaimed);
	}
}

// Handles specific update on the graphics side.
void Window::handleUpdate(GameUpdate update) {
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN:
			if(update.id == client->getId()) {
				guiManager->healthBar->decrementHp(update.damageTaken);
			}
            break;
		case PLAYER_HP_INCREMENT:
			if(update.id == client->getId()) {
				guiManager->healthBar->incrementHp(update.healAmount);
			}
			break;
        case PLAYER_MOVE:
		{
			chars[update.id]->moveToGivenDelta(update.floatDeltaX, update.floatDeltaY);
			guiManager->miniMap->updatePlayerPositionDelta(update.id, update.floatDeltaX, update.floatDeltaY);
			printf("Character %d moved with deltaX: %f, deltaY: %f\n", update.id, update.floatDeltaX, update.floatDeltaY);
            break;
        
		}
		case PROJECTILE_MOVE:
		{
			Projectile p;
			p.type = update.projectileType;
			p.currentPosition.x = update.playerPos.x;
			p.currentPosition.y = update.playerPos.y;
			p.direction = update.direction;

			glm::vec3 trans = glm::vec3(p.currentPosition.x, 1.f, p.currentPosition.y);
			glm::vec3 rotAxis = glm::vec3(0.f, 1.f, 0.f);
			float rotRad = glm::radians(0.f);
			float scale = 1.f;
			glm::vec3 color = glm::vec3(1.f, .5f, .5f);
			char* textFile = "shaders/projectile/arrow_up.png";

			ProjectileElement* pEle = new ProjectileElement("shaders/character/billboard.obj", &projection, &view, shaderProgram,
															trans, rotAxis, rotRad, scale, color, textFile);
			projectiles.push_back(pEle);

            break;
		}
		case GAME_STARTED:
			Window::gameStarted = true;
			guiManager->setSelectScreenVisible(false); // disable the selects creen
			guiManager->setHUDVisible(true); // sets the hud visible
			break;
		case ROLE_CLAIMED:
			Window::handleRoleClaim(update);
            break;
		case PLAYER_ATTACK:
			printf("Calling handleAttack()\n");
			Window::handleAttack(update);
			break;
		// Beacon bar updates
		case BEACON_BEING_TAKEN:
		case BEACON_DECAYING:
			guiManager->beaconBar->setAmount(update.beaconCaptureAmount);
			break;
		case BEACON_CAPTURED:
			guiManager->beaconBar->setAmount(update.beaconCaptureAmount);
			guiManager->miniMap->handleCaptureEvent(update.beaconCaptureAmount);
			break;
		case BEACON_PING_PLAYER:
			guiManager->miniMap->updatePingPosition(update.id, update.playerPos);
			break;
		case ALL_PLAYERS_JOINED:
			guiManager->setSelectScreenVisible(true);
			guiManager->selectScreen->startTimer(update.selectTimerStartTime);
			guiManager->setConnectingScreenVisible(false);
			break;
        default:
            printf("Not Handled Update Type: %d\n", update.updateType);
            break;
    }
}

void Window::handleAttack(GameUpdate update) {
	printf("Player %id is attacking\n", update.id);
	chars[update.id]->setState(attacking);
}


// This function checks if a certain key is being pressed or held down.
void Window::updateLastInput() {

	// E key
	if (keyboard[GLFW_KEY_E]) {
		lastInput = INTERACT;

	// J key
	} else if (keyboard[GLFW_KEY_J]) {
		lastInput = ATTACK;

	// K key
	} else if (keyboard[GLFW_KEY_K]) {
		lastInput = UNIQUE_ATTACK;

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

	// 1 key (select fighter)
	} else if(keyboard[GLFW_KEY_1]) {
		if (guiManager->connectingScreen->hasConnectedToServer) {
			guiManager->selectScreen->handleSelecting(FIGHTER);
		}
	// 2 key (select mage)
	} else if(keyboard[GLFW_KEY_2]) {
		
		if (guiManager->connectingScreen->hasConnectedToServer) {
			guiManager->selectScreen->handleSelecting(MAGE);
		}

	// 3 key (select cleric)
	} else if(keyboard[GLFW_KEY_3]) {
		
		if (guiManager->connectingScreen->hasConnectedToServer) {
			guiManager->selectScreen->handleSelecting(CLERIC);
		}

	// 4 key (select rogue)
	} else if(keyboard[GLFW_KEY_4]) {
		
		if (guiManager->connectingScreen->hasConnectedToServer) {
			guiManager->selectScreen->handleSelecting(ROGUE);
		}
		
	// enter key (claim selected role)
	} else if (keyboard[GLFW_KEY_ENTER]) {
		switch(guiManager->selectScreen->selecting) {
			case FIGHTER:
				lastInput = CLAIM_FIGHTER;
				break;
			case MAGE:
				lastInput = CLAIM_MAGE;
				break;
			case CLERIC:
				lastInput = CLAIM_CLERIC;
				break;
			case ROGUE:
				lastInput = CLAIM_ROGUE;
				break;
		}
	}

	/* ===== THIS #ifndef CODE IS ONLY FOR NON-CONNECTED CLIENTS TO IMPROVE GRAPHICS DEVELOPMENT ==== */
	#ifndef SERVER_ENABLED
	if (keyboard[GLFW_KEY_W]) {
		chars[0]->moveToGivenDelta(0, -INIT_SPEED);
        
	// A key
	} else if(keyboard[GLFW_KEY_A]) {
		chars[0]->moveToGivenDelta(-INIT_SPEED, 0);

	// S key
	} else if(keyboard[GLFW_KEY_S]) {
		chars[0]->moveToGivenDelta(0, INIT_SPEED);
		
	// D key
	} else if(keyboard[GLFW_KEY_D]) {
		chars[0]->moveToGivenDelta(INIT_SPEED, 0);
	}
	#endif
	/* ===== end of #ifndef (no-server client) code ==== */
}

bool Window::connectCommClient(std::string serverIP) {
	
	#ifdef SERVER_ENABLED
	
	if(!client->connectTo(serverIP)) {
		return false;
	}

	cout << "Communication Established" << endl;
	
	// if the id is 3, then set the hpBar to be max monster
	if(client->getId() == 3) {
		guiManager->healthBar->initGivenPlayerType(MONSTER);
		guiManager->selectScreen->setMonster(true);
	}
	guiManager->miniMap->setCurrentPlayer(3, MONSTER); // Monster is currently id = 3
	#endif // SERVER_ENABLED
	
	#ifdef SERVER_ENABLED

	if(client->getId() == 3) clientChar = chars[3];
	
	#else 
	clientChar = chars[0];
	gameStarted = true;
	#endif

	return true;
}