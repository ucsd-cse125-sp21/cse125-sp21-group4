#include "Window.h"

//// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE125_GAME";
CommunicationClient* Window::client;
//spatial hash table for rendering near by objects; starting with 5000 cells; could use few I think
//based on later performance.
SpatialHashTable Window::table(5000, SPATIAL_HASH_SEARCH_DISTANCE);
bool Window::keyboard[KEYBOARD_SIZE];
bool Window::mouse[MOUSE_SIZE];
bool Window::gameStarted;
bool Window::doneInitialRender;
bool Window::gameEnded;

//objects to render
vector<Character*> Window::chars(4); //all the characters players get to control
unordered_map<PlayerType, Character*> Window::playerTypeToCharacterMap;
vector<EnvElement*> Window::envs; //all the environmental static objects
//factory object for ground tiles; this object is responsible for creating all the individual GroundTile objects,
//all the texture, vertice and normal data are shared between the GroundTiles
Ground* Window::ground;
map<int, ObjElement*> Window::objectiveMap;
unordered_map<int, ProjectileElement*> Window::projectiles; //all the environmental static objects
Character* Window::clientChar;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram; //Phong lighting shader; only use this for models without texture
GLuint Window::texShader;     //shader for model with textures. NOTE, it also calculates texture alphas

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
float Window::lastAngle = 0;

// GUI manager
GUIManager* Window::guiManager;

// Audio Program
AudioProgram* Window::audioProgram;
vector<PlayerType> Window::playerJobs {UNKNOWN, UNKNOWN, UNKNOWN, MONSTER};

std::chrono::steady_clock::time_point lastCombatMusicPlayTime;

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

	// Setup the keyboard.
	for(int i = 0; i < KEYBOARD_SIZE; i++) {
		keyboard[i] = false;
	}


	Window::gameStarted = false;
	Window::doneInitialRender = false;
	Window::gameEnded = false;

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
	initSelectScreenElements();

	//  ==========  End of Select Screen  ========== 

	//  ==========  Environment Initialization  ========== 
	//NOTE: envs now only contain environment objects that are globally viewable. All other objects that require
	//proximity rendering should be inserted into "table"
	// envs.push_back(new EnvElement("shaders/environment/ground.obj", &projection, &view, shaderProgram,
	// 	glm::vec3(0.f, -1.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f)));


	//for now tileScale should be tileSize / 2.0
	ground = new Ground("shaders/environment/ground.obj", &projection, &view, texShader,
		"shaders/environment/dry_grass_texture_3x3.png", "shaders/environment/cracked_tile_texture_3x3.png", &table, 3.0f);

	#ifdef RENDER_MAP
	printf("=======================================\nIt will take a while for the game to launch, please wait.\n");
	initMap();
	#endif


	//  ==========  End of Environment Initialization  ========== 

	//  ==========  Character Initialization   ========== 

	
	/* ===== THIS #ifndef CODE IS ONLY FOR NON-CONNECTED CLIENTS TO IMPROVE GRAPHICS DEVELOPMENT ==== */
	#ifndef SERVER_ENABLED
	chars[0] = new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(SPAWN_POSITIONS[0][0], 1.5f, SPAWN_POSITIONS[0][1]), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f),
		"shaders/character/MAGE");	
	clientChar = chars[0];
	Window::gameStarted = true;
	guiManager->healthBar->flashHealthBar();
	#endif
	/* ===== end of #ifndef (no-server client) code ==== */

	initCharacters();

	//  ==========  End of Character Initialization ========== 

	guiManager->setSplashLoaded(true);
	return true;
}

