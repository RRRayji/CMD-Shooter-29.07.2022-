#ifndef __CLASSES_H__
#define __CLASSES_H__
#include <thread>
#include <string>
using namespace std;

class Weapon;
class Gun;

class Player
{
public:
	Player(unsigned short model, unsigned short up, unsigned short right, unsigned short down, unsigned short left, unsigned short shooting);
	~Player();

	void Pick_up_weapon(Weapon* weapon);
	void Weapon_position_update();
	short Get_x();
	short Get_y();

protected:
	std::thread player_thread;
	short x, y;	// position
	unsigned short model, last_used_key, control_keys[5];	// 0..4 = up, r, d, l, shift as shooting
	Weapon* player_weapon;
	static unsigned short id;

	void Control();
	void Set_position(short x, short y);
	void Display();
	void Use();
	static unsigned short Get_id();
	friend Weapon;
};

class Game
{
public:
	Game();
private:
	void Map_render();
};

class Weapon
{
protected:
	class Bullet
	{
	public:
		Bullet(short x, short y);
		~Bullet();

		short x, y;
		unsigned short model = 45;
		thread bullet_thread;

		short Get_x();
		short Get_y();
		void Display();
		void Move(const unsigned short& direction);
	};

	//Weapon();					// POSITION OF WEAPON - NOT BULLET
	short x = 0, y = 0;
	unsigned short direction;

	virtual void Use() = 0;
	//void Display();			// DISPLAY WEAPON POSITION - NOT BULLET

	friend Player;
};

class Gun : public Weapon
{
protected:
	bool ammunition = 0;
	unsigned short model = 'P';
	void Use() override;
};

#endif