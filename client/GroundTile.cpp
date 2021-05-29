#include "GroundTile.h"

GroundTile::GroundTile(glm::mat4* proj, glm::mat4* v, GLuint s, glm::vec3 trans,
	float scale, GLuint tid, vector<glm::vec3> * n, vector<glm::vec3> * p,
	vector<glm::ivec3> * tri, vector<glm::vec2> * tc) {
	
	projection = proj;
	view = v;
	shader = s;
	model = glm::translate(trans) * glm::scale(glm::vec3(scale));
	textId = tid;
	normal = n;
	points = p;
	triangles = tri;
	textCoord = tc;
	triangleSize = tri->size();
	pos = trans;

	// Generate a Vertex Array (VAO) and Vertex Buffer Object (VBO)
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO to the bound VAO, and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points->size(), points->data(), GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal->size(), normal->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * textCoord->size(), textCoord->data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * triangles->size(), triangles->data(), GL_STATIC_DRAW);

	// Unbind the VBO/VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GroundTile::draw(glm::mat4 c) {
	//model used in the shader would be this model mult with passed down transform model
	glm::mat4 m = model * c;
	glUseProgram(shader);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(*view));
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(*projection));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false, glm::value_ptr(m));
	//glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(eyep));

	// Bind the VAO
	glBindVertexArray(VAO);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textId);
	glDisable(GL_BLEND);

	glDrawElements(GL_TRIANGLES, 3 * triangleSize, GL_UNSIGNED_INT, 0);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GroundTile::drawIfNotObstructing(glm::vec3 clientPos, glm::mat4 c) {
	draw();
}

void GroundTile::update() {

}

void GroundTile::updateView(glm::mat4, glm::vec3) {
	
}