void Window::initMap() {
	ifstream map_file("../assets/layout/map_client.csv");
    string line;
    string id;

    while(getline(map_file, line)) {
        istringstream ss(line);
        string field;

		// fields: objName, x, y, width, height (x and y are upper left corner of obj)
		string objName;
		int fields[4] = {0,0,0,0};
		int i = 0;
		while (getline(ss, field, ',')) {
			if(i == 0) {
				objName = field;
			} else {
				fields[i - 1] = stoi(field);
			}
			i++;
		}
		float objX = fields[0];
		float objY = fields[1];
		float width = fields[2];
		float height = fields[3];

		// Spawn the obstacles based on their name
		
		// Green wall = hedge
		if (strcmp(objName.c_str(), "hedge") == 0) {
			// do a for loop to fill the hedge?
			for(int x = objX; x < objX + width; x++) {
				for(int y = objY; y < objY + height; y++) {

					EnvElement* e = new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram,
						glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f));
					table.insert(e);
				}
			}

		// White cube ==  pillar
		} else if (strcmp(objName.c_str(), "pillar") == 0) {
			objX += width / 2;
			objY += height / 2;
			EnvElement* e = new EnvElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram,
				glm::vec3(objX, 1.f, objY), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f),  width, glm::vec3(1.f, 1.f, 1.f)); 
			table.insert(e);

		// Green Tree ==   tree_live
		} else if (strcmp(objName.c_str(), "tree_live") == 0) {
			objX += width / 2;
			objY += height / 2;
			EnvElement* e = new EnvElement("shaders/environment/lowpolypine.obj", &projection, &view, shaderProgram,
				glm::vec3(objX, 7.f, objY), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), width, glm::vec3(0.f, 1.f, 0.f));
			table.insert(e);

		// dead tree = grayish black
		} else if (strcmp(objName.c_str(), "tree_dead") == 0) {
			objX += width / 2;
			objY += height / 2;
			EnvElement* e = new EnvElement("shaders/environment/lowpolypine.obj", &projection, &view, shaderProgram,
				glm::vec3(objX, 7.f, objY), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), width, glm::vec3(0.2f, 0.2f, 0.2f));
			table.insert(e);

		// gray ==  Rock
		} else if (strcmp(objName.c_str(), "rock") == 0) {
			objX += width / 2;
			objY += height / 2;

			EnvElement* e = new EnvElement("shaders/environment/lowpolyrock1.obj", &projection, &view, shaderProgram,
				glm::vec3(objX, 1.f, objY), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f),  width * 2.f, glm::vec3(0.7f, 0.7f, 0.7f));
			table.insert(e);

		// Red cube ==  wall
		} else if (strcmp(objName.c_str(), "wall") == 0) {
			// do a for loop to fill the Wall ?

			for(int x = objX + 3.75f; x < objX + width - 4.0f; x+= 4) {
				for(int y = objY; y < objY + height; y++) {

					EnvElement* e = new EnvElement("shaders/environment/brickwall.obj", &projection, &view, texShader,
						glm::vec3(x, 5.f, y + height / 2), glm::vec3(.5f, 0.f, .5f), glm::radians(180.f), 1.f, glm::vec3(1.f, .5f, .5f), "shaders/environment/brick_wall_texture_3x3.png");
					table.insert(e);

				}
			}
		}
		
	} 
}

void Window::initCharacters() {
	printf("duolan init char\n");

	// Initialize character objects before the screen loads.
	playerTypeToCharacterMap[FIGHTER] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(0.f, 2.f, 0.f), 
		glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/FIGHTER"));	
	playerTypeToCharacterMap[MAGE] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(0.f, 2.f, 0.f), 
		glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/MAGE"));	
	playerTypeToCharacterMap[CLERIC] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(0.f, 2.f, 0.f), 
		glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/CLERIC"));	
	playerTypeToCharacterMap[ROGUE] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(0.f, 2.f, 0.f), 
		glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/ROGUE"));
	playerTypeToCharacterMap[MONSTER] = (new Character("shaders/character/billboard.obj", &projection, &view, &eyePos, texShader,
		glm::vec3(0.f, 2.f, 0.f), 
		glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 5.f, glm::vec3(1.f, .5f, .5f), "shaders/character/MAGE"));
	
	// Load animations
	// playerTypeToCharacterMap[FIGHTER]->loadAnimationAssets("shaders/character/FIGHTER");
	// playerTypeToCharacterMap[MAGE]->loadAnimationAssets("shaders/character/MAGE");
	// playerTypeToCharacterMap[CLERIC]->loadAnimationAssets("shaders/character/CLERIC");
	// playerTypeToCharacterMap[ROGUE]->loadAnimationAssets("shaders/character/ROGUE");
	// playerTypeToCharacterMap[MONSTER]->loadAnimationAssets("shaders/character/MAGE");


	// Set the last character's vector to the monster character object
	chars[3] = playerTypeToCharacterMap[MONSTER];
	chars[3]->moveTo(glm::vec3(SPAWN_POSITIONS[3][0], 1.5f, SPAWN_POSITIONS[3][1]));
}

