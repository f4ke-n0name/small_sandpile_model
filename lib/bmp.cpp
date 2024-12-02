#include "bmp.h"

Color::Color() {
  this->r = 0;
  this->b = 0;
  this->g = 0;
}

Color::Color(unsigned char r, unsigned char g, unsigned char b) {
  this->r = r;
  this->g = g;
  this->b = b;
}

Color::~Color() {
}

const Color ColorWhite = Color(255, 255, 255);
const Color ColorGreen = Color(0, 255, 0);
const Color ColorYellow = Color(255, 255, 0);
const Color ColorPurple = Color(102, 0, 153);
const Color ColorBlack = Color(0, 0, 0);

const int FileHeaderSize = 14;
const int InformationHeaderSize = 40;
const int TotalColors = 5;
const int BitsPerPixel = 4;
const int ColorBytes = 4;
const int ColorPalleteSize = ColorBytes * TotalColors;

void ToImage(uint64_t**& grid, const std::string& output_path, uint16_t width, uint16_t height) {
	std::ofstream f;
	f.open(output_path, std::ios::out | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "File couldn't be opened\n";
		return;
	}

    const uint16_t paddingAmount = (8 - (width % 8)) % 8;
    const uint64_t full_width = width + paddingAmount;

    const uint64_t fileSize = FileHeaderSize + InformationHeaderSize + ColorPalleteSize + (full_width * height) / 2;
    uint8_t fileHeader[FileHeaderSize] = {};
    uint8_t informationHeader[InformationHeaderSize] = {};

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // Reserved 1 (Not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // Reserved 2 (Not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel data offset
    fileHeader[10] = FileHeaderSize + InformationHeaderSize + ColorPalleteSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    // Header size
    informationHeader[0] = InformationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // Image width
    informationHeader[4] = width;
    informationHeader[5] = width >> 8;
    informationHeader[6] = width >> 16;
    informationHeader[7] = width >> 24;
    // Image height
    informationHeader[8] = height;
    informationHeader[9] = height >> 8;
    informationHeader[10] = height >> 16;
    informationHeader[11] = height >> 24;
    // Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    // Bits per pixel (RGB)
    informationHeader[14] = BitsPerPixel;
    informationHeader[15] = 0;
    // Total colors
    informationHeader[32] = TotalColors;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;

    uint8_t colorPallete[ColorPalleteSize] = {};
    //0 - White
    colorPallete[0] = ColorWhite.b;
    colorPallete[1] = ColorWhite.g;
    colorPallete[2] = ColorWhite.r;
    colorPallete[3] = 0;
    //1 - Green
    colorPallete[4] = ColorGreen.b;
    colorPallete[5] = ColorGreen.g;
    colorPallete[6] = ColorGreen.r;
    colorPallete[7] = 0;
    //2 - Yellow
    colorPallete[8] = ColorYellow.b;
    colorPallete[9] = ColorYellow.g;
    colorPallete[10] = ColorYellow.r;
    colorPallete[11] = 0;
    //3 - Purple
    colorPallete[12] = ColorPurple.b;
    colorPallete[13] = ColorPurple.g;
    colorPallete[14] = ColorPurple.r;
    colorPallete[15] = 0;
    //4 - Black
    colorPallete[16] = ColorBlack.b;
    colorPallete[17] = ColorBlack.g;
    colorPallete[18] = ColorBlack.r;
    colorPallete[19] = 0;
   
    f.write(reinterpret_cast<char*>(fileHeader), FileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), InformationHeaderSize);
    f.write(reinterpret_cast<char*>(colorPallete), ColorPalleteSize);

    uint64_t first_color, second_color;
    uint8_t total_color;
    for (int64_t coord_x = height - 1; coord_x >= 0; coord_x--) {
        for (int64_t coord_y = 0; coord_y < full_width; coord_y += 2) {
            if (coord_y + 1 >= width) {
                first_color = grid[coord_x][coord_y];
                second_color = 0;
            } else if (coord_y >= width) {
                first_color = 0;
                second_color = 0;
            } else {
                first_color = grid[coord_x][coord_y];
                second_color = grid[coord_x][coord_y + 1];
            }
            if (first_color >= 4) {
                first_color = 4;
            }
            if (second_color >= 4) {
                second_color = 4;
            }

            total_color = (first_color << BitsPerPixel);
            total_color += second_color;

            f << total_color;
        }
    }
    f.close();
}
