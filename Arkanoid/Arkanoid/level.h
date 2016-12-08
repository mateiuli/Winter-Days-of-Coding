#pragma once
#include <SFML/Graphics.h>

#define MAX_COLS 32
#define MAX_ROWS 32

#define BRICK_WIDTH		40
#define BRICK_HEIGHT	25

typedef struct
{
	/* Numarul de linii de caramizi. */
	int rows;
	
	/* Numarul de coloane de caramizi. */
	int cols;
	
	/* Matricea cu caramizi - numarul de lovituri 
	 * pana la distrugerea unei caramizi. */
	int bricks_life[MAX_ROWS][MAX_COLS];

	/* Forma dreptunghiulara a fiecarei caramizi - forma grafica OpenGL. */
	sfRectangleShape *bricks_shape[MAX_ROWS][MAX_COLS];
} level_t;

/* 
 * Functia citeste fisierul specificat si salveaza level-ul
 * intr-un structura de tip 'level_t' specificata.
 *
 * In caz ca nu a putut fi citit fisierul, functia returneaza
 * valoarea 0. In caz de succes, 1 este returnat.
 */
int level_read(const char *filename, level_t *level);
