#pragma once

#include <glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

unsigned int compileShader(unsigned int type, const std::string& source);

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
unsigned int createShader(const std::string& computeShader);

std::string loadShader(std::string path);