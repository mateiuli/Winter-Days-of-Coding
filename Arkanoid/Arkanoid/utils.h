#pragma once
#include <SFML/Graphics.h>

typedef enum collision_t
{
	NO_COLLISION,
	FROM_LEFT,
	FROM_RIGHT,
	FROM_TOP,
	FROM_BOTTOM
} collision_t;

collision_t get_collision(sfRectangleShape *rectangle, sfCircleShape *ball);