void Window::initSelectScreenElements() {
	//  ==========  Select Screen  ========== 
	float selectScreenScale = 9.f;

	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		glm::vec2(0.0f, 0.0f), selectScreenScale, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/character_select_background.png"));
	

	// Each Job Buttons
	// (1) Fighter
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		glm::vec2(0.f, 0.f), selectScreenScale, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/fighter_unselected.png"));
		
	// (2) Mage
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		glm::vec2(0.f, 0.f), selectScreenScale, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/mage_unselected.png"));
		
	// (3) Cleric
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		glm::vec2(0.f, 0.f), selectScreenScale, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/cleric_unselected.png"));
		
	// (4) Rogue
	guiManager->selectScreen->selectScreenElements.push_back(new ScreenElement("shaders/character/billboard.obj", &projection, &view, &lookAtPoint, texShader,
		glm::vec2(0.f, 0.f), selectScreenScale, glm::vec3(1.f, .5f, .5f),
		"shaders/select_screen/rogue_unselected.png"));

}

// End of init methods

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

	// CommunicationClient no longer blocks on constructor. It will block on connectTo().
	client = new CommunicationClient();

	// setup audio program
	audioProgram = new AudioProgram();
	audioProgram->setMusicVolume(0.55);


	// Display once to show splash screen, then we can deal with connecting window.
	audioProgram->playAudioWithLooping(TITLE_MUSIC);
	guiManager->setSplashScreenVisible(true);
	Window::displayCallback(window);
	guiManager->setConnectingScreenVisible(true);

	// Set swap interval to 1 if you want buffer 
	glfwSwapInterval(0);

	/* ===== THIS #ifndef CODE IS ONLY FOR NON-CONNECTED CLIENTS TO IMPROVE GRAPHICS DEVELOPMENT ==== */
#ifndef SERVER_ENABLED // Client-only (no server)	
	guiManager->setConnectingScreenVisible(false);
	guiManager->beaconBar->setAmount(18.f);
	guiManager->evoBar->setEvo(2.65f);
	guiManager->setSelectScreenVisible(false);
	guiManager->setGameEndVisible(false);
	guiManager->setSplashScreenVisible(false);
	guiManager->initCooldownIcons(MONSTER);
	guiManager->setHUDVisible(true);
	guiManager->evoBar->setVisible(true);
#endif

	/* ===== end of #ifndef (no-server client) code ==== */
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Fixes window crashing when resizing or minimizing
	if (height > 0 && width > 0) {

		// Update the GUIManager's window height/width
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		guiManager->resizeWindow(width, height, fbWidth, fbHeight);

		// Set the projection matrix.
		Window::projection = glm::perspective(glm::radians(80.0), double(width) / (double)height, 0.5, 1000.0);

	}

	// update projection matrix for all the objects
	//int i;
	//for (i = 0; i < chars.size(); i++) {
		
	//}
}

void Window::idleCallback()
{

	Window::updateLastInput();
#ifdef SERVER_ENABLED
	if(client->isConnected()) {
		// 1 + 2. Get the latest input and send it to the server
		GAME_INPUT input;
		input.input = Window::lastInput;
		input.angle = Window::lastAngle;
		client->sendInput(input);

		// 3. Receive updated gamestate from server
		std::vector<GameUpdate> updates = client->receiveGameUpdates();

		// cout << "updating game" << endl;
		Window::handleUpdates(updates);
	}
	lastInput = NO_MOVE;

#endif
	//update camera location
	if(Window::gameStarted && clientChar != nullptr) {
		lookAtPoint = clientChar->pos;
	}
	eyePos = lookAtPoint + glm::vec3(CAMERA_X_OFFSET, CAMERA_Y_OFFSET, CAMERA_Z_OFFSET);
	view = glm::lookAt(Window::eyePos, Window::lookAtPoint, Window::upVector);

	int i;
	for (i = 0; i < chars.size() && Window::gameStarted; i++) {
		if (chars[i] != nullptr) {
			chars[i]->update();
		}
	}

	guiManager->updateHUDPositions();
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
	vector<Object*> result;
	float h = SPATIAL_HASH_SEARCH_DISTANCE;
	int j;
	glm::vec3 base1(-1.f * h, 0.f, 0.f);
	for (j = 0; j < 3 && Window::gameStarted; j++) {
		int k;
		glm::vec3 base2(0.f, 0.f, -2.f * h);   //negative 2 multiplier, so the search area is ahead into the screan
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
		if(clientChar != nullptr) {
			result[i]->drawIfNotObstructing(clientChar->pos);
		}
	}

	for (i = 0; i < chars.size() && Window::gameStarted; i++) {
		if (chars[i] != nullptr) {
			chars[i]->draw();
		}
	}

	// Draws all the objectives in the objective map
	for (auto const& x: objectiveMap) {
		x.second->draw();
		if(clientChar != nullptr) {
			checkNearObjectiveText(x.second);
		}
	}

	// Draws all the projectiles
	for(auto iter = projectiles.begin(); iter != projectiles.end(); iter++) {
		iter->second->draw();

	}

	Window::guiManager->draw();


	glfwPollEvents();
	glfwSwapBuffers(window);
	if(client->isConnected() && !guiManager->selectScreen->isVisible && !gameStarted) {
	#ifdef SERVER_ENABLED
		// send update that we've finished rendering to the server
		GAME_INPUT gameInput;
		gameInput.input = DONE_RENDERING;
		client->sendInput(gameInput);
		printf("done_rendering signal sent\n");
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
		if (!guiManager->connectingScreen->hasConnectedToServer && !gameEnded) {
			guiManager->connectingScreen->handleKeyInput(key, window);
		}

		if(key == GLFW_KEY_M) {
			audioProgram->toggleMute();
		}
		
		if (gameEnded) {
			gameEnded = false;
			guiManager->setConnectingScreenVisible(true);
			guiManager->setSplashScreenVisible(true);
			guiManager->setGameEndVisible(false);

			audioProgram->stopAllAudio();
			audioProgram->playAudioWithLooping(TITLE_MUSIC);
		}

	 // Check for a key release.
	} else if (action == GLFW_RELEASE) {
		keyboard[key] = false;
	}
}


