#include "sandpile.h"

Stack stack;

void Resize(uint64_t**& grid, Arguments& arguments) {
    arguments.height += 2;
    arguments.width += 2;
    uint64_t** new_grid = new uint64_t * [arguments.height]{};
    new_grid[0] = new uint64_t[arguments.width]{};
    new_grid[arguments.height - 1] = new uint64_t[arguments.width]{};
    for (int i = 1; i < arguments.height - 1; ++i) {
        new_grid[i] = new uint64_t[arguments.width]{};
        for (int j = 1; j < arguments.width - 1; ++j) {
            new_grid[i][j] = grid[i - 1][j - 1];
        }
    }
    grid = new_grid;
}

bool CheckForResize(uint64_t** grid, Arguments& arguments) {
    for (uint16_t i = 0; i < arguments.height; ++i) {
        if (grid[i][0] > 3 || grid[i][arguments.width - 1] > 3) {
            return true;
        }
    }
    for (uint16_t i = 0; i < arguments.width; ++i) {
        if (grid[0][i] > 3 || grid[arguments.height - 1][i] > 3) {
            return true;
        }
    }
    return false;
}

void ShiftGrains(uint64_t**& grid, uint16_t x_coord, uint16_t y_coord) {
    grid[x_coord][y_coord] -= 4;
    ++grid[x_coord + 1][y_coord];
    ++grid[x_coord - 1][y_coord];
    ++grid[x_coord][y_coord + 1];
    ++grid[x_coord][y_coord - 1];
}


void SetGrains(uint64_t**& grid, Arguments& arguments) {
    std::ifstream input_file;
    input_file.open(arguments.filename);
    int16_t x_coord;
    int16_t y_coord;
    uint64_t value;
    while (!input_file.eof()) {
        input_file >> x_coord >> y_coord >> value;
        x_coord += arguments.padding_height;
        y_coord += arguments.padding_width;
        grid[arguments.height - y_coord][x_coord - 1] = value;
    }
}

bool IsSandGrainsCollapse(uint64_t**& grid, Arguments& arguments) {
    bool isCollapsePossible = false;
    if (CheckForResize(grid, arguments)) {
        Resize(grid, arguments);
    }
    for (uint16_t x = 0; x < arguments.height; ++x) {
        for (uint16_t y = 0; y < arguments.width; ++y) {
            if (grid[x][y] > 3) {
                isCollapsePossible = true;
                stack.push(x, y);
            }
        }
    }
    while (!stack.empty()) {
        std::pair<uint16_t, uint16_t> coords = stack.pop();
        ShiftGrains(grid, coords.first, coords.second);
    }
    return isCollapsePossible;
}


void Collapse(uint64_t**& grid, Arguments& arguments) {
    uint64_t cur_iter = 0;
    bool is_last_condition = arguments.freq == 0;
    std::string full_path = arguments.output_path + "\\";
    std::string ext = ".bmp";
    if (!is_last_condition) {
        ToImage(grid, full_path + std::to_string(cur_iter / arguments.freq) + ext, arguments.width, arguments.height);
    }
    while ((cur_iter < arguments.max_iter) && IsSandGrainsCollapse(grid, arguments)) {
        ++cur_iter;
        if ((!is_last_condition) && (cur_iter % arguments.freq == 0)) {
            ToImage(grid, full_path + std::to_string(cur_iter / arguments.freq) + ext, arguments.width, arguments.height);
        }
    }

    if (is_last_condition) {
        ToImage(grid, full_path + "0" + ext, arguments.width, arguments.height);
    }
}
