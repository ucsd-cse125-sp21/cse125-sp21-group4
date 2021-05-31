#include "EnvElement.h"
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

EnvElement::EnvElement(string fileName, glm::mat4 * p, glm::mat4 * v, GLuint s, glm::vec3* eyePos,
	glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale, glm::vec3 c, char* textFile) {
	
	// initial translation will bthe initial position
	pos = trans;
	model = glm::translate(trans) * glm::rotate(rotRad, rotAxis)  * glm::scale(glm::vec3(scale));
	projection = p;
	view = v;
	shader = s;
	this->eyep = eyePos;
	//default color is black
	color = c;
	// if path is NOT given at construction time, hasTexture will be false.
	hasTexture = loadTexture(textFile);
	hasMaterial = false; // will be loaded at another time.

	std::vector<glm::vec3> normalp;
	std::vector<glm::vec3> pointsp;
	std::vector<glm::vec2> texp;
	string delim = "/";
	std::ifstream objFile(fileName);
	int triangleCountPerMesh = 0;
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

				triangleCountPerMesh++;
				
			} 
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	materialSize.push_back(triangleCountPerMesh);
	objFile.close();

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

EnvElement::EnvElement(string fileName, glm::mat4 * p, glm::mat4 * v, GLuint s, glm::vec3* vPos, 
	glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale, MaterialManager* matManager, glm::vec3 c) {
	
	
	// initial translation will bthe initial position
	pos = trans;
	model = glm::translate(trans) * glm::rotate(rotRad, rotAxis)  * glm::scale(glm::vec3(scale));
	projection = p;
	view = v;
	shader = s;
	//default color is black
	color = c;
	eyep = vPos;
	// if path is NOT given at construction time, hasTexture will be false.
	// hasTexture = loadTexture(textFile);
	hasTexture = true;
	hasMaterial = false; // will be loaded at another time.

	std::vector<glm::vec3> normalp;
	std::vector<glm::vec3> pointsp;
	std::vector<glm::vec2> texp;
	string delim = "/";
	std::ifstream objFile(fileName);
	int triangleCountPerMesh = 0;
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

				triangleCountPerMesh++;
				
			} else if (label == "usemtl") {
				std::string materialName;
				ss >> materialName;
				
				if(hasMaterial) {
					materials.push_back(materialList->at(materialName));
				}

				materialSize.push_back(triangleCountPerMesh);
				triangleCountPerMesh = 0;
			}

			else if (label == "mtllib") {
				std::string materialName;
				ss >> materialName;
				int handle = matManager->loadMaterial(fileName.substr(0, fileName.find_last_of('/') + 1) + materialName);
				hasMaterial = handle >= 0;
				if(hasMaterial) {
					setMaterialList(matManager->getMaterialList(handle));
				}
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << fileName << std::endl;
	}

	materialSize.push_back(triangleCountPerMesh);;
	objFile.close();

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


// EnvElement::EnvElement(string fileName, glm::mat4 * p, glm::mat4 * v, GLuint s, 
// 	glm::vec3 trans, glm::vec3 rotAxis, float rotRad, float scale, MaterialManager* matManager, glm::vec3 c) {
	
// 	// initial translation will bthe initial position
// 	pos = trans;
// 	model = glm::translate(trans) * glm::rotate(rotRad, rotAxis)  * glm::scale(glm::vec3(scale));
// 	projection = p;
// 	view = v;
// 	shader = s;
// 	//default color is black
// 	color = c;
// 	// if path is NOT given at construction time, hasTexture will be false.
// 	hasMaterial = false; // will be loaded at another time.

// 	std::vector<glm::vec3> normalp;
// 	std::vector<glm::vec3> pointsp;
// 	std::vector<glm::vec2> texp;

// 	std::vector<Vertex> temp_vertices;
// 	std::vector<glm::ivec3> temp_triangles;
	
// 	std::vector<Material> materials;
// 	std::vector<int> materialIndex;

// 	int matIndex = 0;

// 	string delim = "/";
// 	std::ifstream objFile(fileName);
// 	// Check whether the file can be opened.
// 	if (objFile.is_open())
// 	{
// 		std::string line; // A line in the file.
// 		// Read lines from the file.
// 		int vCount = 0;

// 		while (std::getline(objFile, line))
// 		{
// 			std::stringstream ss;
// 			ss << line;
// 			std::string label;
// 			ss >> label;
// 			if (label == "v")
// 			{
// 				glm::vec3 point;
// 				ss >> point.x >> point.y >> point.z;
// 				pointsp.push_back(point);
// 			}