void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) mouse[MOUSE_LEFT_INDEX] = (action == GLFW_PRESS);
	if (button == GLFW_MOUSE_BUTTON_RIGHT)  mouse[MOUSE_RIGHT_INDEX] = (action == GLFW_PRESS);
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	float dx2 = width / 2 - MouseX;
	float dy2 = MouseY - height / 2;
	float dx1 = 0;
	float dy1 = 1;
	float dot = dx1*dx2 + dy1*dy2;      // dot product
	float det = dx1*dy2 - dy1*dx2;      // determinant
	lastAngle = atan2(det, dot) - M_PI / 2;  // atan2(y, x) or atan2(sin, cos)

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
	playerJobs[update.id] = update.roleClaimed;

	switch(update.roleClaimed) {
		case FIGHTER:
			chars[update.id] = playerTypeToCharacterMap[FIGHTER];
			chars[update.id]->moveTo(glm::vec3(SPAWN_POSITIONS[update.id][0], 2.f, SPAWN_POSITIONS[update.id][1]));
			break;

		case MAGE:
			chars[update.id] = playerTypeToCharacterMap[MAGE];
			chars[update.id]->moveTo(glm::vec3(SPAWN_POSITIONS[update.id][0], 1.5f, SPAWN_POSITIONS[update.id][1]));
			break;

		case CLERIC:
			chars[update.id] = playerTypeToCharacterMap[CLERIC];
			chars[update.id]->moveTo(glm::vec3(SPAWN_POSITIONS[update.id][0], 2.f, SPAWN_POSITIONS[update.id][1]));
			break;

		case ROGUE:
			chars[update.id] = playerTypeToCharacterMap[ROGUE];
			chars[update.id]->moveTo(glm::vec3(SPAWN_POSITIONS[update.id][0], 2.f, SPAWN_POSITIONS[update.id][1]));
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
		guiManager->initCooldownIcons(update.roleClaimed);
	} else {
		guiManager->miniMap->setPlayerType(update.id, update.roleClaimed);
	}
}

