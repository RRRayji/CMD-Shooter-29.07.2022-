#include "lib.h"

struct seed_t
{
	unsigned x = 69; // начальные значения могут быть любыми
	unsigned y = 445;
	unsigned z = 665;
	unsigned w = 21;
};

unsigned XorShift128()
{
static seed_t s;
unsigned t = s.x ^ (s.x << 11);
s.x = s.y;
s.y = s.z;
s.z = s.w;
s.w = (s.w ^ (s.w >> 19)) ^ (t ^ (t >> 8));
return s.w;
}

void gotoxy(short int x, short int y) {
	COORD coordinates = { x, y };
	HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(outputHandle, coordinates);
}