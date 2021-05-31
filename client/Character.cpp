#include "Character.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
	constructor usage:
	projection p, view v, view position, and shader s are taken cared of in Window class.
	translation trans is the initial position where you want to place this object;
	rotAxis is the the axis you want to rotate about;
	rotRad  is the amount of rotation you want in RADIAN;
	scale is a factor you want to scale the initial object;
	color c is the initial model color; default is black
*/

Character::Character(string fileName, glm::mat4* p, glm::mat4* v, glm::vec3* vPos,
	GLuint s, glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale,
	glm::vec3 c, char* textFile) : idleTex(4), moveTex(4), attackTex(4), currDirec(Direction::WEST) {

	//animation related
	frameIdx = 0;
	currState = idle;
	prevState = idle;
	currTime = clock();
	prevTime = currTime;

	// prepare vectors for idle animations
	idleTex[Direction::WEST] = &idle_west;
	idleTex[Direction::EAST] = &idle_east;
	idleTex[Direction::NORTH] = &idle_north;
	idleTex[Direction::SOUTH] = &idle_south;
	animSequence.push_back(&idleTex); //order must match the enum in header file


	// prepare vectors for move animations
	moveTex[Direction::WEST] = &move_west;
	moveTex[Direction::EAST] = &move_east;
	moveTex[Direction::NORTH] = &move_north;
	moveTex[Direction::SOUTH] = &move_south;
	animSequence.push_back(&moveTex);


	// prepare vectors for attack animations
	attackTex[Direction::WEST] = &attack_west;
	attackTex[Direction::EAST] = &attack_east;
	attackTex[Direction::NORTH] = &attack_north;
	attackTex[Direction::SOUTH] = &attack_south;
	animSequence.push_back(&attackTex);


	// initial translation will bthe initial position
	pos = trans;
	scaleMtx = glm::scale(glm::vec3(scale));
	model = glm::translate(trans) * glm::rotate(rotRad, rotAxis) * scaleMtx;
	projection = p;
	view = v;
	eyep = vPos;
	shader = s;
	// default color is black
	color = c;
	// if path is NOT given at construction time, hasTexture will be false.
	hasTexture = loadAnimationAssets(textFile);

	// For flashing the character on damage taken events
	isVisible = true;
	lastDamageFlash = currTime;
	damageFlashUntil = currTime;


	std::vector<glm::vec3> normalp;
	std::vector<glm::vec3> pointsp;
	std::vector<glm::vec2> texp;
	string delim = "/";
	std::ifstream objFile(fileName);
	// Check whether the file can be opened.
	if (objFile.is_open())
	{
		std::string line; // A line in the file.
		// Read lines from the file.
		int vCount = 0;

		while (std::getline(objFile, line))
		{
			// Turn the line into a string stream for processing.
			std::stringstream ss;
			ss << line;
			std::string label;
			ss >> label;
			if (label == "v")
			{
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				pointsp.push_back(point);
			}

			if (label == "vt")
			{
				glm::vec2 tex;
				ss >> tex.x >> tex.y;
				texp.push_back(tex);
			}

			if (label == "vn") {
				glm::vec3 norm;
				ss >> norm.x >> norm.y >> norm.z;
				norm = glm::normalize(norm);
				normalp.push_back(norm);
			}

			if (label == "f") {
				glm::ivec3 triangle;
				std::string a, b, c;
				ss >> a >> b >> c;

				points.push_back(pointsp[stoi(a.substr(0, a.find(delim))) - 1]);
				a = a.substr(a.find(delim) + 1, string::npos);
				points.push_back(pointsp[stoi(b.substr(0, b.find(delim))) - 1]);
				b = b.substr(b.find(delim) + 1, string::npos);
				points.push_back(pointsp[stoi(c.substr(0, c.find(delim))) - 1]);
				c = c.substr(c.find(delim) + 1, string::npos);

				if (hasTexture) {
					textCoord.push_back(texp[stoi(a.substr(0, a.find(delim))) - 1]);
					a = a.substr(a.find(delim) + 1, string::npos);
					textCoord.push_back(texp[stoi(b.substr(0, b.find(delim))) - 1]);
					b = b.substr(b.find(delim) + 1, string::npos);
					textCoord.push_back(texp[stoi(c.substr(0, c.find(delim))) - 1]);
					c = c.substr(c.find(delim) + 1, string::npos);
				}

				normal.push_back(normalp[stoi(a.substr(a.find(delim) + 1, string::npos)) - 1]);
				normal.push_back(normalp[stoi(b.substr(b.find(delim) + 1, string::npos)) - 1]);
				normal.push_back(normalp[stoi(c.substr(c.find(delim) + 1, string::npos)) - 1]);

				triangle.x = vCount++;
				triangle.y = vCount++;
				triangle.z = vCount++;
				//cout << triangle.x << " " << triangle.y << " " << triangle.z << endl;
				triangles.push_back(triangle);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	objFile.close();

	//cout << "initing" << endl;
	std::vector<glm::vec3>::iterator it = points.begin();
	//centering + re-position + rescale
	float xPos = 0, xNeg = 0, yPos = 0, yNeg = 0, zPos = 0, zNeg = 0;;
	while (it != points.end()) {
		//cout << "changing" << endl;
		if ((*it).x > xPos)
			xPos = (*it).x;
		if ((*it).x < xNeg)
			xNeg = (*it).x;
		if ((*it).y > yPos)
			yPos = (*it).y;
		if ((*it).y < yNeg)
			yNeg = (*it).y;
		if ((*it).z > zPos)
			zPos = (*it).z;
		if ((*it).z < zNeg)
			zNeg = (*it).z;
		it++;
	}
	float xCenter = (xPos + xNeg) / 2;
	float yCenter = (yPos + yNeg) / 2;
	float zCenter = (zPos + zNeg) / 2;
	it = points.begin();
	while (it != points.end()) {
		(*it).x -= xCenter;
		(*it).y -= yCenter;
		(*it).z -= zCenter;
		//cout << "x: " << (*it).x << " y: " << (*it).y << " z: " << (*it).z << endl;
		it++;
	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal.size(), normal.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if (hasTexture) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textCoord.size(), textCoord.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Character::draw(glm::mat4 c) {
	// if not visible then don't draw
	if(!isVisible) {
		return;
	}
	
	//model used in the shader would be this model mult with passed down transform model
	glm::mat4 m = model * c;
	glUseProgram(shader);


	// ======== Fix the Distorted Character Issue ================
	// get straight line from camera to center of character
	glm::vec3 z = glm::vec3(0.f, 0.f, 0.f);
	for (int i = 0; i < 3; i++) {
		z[i] = pos[i] - (*eyep)[i];
	}

	float scaleX = sqrt(m[0][0]*m[0][0]+m[0][1]*m[0][1]+m[0][2]*m[0][2]);
	float scaleY = sqrt(m[1][0]*m[1][0]+m[1][1]*m[1][1]+m[0][2]*m[1][2]);
	float scaleZ = sqrt(m[2][0]*m[2][0]+m[2][1]*m[2][1]+m[0][2]*m[2][2]);

	glm::vec3 x = glm::cross(glm::vec3(0.f, 0.1f, 0.f), z);
	glm::vec3 y = glm::cross(x, z);
	glm::vec3 xn = glm::normalize(x);
	glm::vec3 yn = glm::normalize(y);
	glm::vec3 zn = glm::normalize(z);

	// rotate the character so it will parallel with the line from camera to character
	for (int i = 0; i < 3;  i++) m[i][0] = xn[i];
	for (int i = 0; i < 3;  i++) m[i][1] = yn[i];
	for (int i = 0; i < 3;  i++) m[i][2] = zn[i];
	// restore scaling
	m[0][0] = scaleX;
	m[1][1] = scaleY;
	m[2][2] = scaleZ;
	// ========================================================


	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(*view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(*projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(m));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(*eyep));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);

	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (hasTexture) {
		//cout << "has texture" << endl;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textId);
	}

	glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);

	glDisable(GL_BLEND);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Character::drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c) {
	draw();
}

//no longer used
void Character::move(int dir) {
}

void Character::moveTo(glm::vec3 newPos) {
	if (pos == newPos)
		currState = idle;

	pos = newPos;
	model = glm::translate(pos) * scaleMtx;
}


void Character::moveToGivenDelta(float deltaX, float deltaY) {
	// deltaX is the x axis in the graphics client
	glm::vec3 newPos = pos;
	newPos.x += deltaX;

	// deltaY is the z axis in the graphics client
	newPos.z += deltaY;

	// move to new position
	moveTo(newPos);
}

void Character::update() {
	currTime = clock();

	if (currState == spectating) return;

	if (currState != prevState) {
		frameIdx = 0; // start a new sequence of animation
		textId = ( *(*animSequence[currState]) [currDirec] )[frameIdx];
		prevState = currState;
	}
	else {
		float timeDiff = (float)(currTime - prevTime) / CLOCKS_PER_SEC;
		
		// interval depends on the currState of the animation
		float interval = IDLE_ANIMATION_INTERVAL;
		switch(currState) {
			case idle:
				interval = IDLE_ANIMATION_INTERVAL;
				break;
			case attacking:
				interval = ATTACK_ANIMATION_INTERVAL;
				break;
			case moving:
				interval = MOVING_ANIMATION_INTERVAL;
				break;
			default:
				printf("Invalid animation state. Update the switch statement.\n");
		}
		if (timeDiff >= interval) {
			// check if on the last frame of a sequence
			// Previous
			// if (frameIdx + 1 == (*(*animSequence[currState])[currDirec]).size()) {
			if (frameIdx + 1 >= ( *(*animSequence[currState]) [currDirec] ).size()) {
				frameIdx = 0;
				if(currState != idle && currState != spectating) currState = idle; // get out of attack and movement state.
			}
			else
				frameIdx++;
			textId = ( *(*animSequence[currState]) [currDirec] )[frameIdx];
			prevState = currState;
			prevTime = currTime;
		}
	}

	// Flash the character
	float flashTimeDiff = (float) (currTime - lastDamageFlash) / CLOCKS_PER_SEC * 1000;
	if(currTime < damageFlashUntil && flashTimeDiff > CHARACTER_DAMAGE_TAKEN_FLASHING_INTERVAL_MS) {
		isVisible = !isVisible;
		lastDamageFlash = currTime;

	// past the flash time, set reversedColor to false so we return to normal colors
	} else if (currTime > damageFlashUntil)  {
		isVisible = true;
	}
}

void Character::updateView(glm::mat4, glm::vec3) {
}

GLuint Character::loadTexture(string path) {
	const char* texturePath = path.c_str();
	if (strcmp(texturePath, "") == 0)
		return false;
	FILE* f;
	if (f = fopen(texturePath, "r")) {
		fclose(f);
		cout << "loading texture at " << texturePath << endl;
	}
	else {
		cout << "cannot load texture at " << texturePath << endl;
		return false;
	}

	int ftw, fth, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &ftw, &fth, &channels, STBI_rgb_alpha);
	if (data == NULL) {
		cout << "cannot load texture at " << texturePath << endl;
		return false;
	}
	//cout << "num of channels: " << channels << endl;
	
	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ftw, fth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	fclose(f);
	return tid;
}

/*
	usage:to load an animation sequence for one state of this character
	-state is the state this animation is for, such as idle, moving, attacking, check enum in header
	-animFolder is the folder containing the texture assets of this animation. The arguement for
	this parameter should just be the folder path without ending "/"; a good example would be 
	"shaders/character/MAGE_LEFT_IDLE". 
	In this folder there needs to be an "index.txt" file containing all the actual texture files with 
	leading "/" like "/attack1.png"
*/

bool Character::loadAnimation(CharState state, Direction d, string animFolder) {
	//cout << animFolder << endl;
	std::ifstream objFile(animFolder + "/index.txt");
	if (!objFile.is_open()) {
		cout << "cannot open anim size file in " << animFolder << endl;
		return false;
	}
	vector<GLuint> * animation = (*animSequence[state])[d];
	std::string line;
	while (std::getline(objFile, line)) {
		string texFile =animFolder + line;
		//cout << texFile << endl;
		animation->push_back(loadTexture(texFile.c_str()));
	}
	objFile.close();
	return true;
}

bool Character::loadAnimationAssets(string assetFolder) {
	std::ifstream objFile(assetFolder + "/index.txt");
	if (!objFile.is_open()) {
		cout << "cannot open asset size file in " << assetFolder << endl;
		return false;
	}

	std::string line;
	while (std::getline(objFile, line)) {
		std::stringstream ss;
		ss << line;
		int state;
		int direction;
		string animFolder;
		ss >> state >> direction >> animFolder;
		//cout << state << " " << animFolder;
		if (!loadAnimation((CharState)state, (Direction) direction, assetFolder + animFolder) ) {
			cout << "cannot load asset in " << animFolder << endl;
			return false;
		}
	}
	objFile.close();
	return true;
}

// set the state of the character
void Character::setState(CharState state) {
	currState = state;
}

CharState Character::getState() {
	return currState;
}

void Character::flashDamage() {
	damageFlashUntil = clock() + CHARACTER_DAMAGE_TAKEN_FLASHING_TIME_MS * CLOCKS_PER_SEC / 1000;
}

// set the current direction the player is facing
void Character::setDirection(Direction d) {
	currDirec = d;
}

int Character::getViewingSpecID() {
	return viewingSpecID;
}

void Character::setViewingSpecID(int id) {
	viewingSpecID = id;
}