// 			if (label == "vt")
// 			{
// 				glm::vec2 tex;
// 				ss >> tex.x >> tex.y;
// 				texp.push_back(tex);
// 			}

// 			//get all the vertex normals
// 			else if (label == "vn") {
// 				glm::vec3 norm;
// 				ss >> norm.x >> norm.y >> norm.z;
// 				norm = glm::normalize(norm);
// 				normalp.push_back(norm);
// 			}

// 			//get the triangle surfaces
// 			else if (label == "f") {
// 				glm::ivec3 triangle;
// 				std::string a, b, c;
// 				ss >> a >> b >> c;

// 				points.push_back(pointsp[stoi(a.substr(0, a.find(delim))) - 1]);
// 				a = a.substr(a.find(delim) + 1, string::npos);
// 				points.push_back(pointsp[stoi(b.substr(0, b.find(delim))) - 1]);
// 				b = b.substr(b.find(delim) + 1, string::npos);
// 				points.push_back(pointsp[stoi(c.substr(0, c.find(delim))) - 1]);
// 				c = c.substr(c.find(delim) + 1, string::npos);

// 				textCoord.push_back(texp[stoi(a.substr(0, a.find(delim))) - 1]);
// 				a = a.substr(a.find(delim) + 1, string::npos);
// 				textCoord.push_back(texp[stoi(b.substr(0, b.find(delim))) - 1]);
// 				b = b.substr(b.find(delim) + 1, string::npos);
// 				textCoord.push_back(texp[stoi(c.substr(0, c.find(delim))) - 1]);
// 				c = c.substr(c.find(delim) + 1, string::npos);
				

// 				normal.push_back(normalp[stoi(a.substr(a.find(delim) + 1, string::npos)) - 1]);
// 				normal.push_back(normalp[stoi(b.substr(b.find(delim) + 1, string::npos)) - 1]);
// 				normal.push_back(normalp[stoi(c.substr(c.find(delim) + 1, string::npos)) - 1]);

// 				triangle.x = vCount++;
// 				triangle.y = vCount++;
// 				triangle.z = vCount++;
// 				triangles.push_back(triangle);
// 			}

// 			else if (label == "usemtl") {

// 				std::string materialName;
// 				ss >> materialName;
// 				materials.push_back(materialList->at(materialName));
// 				materialIndex.push_back(triangles.size());
	
// 			}

// 			else if (label == "mtllib") {
// 				std::string materialName;
// 				ss >> materialName;
// 				int handle = matManager->loadMaterial(fileName.substr(0, fileName.find_last_of('/') + 1) + materialName);
// 				hasMaterial = handle >= 0;
// 				if(hasMaterial) {
// 					setMaterialList(matManager->getMaterialList(handle));
// 				}
// 			}
// 		}
// 	}
// 	else
// 	{
// 		std::cerr << "Can't open the file " << fileName << std::endl;
// 	}

// 	objFile.close();

// 	std::vector<glm::vec3>::iterator it = points.begin();
// 	//centering
// 	float xPos = 0, xNeg = 0, yPos = 0, yNeg = 0, zPos = 0, zNeg = 0;;
// 	while (it != points.end()) {
// 		if ((*it).x > xPos)
// 			xPos = (*it).x;
// 		if ((*it).x < xNeg)
// 			xNeg = (*it).x;
// 		if ((*it).y > yPos)
// 			yPos = (*it).y;
// 		if ((*it).y < yNeg)
// 			yNeg = (*it).y;
// 		if ((*it).z > zPos)
// 			zPos = (*it).z;
// 		if ((*it).z < zNeg)
// 			zNeg = (*it).z;
// 		it++;
// 	}
// 	float xCenter = (xPos + xNeg) / 2;
// 	float yCenter = (yPos + yNeg) / 2;
// 	float zCenter = (zPos + zNeg) / 2;
// 	it = points.begin();
// 	while (it != points.end()) {
// 		(*it).x -= xCenter;
// 		(*it).y -= yCenter;
// 		(*it).z -= zCenter;
// 		it++;
// 	}

// 	// At the end, append the triangle size to materialIndex so we know to stop there
// 	materialIndex.push_back(triangles.size());

// 	// Create the meshes
// 	for (int i = 0; i < materialIndex.size() - 1; i++) {

