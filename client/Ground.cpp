#include "Ground.h"


/*
	since ground.obj has width TILE_SIZE, actual size displayed is tileScale times TILE_SIZE
*/
Ground::Ground(string fileName, glm::mat4* proj, glm::mat4* v, GLuint s, char * grassTextFile, 
char * crackedTileTextFile, float tileScale) {

	cout << "creating ground" << endl;

	int i, j;
	const int width = GROUND_WIDTH / TILE_SIZE;
	const int length = GROUND_WIDTH / TILE_SIZE;

	cout << "map width: " << width << endl;
	float tileSize = 2.f * tileScale;
	cout << "creating a " << width << " by " << length << " ground"<< endl;

	// Get data from csv to tell what tiles to draw for specific position.
	int initTiles [width][length];

    ifstream map_file("../assets/layout/map_ground.csv");
    string line;
    string id;

    i = 0;
    j = 0;
    while(getline(map_file, line)) {
        stringstream ss(line);
         
        while(getline(ss, id, ',')) {
			// j = x, i = y.
            int objID = std::stoi(id);

			// populate the models for each tiles
			switch(objID) {
				case DRY_GRASS_ID: {
					grassTileTrans.push_back(glm::vec3(j* tileSize, 0, i* tileSize));
					break;
				}
				case CRACKED_TILE_ID: {
					roadTileTrans.push_back(glm::vec3(j* tileSize, 0, i* tileSize));
					break;
				}
			}
            j++;
        }
        j = 0;
        i++;
    }

	//build the ground
	roadTiles = new GroundTile("shaders/environment/ground.obj", crackedTileTextFile, proj, v, s,
		tileScale, &roadTileTrans);
	grassTiles = new GroundTile("shaders/environment/ground.obj", grassTextFile, proj, v, s,
		tileScale, &grassTileTrans);

	cout << "ground tiles initialized" << endl;
}

void Ground::update() {

}


/*
	draw the ground instances
*/
void Ground::draw() {
	//cout << "drawing ground" << endl;
	grassTiles->draw();
	roadTiles->draw();
}
