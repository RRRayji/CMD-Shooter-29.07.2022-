#ifndef __GLOBALS_H__
#define __GLOBALS_H__

const unsigned short OBJECTS[2] = { '#', ' '};

bool RUN = 1;
const unsigned short delay = 130;	// normal is 130
const unsigned short PLAYERS_COUNT = 2;

constexpr short MAP_SIZE[2] = { 32, 16 };	// x, y

string* MAP_BUFFER = new string[16]{
	"################################",
	"#           #                  #",
	"#           #      #           #",
	"#   ####    #      #    ####   #",
	"#   #          #####       #   #",
	"#   #                      #   #",
	"#          ##                  #",
	"#          #        ##         #",
	"#         ##        #          #",
	"#                  ##          #",
	"#   #                      #   #",
	"#   #       #####          #   #",
	"#   ####    #      #    ####   #",
	"#           #      #           #",
	"#                  #           #",
	"################################",
};

unsigned short MAP[MAP_SIZE[1]][MAP_SIZE[0]];	// y, x

#endif