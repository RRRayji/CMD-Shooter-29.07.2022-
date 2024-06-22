#include "classes.h"
#include "globals.h"
#include "lib.h"

// ----    LOCAL FUNCTIONS    ----

enum keys {
	w = 87, up = 38,
	a = 65, left = 37,
	s = 83, down = 40,
	d = 68, right = 39
};

inline void Get_direction(short& x, short& y, const unsigned short& direction)
{
	if (direction == keys::w || direction == keys::up)
		--y;
	else if (direction == keys::d || direction == keys::right)
		++x;
	else if (direction == keys::s || direction == keys::down)
		++y;
	else if (direction == keys::a || direction == keys::left)
		--x;
}

inline void Map_random(short& x, short& y)
{
	while (MAP[y][x] == OBJECTS[0])
	{
		x = XorShift128() % (MAP_SIZE[0] - 4) + 2;
		y = XorShift128() % (MAP_SIZE[1] - 4) + 2;
	}
}

void Reset_map()	// Глоток воздуха в забагованной карте
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
	for (unsigned short y = 0; y < MAP_SIZE[1]; y++)
	{
		for (unsigned short x = 0; x < MAP_SIZE[0]; x++)
		{
			putchar(MAP[y][x]);
		}
		putchar('\n');
	}
}

// ----    LOCAL FUNCTIONS' END    ----

// ----    GETTERS    ----

short Player::Get_x()
{
	return x;
}

short Player::Get_y()
{
	return y;
}

short Weapon::Bullet::Get_x()
{
	return x;
}

short Weapon::Bullet::Get_y()
{
	return y;
}

// ----    GETTERS' END    ----

// ----    SETTERS    ----

void Player::Set_position(short x, short y)
{
	this->x = x;
	this->y = y;
}

// ----    SETTERS' END    ----

unsigned short Player::id = 0;

Player::Player(unsigned short model, unsigned short up, unsigned short right, unsigned short down, unsigned short left, unsigned short shooting)
{
	id++;
	this->model = model;
	control_keys[0] = up;
	control_keys[1] = right;
	control_keys[2] = down;
	control_keys[3] = left;
	control_keys[4] = shooting;
	last_used_key = shooting;
	Map_random(x, y);
	player_weapon = nullptr;
	Display();
	player_thread = thread(&Player::Control, this);
};

Player::~Player()
{
	if (player_thread.joinable())
	{
		player_thread.join();
	}
}

unsigned short Player::Get_id()
{
	return id;
}

void Player::Control()
{
	while (RUN)
	{
		if (GetAsyncKeyState(control_keys[0]) & 0x8000 && MAP[y - 1][x] != OBJECTS[0])		// UP
		{
			last_used_key = control_keys[0];
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y-- });
			putchar(OBJECTS[1]);
			Display();
		}
		else if (GetAsyncKeyState(control_keys[1]) & 0x8000 && MAP[y][x + 1] != OBJECTS[0])	// RIGHT
		{
			last_used_key = control_keys[1];
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x++,y });
			putchar(OBJECTS[1]);
			Display();
		}
		else if (GetAsyncKeyState(control_keys[2]) & 0x8000 && MAP[y + 1][x] != OBJECTS[0])	// DOWN
		{
			last_used_key = control_keys[2];
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y++ });
			putchar(OBJECTS[1]);
			Display();
		}
		else if (GetAsyncKeyState(control_keys[3]) & 0x8000 && MAP[y][x - 1] != OBJECTS[0])	// LEFT
		{
			last_used_key = control_keys[3];
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x--,y });
			putchar(OBJECTS[1]);
			Display();
		}
		else if (GetAsyncKeyState(control_keys[4]) & 0x8000 && player_weapon != nullptr && last_used_key != control_keys[4])
		{
			Use();
		}
		else if (GetAsyncKeyState(8) & 0x8000) 
		{
			Reset_map();
		}
		Sleep(delay);
	}
}

void Player::Display()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
	putchar(model);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,MAP_SIZE[1] });
}

void Player::Pick_up_weapon(Weapon* weapon)
{
	player_weapon = *&weapon;
}

void Player::Weapon_position_update()
{
	player_weapon->x = x;
	player_weapon->y = y;
}

void Player::Use()
{
	Weapon_position_update();
	player_weapon->direction = last_used_key;
	player_weapon->Use();
}

Weapon::Bullet::Bullet(short x, short y)
{
	this->x = x;
	this->y = y;
}

Weapon::Bullet::~Bullet()
{
	if (bullet_thread.joinable())
	{
		bullet_thread.join();
	}
}

void Weapon::Bullet::Display()
{
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
	putchar(model);
}

void Weapon::Bullet::Move(const unsigned short& direction)
{
	while (true)
	{
		if (MAP[y][x] == OBJECTS[0])
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
			putchar(OBJECTS[0]);
			break;
		}
		else
		{
			Sleep(70);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { x,y });
			putchar(OBJECTS[1]);
			Get_direction(x, y, direction);
			Display();
		}
	}
}

/*

Weapon::Weapon()
{
	Map_random(x, y);
}
*/

void Gun::Use()
{
	if (ammunition == 0)
	{
		ammunition = 1;
		Bullet gun_bullet(Gun::x, Gun::y); // ( x,y );
		Get_direction(gun_bullet.x, gun_bullet.y, direction);
		gun_bullet.Display();

		gun_bullet.Move(direction);

		ammunition = 0;
	}
}

Game::Game()
{
	Game::Map_render();
	
	Weapon* p1_w;
	Gun gun;
	p1_w = &gun;

	Weapon* p2_w;
	Gun Gun2;
	p2_w = &Gun2;

	Player A(keys::a, keys::w, keys::d, keys::s, keys::a, 160);
	A.Pick_up_weapon(p1_w);

	Player B(66, keys::up, keys::right, keys::down, keys::left, 161);
	B.Pick_up_weapon(p2_w);



	while (RUN)
	{
		if (GetAsyncKeyState(27) & 0x8000 && RUN == 1)
		{
			RUN = 0;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,short(MAP_SIZE[1]) });
		}
	};

};

void Game::Map_render()
{
	for (unsigned short y = 0; y < MAP_SIZE[1]; y++)
	{
		for (unsigned short x = 0; x < MAP_SIZE[0]; x++)
		{
			MAP[y][x] = MAP_BUFFER[y][x];
			putchar(MAP[y][x]);
		}
		putchar(10);
	}
	delete[]MAP_BUFFER;
	MAP_BUFFER = nullptr;
}