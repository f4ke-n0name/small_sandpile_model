#include "ArgumentsParser.h"

void Arguments::PrintError(char* type_error) {
	std::cerr << type_error;
	exit(EXIT_FAILURE);
}

void Arguments::GetMaxIterArgument(int argc, char* argv[], int& iterator) {
	char* pEnd = nullptr;
	const char* short_argument = "-m";
	const char* long_argument = "--max-iter=";
	int64_t result = 0;
	const bool range_error = errno == ERANGE;
	if (iterator + 1 < argc && !strcmp(argv[iterator], short_argument)) {
		result = strtol(argv[iterator + 1], &pEnd, 10);
		++iterator;
		if (argv[iterator + 1] == pEnd || range_error) {
			PrintError("The -m and --max-iterator arguments require correctly entered arguments");
		}
		max_iter = result;
	}
	else if (!strncmp(argv[iterator], long_argument, kLenMaxIter)) {
		result = _strtoi64(argv[iterator] + kLenMaxIter, &pEnd, 10);
		if (argv[iterator] + kLenMaxIter == pEnd || range_error) {
			PrintError("The -m and --max-iterator arguments require correctly entered arguments");
		}
		max_iter = result;
	}
}

void Arguments::GetFreqArgument(int argc, char* argv[], int& iterator) {
	char* pEnd = nullptr;
	const char* short_argument = "-f";
	const char* long_argument = "--freq=";
	int64_t result = 0;
	const bool range_error = errno == ERANGE;
	if (iterator + 1 < argc && !strcmp(argv[iterator], short_argument)) {
		result = strtol(argv[iterator + 1], &pEnd, 10);
		++iterator;
		if (argv[iterator + 1] == pEnd || range_error) {
			PrintError("The -f and --freq arguments require correctly entered arguments");
		}
		freq = result;
	}
	else if (!strncmp(argv[iterator], long_argument, kLenFreq)) {
		result = strtol(argv[iterator] + kLenFreq, &pEnd, 10);
		if (argv[iterator] + kLenFreq == pEnd || range_error) {
			PrintError("The -f and --freq arguments require correctly entered arguments");
		}
		freq = result;
	}
}

void Arguments::GetFilename(int argc, char* argv[], int& iterator) {
	char* pEnd = nullptr;
	const char* short_argument = "-i";
	const char* long_argument = "--input=";
	if (iterator + 1 < argc && !strcmp(argv[iterator], short_argument)) {
		if (argv[iterator + 1] == pEnd) {
			PrintError("The -i and --input arguments require correctly entered arguments");
		}
		filename = argv[iterator + 1];
		++iterator;
	}
	else if (!strncmp(argv[iterator], long_argument, kLenFilename)) {
		if (argv[iterator] + kLenFilename == pEnd) {
			PrintError("The -i and --input arguments require correctly entered arguments");
		}
		filename = argv[iterator] + kLenFilename;
	}
}

void Arguments::GetOutputPath(int argc, char* argv[], int& iterator) {
	char* pEnd = nullptr;
	const char* short_argument = "-o";
	const char* long_argument = "--output=";
	if (!strcmp(argv[iterator], short_argument) && iterator + 1 < argc) {
		if (argv[iterator + 1] == pEnd) {
			PrintError("The -0 and --output arguments are not provided with data");
		}
		output_path = argv[iterator + 1];
		++iterator;
	}
	else if (!strncmp(argv[iterator], long_argument, kLenOutputPath)) {
		if (argv[iterator] + kLenOutputPath == pEnd) {
			PrintError("The -o and --output arguments are not provided with data");
		}
		output_path = argv[iterator] + kLenOutputPath;
	}
}

void Arguments::GetGridSize() {
	if (filename != "") {
		input.open(filename);
		int16_t x_max = 0;
		int16_t y_max = 0;
		int16_t x_min = 0;
		int16_t y_min = 0;
		int16_t x_new;
		int16_t y_new;
		uint64_t value;
		while (!input.eof()) {
			input >> x_new >> y_new >> value;
			x_max = ((x_new > 0) ? std::max(x_max, x_new) : x_max);
			x_min = ((x_new <= 0) ? std::min(x_min, x_new) : x_min);
			y_max = ((y_new > 0) ? std::max(y_max, y_new) : y_max);
			x_max = ((y_new <= 0) ? std::min(y_min, y_new) : y_min);
		}
		height = (x_max - x_min) + 1;
		width = (y_max - y_min) + 1;
		padding_width = (0 - y_min == 0) ? 0 : 0 - y_min + 1;
		padding_height = (0 - x_min == 0) ? 0 : 0 - x_min + 1;
	}
}

void Arguments::GetArguments(int argc, char* argv[]) {
	for (int i = 1; i < argc; ++i) {
		GetMaxIterArgument(argc, argv, i);
		GetFreqArgument(argc, argv, i);
		GetFilename(argc, argv, i);
		GetOutputPath(argc, argv, i);
		GetGridSize();
	}
}