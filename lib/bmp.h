#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "MyContainer.h"
#include "ArgumentsParser.h"

struct Color {
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color();
	Color(unsigned char r, unsigned char g, unsigned char b);
	~Color();

};

void ToImage(uint64_t**& grid, const std::string& path, uint16_t width, uint16_t height);

