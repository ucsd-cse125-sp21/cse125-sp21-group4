#include "Ground.h"
#include "stb_image.h"


/*
	since ground.obj has width TILE_SIZE, actual size displayed is tileScale times TILE_SIZE
*/
Ground::Ground(string fileName, glm::mat4* proj, glm::mat4* v, GLuint s, char * textFile, 
	SpatialHashTable * table, float tileScale) {
	// initial translation will bthe initial position
	projection = proj;
	view = v;
	shader = s;

	cout << "loading ground texture --" << textFile << endl;
	hasTexture = loadTexture(textFile);
	if (!hasTexture) {
		cout << "CANNOT load Ground Texture, please check texture file" << endl;
		return;
	}

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

	int i, j;
	int width = GROUND_WIDTH_IN_TILES / TILE_SIZE / tileScale;
	int length = GROUND_WIDTH_IN_TILES / TILE_SIZE / tileScale;
	cout << "map width: " << width << endl;
	float tileSize = 2.f * tileScale;
	for (i = 0; i < width; i++) {
		for (j = 0; j < length; j++) {
			//all the texture, points, normals, textCoords are shared between all tiles
			GroundTile* g = new GroundTile(proj, v, s, glm::vec3(i * tileSize, 0, j * tileSize), tileScale,
				textId, &normal, &points, &triangles, &texp);
			table->insert(g);
		}
	}
	cout << "ground tiles initialized" << endl;
}

void Ground::update() {

}

bool Ground::loadTexture(char* texturePath) {
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

	stbi_set_flip_vertically_on_load(true);
	int ftw, fth, channels;
	unsigned char* data = stbi_load(texturePath, &ftw, &fth, &channels, 4);
	if (data == NULL) {
		cout << "cannot load texture at " << texturePath << endl;
		return false;
	}
	glGenTextures(1, &textId);
	glBindTexture(GL_TEXTURE_2D, textId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ftw, fth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	fclose(f);
	return true;
}