// Handles specific update on the graphics side.
void Window::handleUpdate(GameUpdate update) {
    switch(update.updateType) {
        case PLAYER_DAMAGE_TAKEN: {

			if(update.id == client->getId()) {
				guiManager->healthBar->decrementHp(update.damageTaken);
				guiManager->healthBar->flashHealthBar();
			}
			chars[update.id]->flashDamage();

			auto now = std::chrono::steady_clock::now();
    		std::chrono::duration<float> duration = now - lastCombatMusicPlayTime;

			
			if (std::chrono::duration_cast<std::chrono::seconds>(duration).count() > audioProgram->getSoundLength(COMBAT_MUSIC)) {
				audioProgram->playAudioWithoutLooping(COMBAT_MUSIC);
				lastCombatMusicPlayTime = now;
			}
            break;
		}
		case PLAYER_HP_INCREMENT:
			if(update.id == client->getId()) {
				guiManager->healthBar->incrementHp(update.healAmount);
			}
			break;
		case SAFE_REGION_UPDATE:
			guiManager->miniMap->safeRegionX = update.playerPos.x;
			guiManager->miniMap->safeRegionY = update.playerPos.y;
			guiManager->miniMap->safeRegionRadius = update.beaconCaptureAmount;
			break;
        case PLAYER_MOVE:
		{
			chars[update.id]->setState(moving);
			switch(update.direction) {
				case NORTH:
					chars[update.id]->setDirection(NORTH);
					break;
				case EAST:
					chars[update.id]->setDirection(EAST);
					break;
				case SOUTH:
					chars[update.id]->setDirection(SOUTH);
					break;
				case WEST:
					chars[update.id]->setDirection(WEST);
					break;
				case NORTH_WEST:
					chars[update.id]->setDirection(WEST);
					break;
				case SOUTH_WEST:
					chars[update.id]->setDirection(WEST);
					break;
				case NORTH_EAST:
					chars[update.id]->setDirection(EAST);
					break;
				case SOUTH_EAST:
					chars[update.id]->setDirection(EAST);
					break;
			}
			chars[update.id]->moveToGivenDelta(update.floatDeltaX, update.floatDeltaY);
			guiManager->miniMap->updatePlayerPositionDelta(update.id, update.floatDeltaX, update.floatDeltaY);
			printf("Character %d moved with deltaX: %f, deltaY: %f\n", update.id, update.floatDeltaX, update.floatDeltaY);
            break;
        
		}
		case PROJECTILE_MOVE:
		{
			
			glm::vec3 trans = glm::vec3(update.playerPos.x, 1.f, update.playerPos.y);
			float scale = 1.f;
			glm::vec3 color = glm::vec3(1.f, .5f, .5f);
			string textFile;

			// select projectile texture
			if (update.projectileType == MAGE_FIREBALL) textFile ="shaders/projectile/fireball_up.png";
			if (update.projectileType == MAGE_SHOOT) textFile ="shaders/projectile/firearrow_up.png";
			if (update.projectileType == ROGUE_ARROW) textFile ="shaders/projectile/arrow_up.png";
			if (update.projectileType == CLERIC_SHOOT) textFile ="shaders/projectile/lightball_up.png";
			if (update.projectileType == MONSTER_RANGED) textFile ="shaders/projectile/earthchunk_up.png";


			ProjectileElement* pEle = new ProjectileElement("shaders/character/billboard.obj", &projection, &view, texShader, &eyePos,
															trans, scale, color, (char*) textFile.c_str(), update.floatDeltaX, update.floatDeltaY);
			projectiles[update.id] = pEle;

            break;
		}
		case PROJECTILE_END:
			delete projectiles[update.id];
			projectiles.erase(update.id);
			break;
		case GAME_STARTED:
			Window::gameStarted = true;
			guiManager->setSelectScreenVisible(false); // disable the selects creen
			guiManager->setHUDVisible(true); // sets the hud visible
		
			// If client is monster, set the evo visible.
			if(playerJobs[client->getId()] == MONSTER) {
				guiManager->evoBar->isMonster = true;
			} else {
				guiManager->evoBar->isMonster = false;
			}

			audioProgram->stopAudio(TITLE_MUSIC);
			break;
		case ROLE_CLAIMED:
			Window::handleRoleClaim(update);
            break;
		case PLAYER_ATTACK:
			printf("Calling handleAttack()\n");
			Window::handleAttack(update);
			break;
		case PLAYER_UNIQUE_ATTACK:
			Window::handleUniqueAttack(update);
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
			guiManager->setSplashScreenVisible(false);
			break;

		// Objective spawned
		case SPAWN_OBJECTIVE: {
			initializeObjective(update.objectiveID, update.objectiveSpawnType, update.objRestrictionType, update.gridPos.x, update.gridPos.y);

			break;
		}

		// objective taken
		
        // Objectives:
        case HEAL_OBJECTIVE_TAKEN:
        case ARMOR_OBJECTIVE_TAKEN:
			if(update.id == client->getId()) {
				guiManager->healthBar->incrementHp(update.healAmount);
			}
            removeObj(update.objectiveID);
            break;
        case EVO_OBJECTIVE_TAKEN:
            // The level up process done in another update.
            removeObj(update.objectiveID);
            break;

		case MONSTER_EVO_UP:
			guiManager->evoBar->setEvo(update.newEvoLevel);
			break;

		case GAME_END:
			endGame();
			guiManager->setHUDVisible(false);
			guiManager->setConnectingScreenVisible(false);
			guiManager->setSelectScreenVisible(false);
			guiManager->setGameEndVisible(true);
			break;
        default:
            printf("Not Handled Update Type: %d\n", update.updateType);
            break;
    }
}

