#include "level.h"
#include <stdio.h>

int level_read(const char *filename, level_t *level)
{
	/* Ne asiguram ca parametrii primiti nu sunt niste 
	 * pointeri catre zone de memorie invalide. */
	if (!filename || !level)
		return 0;

	/* Deschidem fisierul text pentru citire. */
	FILE *f = fopen(filename, "r");

	/* In cazul in care fisierul nu exista, fopen returneaza NULL. 
	 * Ne asiguram ca in cazul acesta anuntam problema mai departe. */
	if (!f) {
		printf("Fisierul '%s' nu exista!\n", filename);
		return 0;
	}

	/* Pe prima linie a fisierului text se afla numarul de linii,
	 * respectiv numarul de coloane, separate printr-un spatiu. */
	fscanf(f, "%d %d", &level->rows, &level->cols);

	/* Verificam daca numarul de linii si numarul de coloane
	 * specificate in fisierul de intrare sunt mai mici decat 
	 * dimensiunile maxime admise, MAX_ROWS si MAX_COLS. */
	for (int i = 0; i < level->rows; i++) {
		for (int j = 0; j < level->cols; j++) {
			fscanf(f, "%d", &level->bricks_life[i][j]);
			printf("%d ", level->bricks_life[i][j]);
		}
		printf("\n");
	}

	/* Am citit structura stagiului din fisier intr-o matrice unde 
	 * fiecare element reprezinta numarul de lovituri pe care o 
	 * caramida trebuie sa le primeasca pentru a se distruge. 
	 * 
	 * Acum trebuie sa atasam fiecarei caramizi o structura 
	 * de tipul 'sfRectangleShape'. Aceasta este o structura ce 
	 * mentine informatii despre pozitia caramizii pe ecran, unghiul de 
	 * rotatie si multe altele. Pe baza acestei structuri, biblioteca 
	 * CSFML, prin intermediul OpenGL, poate desena pe ecran caramizile 
	 * cu informatiile din structura (culoare, pozitie, etc). */
	
	/* Toate caramizile au aceasi dimensiune. Specificam dimensiunea
	 * caramizii pe axa X (lungime) si pe axa Y (inaltime) in pixeli. */
	sfVector2f brick_size = {
		.x = BRICK_WIDTH,
		.y = BRICK_HEIGHT
	};

	for (int i = 0; i < level->rows; i++) {
		for (int j = 0; j < level->cols; j++) {
			/* Daca aceasta caramida (i, j) are valoarea 0 citita din fisier,
			 * atunci ea nu va fi desenata pe ecran. */
			if (level->bricks_life[i][j] == 0) {
				level->bricks_shape[i][j] = NULL;
				continue;
			}

			level->bricks_shape[i][j] = sfRectangleShape_create();

			/* Calculam pozitia caramizii. */
			sfVector2f brick_pos = {
				.x = 1.6f * j * BRICK_WIDTH + 100.0f, 
				.y = 2.0f * i * BRICK_HEIGHT + 50.0f
			};

			/* Setam pozitia caramizii. */
			sfRectangleShape_setPosition(level->bricks_shape[i][j], brick_pos);
			
			/* Setam dimensinuea caramizii. */
			sfRectangleShape_setSize(level->bricks_shape[i][j], brick_size);
			
			/* Setam culoarea caramizii. */
			if (level->bricks_life[i][j] == 1)
				sfRectangleShape_setFillColor(level->bricks_shape[i][j], sfGreen);
			else if (level->bricks_life[i][j] == 2)
				sfRectangleShape_setFillColor(level->bricks_shape[i][j], sfYellow);
			else if (level->bricks_life[i][j] == 3)
				sfRectangleShape_setFillColor(level->bricks_shape[i][j], sfRed);
			else 
				sfRectangleShape_setFillColor(level->bricks_shape[i][j], sfBlue);
		}
	}

	/* Inchidem fisierul - nu lasam fisiere deschise. */
	fclose(f);

	/* Returnam valoare de succes, 1. */
	return 1;
}
