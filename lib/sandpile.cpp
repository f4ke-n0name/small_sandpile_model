#include "sandpile.h"

Stack stack;

void AddRightColumn(uint64_t**& grid, uint16_t& width, uint16_t& height) {
    uint64_t** new_grid = new uint64_t * [height];
    for (uint16_t i = 0; i < height; ++i) {
        new_grid[i] = new uint64_t[width + 1]{ 0 };
        for (uint16_t j = 0; j < width; ++j) {
            new_grid[i][j] = grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
    grid = new_grid;
    ++width;
}


void AddLeftColumn(uint64_t**& grid, uint16_t& width, uint16_t& height) {
    uint64_t** new_grid = new uint64_t * [height];
    for (uint16_t i = 0; i < height; ++i) {
        new_grid[i] = new uint64_t[width + 1]{ 0 };
        for (uint16_t j = 0; j < width; ++j) {
            new_grid[i][j + 1] = grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
    grid = new_grid;
    ++width;
}
void AddUpLine(uint64_t**& grid, uint16_t& width, uint16_t& height) {
    uint64_t** new_grid = new uint64_t * [height + 1];
    for (uint16_t i = 0; i < height; ++i) {
        new_grid[i + 1] = new uint64_t[width]{ 0 };
        for (uint16_t j = 0; j < width; ++j) {
            new_grid[i + 1][j] = grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
    new_grid[0] = new uint64_t[width]{ 0 };
    for (uint16_t j = 0; j < width; ++j) {
        new_grid[0][j] = 0;
    }
    grid = new_grid;
    ++height;
}

void AddDownLine(uint64_t**& grid, uint16_t& width, uint16_t& height) {
    uint64_t** new_grid = new uint64_t * [height + 1];
    for (uint16_t i = 0; i < height; ++i) {
        new_grid[i] = new uint64_t[width]{ 0 };
        for (uint16_t j = 0; j < width; ++j) {
            new_grid[i][j] = grid[i][j];
        }
        delete[] grid[i];
    }
    delete[] grid;
    new_grid[height] = new uint64_t[width]{ 0 };
    for (uint16_t j = 0; j < width; ++j) {
        new_grid[height][j] = 0;
    }

    grid = new_grid;
    ++height;
}

void Resize(uint64_t**& grid, uint16_t& width, uint16_t& height) {
    bool is_need_left_column = false;
    bool is_need_right_column = false;
    bool is_need_down_line = false;
    bool is_need_up_line = false;
    for (uint16_t i = 0; i < height; ++i) {
        if (grid[i][0] > 3) {
            is_need_left_column = true;
        }
    }
    for (uint16_t i = 0; i < width; ++i) {
        if (grid[0][i] > 3) {
            is_need_up_line = true;
        }
    }
    for (uint16_t i = 0; i < height; ++i) {
        if (grid[i][width - 1] > 3) {
            is_need_right_column = true;
        }
    }
    for (uint16_t i = 0; i < width; ++i) {
        if (grid[height - 1][i] > 3) {
            is_need_down_line = true;
        }
    }
    if (is_need_down_line) {
        AddDownLine(grid, width, height);
    }
    if (is_need_left_column) {
        AddLeftColumn(grid, width, height);
    }
    if (is_need_right_column) {
        AddRightColumn(grid, width, height);
    }
    if (is_need_up_line) {
        AddUpLine(grid, width, height);
    }
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
    uint16_t x_coord;
    uint16_t y_coord;
    uint64_t value;
    while (!input_file.eof()) {
        input_file >> x_coord >> y_coord >> value;
        x_coord += arguments.padding_height;
        y_coord += arguments.padding_width;
        grid[arguments.height - y_coord][x_coord - 1] = value;
    }
}

bool IsSandGrainsCollapse(uint64_t**& grid, uint16_t& height, uint16_t& width) {
    bool flag = false;
    Resize(grid, height, width);
    for (uint16_t x = 0; x < height; ++x) {
        for (uint16_t y = 0; y < width; ++y) {
            if (grid[x][y] > 3) {
                flag = true;
                stack.push(x, y);
            }
        }
    }
    while (!stack.empty()) {
        std::pair<uint16_t, uint16_t> coords = stack.pop();
        ShiftGrains(grid, coords.first, coords.second);
    }
    return flag;
}


void Collapse(uint64_t**& grid, Arguments& arguments) {
    uint64_t cur_iter = 0;
    bool is_last_condition = arguments.freq == 0;
    std::string full_path = arguments.output_path + "\\";
    std::string ext = ".bmp";
    if (!is_last_condition) {
        ToImage(grid, full_path + std::to_string(cur_iter / arguments.freq) + ext, arguments.width, arguments.height);
    }
    while ((cur_iter < arguments.max_iter) && IsSandGrainsCollapse(grid, arguments.height, arguments.width)) {
        ++cur_iter;
        if ((!is_last_condition) && (cur_iter % arguments.freq == 0)) {
            ToImage(grid, full_path + std::to_string(cur_iter / arguments.freq) + ext, arguments.width, arguments.height);
        }
    }

    if (is_last_condition) {
        ToImage(grid, full_path + "0" + ext, arguments.width, arguments.height);
    }
}