void Window::handleAttack(GameUpdate update) {
	printf("Player %d is attacking\n", update.id);
	chars[update.id]->setState(attacking);

	// Update the cooldown timer
	if(update.id == client->getId()) {
		guiManager->handleCooldownUpdate(NORMAL_ATTACK);
	}

	// Play the audio
	if(glm::distance(chars[update.id]->pos, clientChar->pos) < MAX_HEARING_DISTANCE) {
		
		AudioPosition clientPos = {clientChar->pos.x, clientChar->pos.z};
		AudioPosition otherPos = {chars[update.id]->pos.x, chars[update.id]->pos.z};
		switch(playerJobs[update.id]) {
			case FIGHTER:
				audioProgram->playDirectionalEffect(FIGHTER_ATTACK_SOUND, clientPos, otherPos);
				break;

			case MAGE:
				audioProgram->playDirectionalEffect(MAGE_ATTACK_SOUND, clientPos, otherPos);
				break;

			case CLERIC:
				audioProgram->playDirectionalEffect(CLERIC_ATTACK_SOUND, clientPos, otherPos);
				break;

			case ROGUE:
				audioProgram->playDirectionalEffect(ROGUE_ATTACK_SOUND, clientPos, otherPos);
				break;

			case MONSTER:
				audioProgram->playDirectionalEffect(MONSTER_THROW_SOUND, clientPos, otherPos);
				break;
		}
	}
}

void Window::handleUniqueAttack(GameUpdate update) {

	// animation depends on who is attacking but for now it's default.
	// e.g. fighter should not slash when they use their armor ability
	chars[update.id]->setState(attacking);

	// Update the cooldown timer
	if(update.id == client->getId()) {
		guiManager->handleCooldownUpdate(UNIQUE_ATTACK);
	}

	// TO BE ADDED: Audio for unique attacks
	// if(glm::distance(chars[update.id]->pos, clientChar->pos) < MAX_HEARING_DISTANCE) {
		
	// 	AudioPosition clientPos = {clientChar->pos.x, clientChar->pos.z};
	// 	AudioPosition otherPos = {chars[update.id]->pos.x, chars[update.id]->pos.z};
	// 	switch(playerJobs[update.id]) {
	// 		case FIGHTER:
	// 			audioProgram->playDirectionalEffect(FIGHTER_ATTACK_SOUND, clientPos, otherPos);
	// 			break;

	// 		case MAGE:
	// 			audioProgram->playDirectionalEffect(MAGE_ATTACK_SOUND, clientPos, otherPos);
	// 			break;

	// 		case CLERIC:
	// 			audioProgram->playDirectionalEffect(CLERIC_ATTACK_SOUND, clientPos, otherPos);
	// 			break;

	// 		case ROGUE:
	// 			audioProgram->playDirectionalEffect(ROGUE_ATTACK_SOUND, clientPos, otherPos);
	// 			break;

	// 		case MONSTER:
	// 			audioProgram->playDirectionalEffect(MONSTER_THROW_SOUND, clientPos, otherPos);
	// 			break;
	// 	}
	// }

	
}

