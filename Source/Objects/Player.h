#pragma once

#define PLAYER_MAX_HEALTH 8
#define PLAYER_MAX_AMMO 8

#define PLAYER_SPEED 200   
#define TURN_SPEED 150 

extern float PLAYER_X;
extern float PLAYER_Y;
extern float PLAYER_DELTA_X;
extern float PLAYER_DELTA_Y;
extern float PLAYER_ANGLE;

class Player
{
private:
	int health;
	int ammo;

public:
	Player();

	int GetHealth();
	int GetAmmo();
};