// 		// Index represents up to which triangle we should stop copying to (excluding that index)
// 		for (int j = materialIndex[i]; j < materialIndex[i + 1]; j++) {
// 			temp_triangles.push_back(triangles[j]);
// 			Vertex v1;
// 			Vertex v2;
// 			Vertex v3;
// 			v1.normal = normal[j];
// 			v2.normal = normal[j + 1];
// 			v3.normal = normal[j + 2];
// 			v1.point = points[j];
// 			v2.point = points[j + 1];
// 			v3.point = points[j + 2];
// 			v1.texCoord = textCoord[j];
// 			v2.texCoord = textCoord[j + 1];
// 			v3.texCoord = textCoord[j + 2];
// 			temp_vertices.push_back(v1);
// 			temp_vertices.push_back(v2);
// 			temp_vertices.push_back(v3);
// 		}

// 		// Copy all the stuff to the mesh.
// 		vector<Vertex> copy_vertices(temp_vertices);
// 		vector<glm::ivec3> copy_indices(temp_triangles);
// 		Mesh mesh(copy_vertices, copy_indices, materials[i], model, view, projection, eyep, color);
// 		meshes.push_back(mesh);

// 		// Clear for next time
// 		// temp_vertices.clear();
// 		// temp_triangles.clear();
// 	}
// }



void EnvElement::draw(glm::mat4 c) {
	//model used in the shader would be this model mult with passed down transform model
	glm::mat4 m = model * c;
	glUseProgram(shader);

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

void EnvElement::drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c) {
	//model used in the shader would be this model mult with passed down transform model
	glm::mat4 m = model * c;
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(*view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(*projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(m));
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(*eyep));
	glUniform3fv(glGetUniformLocation(shader, "color"), 1, glm::value_ptr(color));

	//Bind the VAO
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);

	glDepthMask(GL_TRUE);

	glEnable(GL_BLEND);
	if (hasTexture) {
		// cout << "has texture" << endl;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textId);
	}

	// simple calculation to check if the object is blocking the character
	bool isCloseToObstructing = glm::distance(clientPos, pos) < 10.f && pos.z > clientPos.z && pos.z - clientPos.z > abs(pos.x - clientPos.x) && pos.y >= 3.f;
	if(isCloseToObstructing) {
		// printf("EnvElement is blocking the character.\n");
		glDepthMask(GL_FALSE);
		// glBlendColor(0, 0, 0, abs(glm::distance(clientPos, pos) - 5.f) * 2.f / 255.f);
		// glBlendFunc(GL_SRC_ALPHA, GL_CONSTANT_COLOR);
		// glUniform1f(glGetUniformLocation(shader, "alpha"), abs(glm::distance(clientPos, pos) - 5.f) / 255.f);
	} 
	// else {
	// 	glUniform1f(glGetUniformLocation(shader, "alpha"), 1.f);
	// }
	
	int sum = 0;
	for(int i = 0; i < materialSize.size(); i++) {
		if (i < materials.size()) {
			glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(*eyep));
			glUniform3fv(glGetUniformLocation(shader, "ambientColor"), 1, glm::value_ptr(materials[i].diffuse));
			glUniform3fv(glGetUniformLocation(shader, "diffuseFactor"), 1, glm::value_ptr(materials[i].diffuse));
			glUniform3fv(glGetUniformLocation(shader, "specColor"), 1, glm::value_ptr(materials[i].specular));
			glUniform1f(glGetUniformLocation(shader, "specHighlight"), materials[i].shininess);
			glUniform1f(glGetUniformLocation(shader, "dissolve"), materials[i].dissolve);
		}
		glDrawElements(GL_TRIANGLES, 3 * materialSize[i], GL_UNSIGNED_INT, (GLvoid*) (sizeof(GLuint) * sum));
		sum += 3*materialSize[i];
	}
	// glDrawElements(GL_TRIANGLES, 3 * triangles.size(), GL_UNSIGNED_INT, 0);
	
	if (isCloseToObstructing) {
		glDepthMask(GL_TRUE);
	}
	glDisable(GL_BLEND);
	// Draw the points 
	//glDrawArrays(GL_POINTS, 0, points.size());

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void EnvElement::update() {

}

void EnvElement::updateView(glm::mat4 proj, glm::vec3) {

}

bool EnvElement::loadTexture(char* texturePath) {
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
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ftw, fth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	fclose(f);
	return true;
}

void EnvElement::setMaterialList(std::map<std::string, Material> * materialListPtr) {
	materialList = materialListPtr;
	hasMaterial = true;
}