// This function checks if a certain key is being pressed or held down.
void Window::updateLastInput() {
	// E key
	if (keyboard[GLFW_KEY_E]) {
		lastInput = INTERACT;

	// WA key together
	} 
	else if(keyboard[GLFW_KEY_W] && keyboard[GLFW_KEY_A]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_UPLEFT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_UPLEFT_UNIQUE_ATTACK;
		else lastInput = MOVE_UPLEFT;

	// WD key together
	} else if(keyboard[GLFW_KEY_W] && keyboard[GLFW_KEY_D]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_UPRIGHT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_UPRIGHT_UNIQUE_ATTACK;
		else lastInput = MOVE_UPRIGHT;

	// AS key together
	} else if(keyboard[GLFW_KEY_S] && keyboard[GLFW_KEY_A]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_DOWNLEFT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_DOWNLEFT_UNIQUE_ATTACK;
		else lastInput = MOVE_DOWNLEFT;

	// SD key together
	} else if(keyboard[GLFW_KEY_S] && keyboard[GLFW_KEY_D]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_DOWNRIGHT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput =MOVE_DOWNRIGHT_UNIQUE_ATTACK;
		else lastInput = MOVE_DOWNRIGHT;

	// W key
	} 
	else if (keyboard[GLFW_KEY_W]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_FORWARD_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_FORWARD_UNIQUE_ATTACK;
		else lastInput = MOVE_FORWARD;

	// A key
	} else if(keyboard[GLFW_KEY_A]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_LEFT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_LEFT_UNIQUE_ATTACK;
		else lastInput = MOVE_LEFT;

	// S key
	} else if(keyboard[GLFW_KEY_S]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_BACKWARD_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_BACKWARD_UNIQUE_ATTACK;
		else lastInput = MOVE_BACKWARD;
		
	// D key
	} else if(keyboard[GLFW_KEY_D]) {
		if (mouse[MOUSE_LEFT_INDEX]) lastInput = MOVE_RIGHT_ATTACK;
		else if (mouse[MOUSE_RIGHT_INDEX]) lastInput = MOVE_RIGHT_UNIQUE_ATTACK;
		else lastInput = MOVE_RIGHT;

	// 1 (selec fighter)
	} else if(keyboard[GLFW_KEY_1]) {

		// Select screen logic
		if (guiManager->connectingScreen->hasConnectedToServer && !gameStarted) {
			guiManager->selectScreen->handleSelecting(FIGHTER);
			audioProgram->playAudioWithoutLooping(SELECT_SOUND);
		}
	// 2 key (select mage)
	} else if(keyboard[GLFW_KEY_2]) {
		
		// Select screen logic
		if (guiManager->connectingScreen->hasConnectedToServer && !gameStarted) {
			guiManager->selectScreen->handleSelecting(MAGE);
			audioProgram->playAudioWithoutLooping(SELECT_SOUND);
		}

	// 3 key (select cleric)
	} else if(keyboard[GLFW_KEY_3]) {
		
		// Select screen logic
		if (guiManager->connectingScreen->hasConnectedToServer && !gameStarted) {
			guiManager->selectScreen->handleSelecting(CLERIC);
			audioProgram->playAudioWithoutLooping(SELECT_SOUND);
		}

	// 4 key (select rogue)
	} else if(keyboard[GLFW_KEY_4]) {
		
		// Select screen logic
		if (guiManager->connectingScreen->hasConnectedToServer && !gameStarted) {
			guiManager->selectScreen->handleSelecting(ROGUE);
			audioProgram->playAudioWithoutLooping(SELECT_SOUND);
		}
		
	// enter key (claim selected role)
	} else if (keyboard[GLFW_KEY_ENTER]) {

		// Select screen logic
		if (guiManager->connectingScreen->hasConnectedToServer && !gameStarted) {
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
			audioProgram->playAudioWithoutLooping(SELECT_SOUND);
		}
	} else if (mouse[MOUSE_LEFT_INDEX]) {
		lastInput = LEFT_MOUSE_ATTACK;

	} else if (mouse[MOUSE_RIGHT_INDEX]) {
		lastInput = RIGHT_MOUSE_ATTACK;
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
	} else if(keyboard[GLFW_KEY_J]) {
		audioProgram->playDirectionalEffect(MAGE_FIREBALL_SOUND, {0,0}, {0,0});
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
		guiManager->initCooldownIcons(MONSTER);
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

void Window::initializeObjective(int objectiveID, ObjectiveType type, Restriction restriction, float x, float y) {

	switch(type) {
		case EVO:{
			
			ObjElement* e = new ObjElement("shaders/character/billboard.obj", &projection, &view, texShader,
				glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), false, "shaders/objectives/evolution_pickup.png", EVO, R_MONSTER);
			objectiveMap[objectiveID] = e;
			break;

		}
		case HEAL:{
			switch(restriction) {
				case R_HUNTER: {
					ObjElement* e = new ObjElement("shaders/character/billboard.obj", &projection, &view, texShader,
						glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), false, "shaders/objectives/health_pickup.png", HEAL, R_HUNTER);
					objectiveMap[objectiveID] = e;
					break;
				}
				case R_MONSTER: {
					ObjElement* e = new ObjElement("shaders/character/billboard.obj", &projection, &view, texShader,
						glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), true, "shaders/objectives/health_pickup.png", HEAL, R_MONSTER);
					objectiveMap[objectiveID] = e;
					break;
				}
			}
			break;
			
		}
		case BEACON:{

			ObjElement* e = new ObjElement("shaders/environment/cube_env.obj", &projection, &view, shaderProgram,
				glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), .25f, glm::vec3(0.f, 0.f, 1.f), false, "", BEACON, R_NEUTRAL);
			objectiveMap[objectiveID] = e;
			break;
		}
		case ARMOR:{

			ObjElement* e = new ObjElement("shaders/character/billboard.obj", &projection, &view, texShader,
				glm::vec3(x, 1.f, y), glm::vec3(0.f, 1.f, 0.f), glm::radians(0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), false, "shaders/objectives/armour_pickup.png", ARMOR, R_HUNTER);
			objectiveMap[objectiveID] = e;
			break;
		}
		case INVALID_OBJ:{
			printf("Invalid OBJECTIVE_SPAWN objType. \n");
			break;
		}
			
	}
	
	return;
}


