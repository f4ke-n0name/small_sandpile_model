#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>

struct Arguments {
public:
	int64_t max_iter = 0;
	int64_t freq = 0;
	std::string filename = "";
	std::string output_path = "";
	uint16_t width = 0;
	uint16_t height = 0;
	uint16_t padding_width = 0;
	uint16_t padding_height = 0;
	std::ifstream input;
	void GetArguments(int argc, char* argv[]);
	const static int kLenFilename = 8;
	const static int kLenOutputPath = 9;
	const static int kLenMaxIter = 11;
	const static int kLenFreq = 7;
	void PrintError(char* type_error);
	void GetMaxIterArgument(int argc, char* argv[], int& iterator);
	void GetFreqArgument(int argc, char* argv[], int& iterator);
	void GetFilename(int argc, char* argv[], int& iterator);
	void GetOutputPath(int argc, char* argv[], int& iterator);
	void GetGridSize();
};