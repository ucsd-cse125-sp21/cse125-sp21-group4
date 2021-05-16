#include "ProjectileElement.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*
	constructor usage:
	projection p, view v, and shader s are taken cared of in Window class.
	translation trnas is the initial position where you want to place this object;
	rotAxis is the the axis you want to rotate about;
	rotRad  is the amount of rotation you want in RADIAN;
	scale is a factor you want to scale the initial object;
	color c is the initial model color; default is black
*/

ProjectileElement::ProjectileElement(string fileName, glm::mat4* p, glm::mat4* v, GLuint s,
	glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale, glm::vec3 c, char* textFile) {

	// initial translation will bthe initial position
	pos = trans;
	model = glm::rotate(rotRad, rotAxis) * glm::translate(trans) * glm::scale(glm::vec3(scale));
	projection = p;
	view = v;
	shader = s;
	//default color is black
	color = c;
	// if path is NOT given at construction time, hasTexture will be false.
	hasTexture = loadTexture(textFile);

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

			//get all the vertex normals
			else if (label == "vn") {
				glm::vec3 norm;
				ss >> norm.x >> norm.y >> norm.z;
				norm = glm::normalize(norm);
				normalp.push_back(norm);
			}

			//get the triangle surfaces
			else if (label == "f") {
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
				triangles.push_back(triangle);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	objFile.close();

	cout << "initing" << endl;
	std::vector<glm::vec3>::iterator it = points.begin();
	//centering
	float xPos = 0, xNeg = 0, yPos = 0, yNeg = 0, zPos = 0, zNeg = 0;;
	while (it != points.end()) {
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
		it++;
	}

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(), points.data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal.size(), normal.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	if (hasTexture) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textCoord.size(), textCoord.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	}

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triangles.size(), triangles.data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	cout << "projectile element initialized\n";
}

void ProjectileElement::draw(glm::mat4 c) {
	cout << "drawing projectile element \n";
	//model used in the shader would be this model mult with passed down transform model
	glm::mat4 m = model * c;
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(*view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(*projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(m));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(eyep));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	// Bind the VAO
	glBindVertexArray(VAO);

	if (hasTexture) {
		//cout << "has texture" << endl;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textId);
	}

	// Draw the points 
	//glDrawArrays(GL_POINTS, 0, points.size());
	glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ProjectileElement::update() {

}

void ProjectileElement::updateView(glm::mat4 proj, glm::vec3) {

}

bool ProjectileElement::loadTexture(char* texturePath) {
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
	unsigned char* data = stbi_load(texturePath, &ftw, &fth, &channels, 3);
	if (data == NULL) {
		cout << "cannot load texture at " << texturePath << endl;
		return false;
	}
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ftw, fth, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
	return true;
}