void Window::removeObj(int objectiveID) {
	if (objectiveMap.find(objectiveID) != objectiveMap.end()) {
		delete objectiveMap[objectiveID];
		objectiveMap.erase(objectiveID);
	}
}

void Window::endGame() {
	// reset comm Client
	client->cleanup();
	delete client;
	client = new CommunicationClient();

	// reset gui/hud
	guiManager->reset();

	// Reset clientChar pointer
	clientChar = nullptr;

	// Reset window boolean variables
	gameStarted = false;
	doneInitialRender = false;

	// reset objectives map
	for(std::map<int, ObjElement*>::iterator i = objectiveMap.begin(); i != objectiveMap.end(); i++) {
		if(objectiveMap[i->first] != nullptr) {
			delete objectiveMap[i->first];
		}
	}
	objectiveMap.clear();

	// reset projectiles map
	for(std::unordered_map<int, ProjectileElement*>::iterator i = projectiles.begin(); i != projectiles.end(); i++) {
		if(projectiles[i->first] != nullptr) {
			delete projectiles[i->first];
		}
	}
	projectiles.clear();

	// reset the player jobs
	playerJobs = {UNKNOWN, UNKNOWN, UNKNOWN, MONSTER};

	// reset monster position
	chars[3] = playerTypeToCharacterMap[MONSTER];
	chars[3]->moveTo(glm::vec3(SPAWN_POSITIONS[3][0], 1.5f, SPAWN_POSITIONS[3][1]));

	gameEnded = true;
}

void Window::checkNearObjectiveText(ObjElement* obj) {
	PlayerType playerJob = playerJobs[client->getId()];
	ObjectiveType objType = obj->getObjType();
	Restriction restriction = obj->getRestrictionType();

	// For restricted objectives
	if((playerJob == MONSTER && restriction == R_MONSTER) || (playerJob != MONSTER && restriction == R_HUNTER) || (restriction == R_NEUTRAL)) {
		switch(objType) {
			case HEAL:

				if(glm::distance(obj->pos, clientChar->pos) < HEALING_INTERACTION_RANGE) {
					// nvg draw text to interact with
					guiManager->drawCenterText(std::string("Press E to pickup the health."), width, height);
				}
				break;
			case EVO:
				if(glm::distance(obj->pos, clientChar->pos) < EVO_INTERACTION_RANGE) {
					// nvg draw text to interact with
					guiManager->drawCenterText(std::string("Press E to pickup the shard."), width, height);
				}
				break;
			case ARMOR:
				if(glm::distance(obj->pos, clientChar->pos) < ARMOR_INTERACTION_RANGE) {
					// nvg draw text to interact with
					guiManager->drawCenterText(std::string("Press E to pickup the armor."), width, height);
				}
				break;
			case BEACON:
				if (guiManager->beaconBar->captureAmount > MONSTER_BEACON_CAPTURE_THRESHOLD &&
					guiManager->beaconBar->captureAmount < HUNTER_BEACON_CAPTURE_THRESHOLD && 
					glm::distance(obj->pos, clientChar->pos) < BEACON_INTERACTION_RANGE) {
					guiManager->drawCenterText(std::string("Capturing beacon..."), width, height);
				}
				break;
			default:
				break;
		}
	}
}