#include <lib/ArgumentsParser.h>
#include <lib/sandpile.h>
#include <lib/MyContainer.h>


int main(int argc, char* argv[]) {
	Arguments arguments;
	arguments.GetArguments(argc, argv);
	uint64_t** grid = new uint64_t * [arguments.height];
	for (uint16_t i = 0; i < arguments.height; ++i) {
		grid[i] = new uint64_t[arguments.width]{ 0 };
	}
	SetGrains(grid, arguments);
	Collapse(grid, arguments);
	return 0;
}