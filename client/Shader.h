#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif
