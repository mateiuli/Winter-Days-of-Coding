#pragma once

#include <stdio.h>
#include <SFML/Graphics.h>
#include "level.h"

#define EXIT_FAILURE	-1
#define EXIT_SUCCESS	 0

int main()
{
	/* Initializare structura cu dimensiunile fereastre. */
	sfVideoMode videoMode = {
		.width = 800,
		.height = 600,
		.bitsPerPixel = 32
	};

	/* Crearea efectiva a ferestrei de desenare, cu dimensiunile
	   specificate anterior. */
	sfRenderWindow *window = sfRenderWindow_create(
		videoMode,
		"Winter Days of Coding - Arkanoid",
		sfClose,
		NULL
	);
	if (!window)
		return EXIT_FAILURE;

	/* Citim level-ul din fisier. */
	level_t level;
	if (!level_read("resurse/levels/level1.txt", &level)) {
		printf("Nu am putut incarca level-ul!\n");
		return EXIT_FAILURE;
	}

	/* Bucla principala a jocului. Cat timp fereastra este inca deschisa
	   jocul continua. Cand fereastra este inchisa se va iesi din bucla. */
	while (sfRenderWindow_isOpen(window)) {
		/* Tratare evenimente de la tastatura / mouse. */
		sfEvent event;

		/* Evenimentele sunt trimise de la sistemul de operare sub forma de 
		   intreruperi. Biblitoca CSFML le pune intr-o coada - in ordinea in 
		   care au fost primite. 

		   Interogam coada daca au aparut evenimente de la ultima iteratie
		   si in functie de acestea executam diferite actiuni, cum ar fi 
		   miscarea unor obiecte in fereastra. */
		while (sfRenderWindow_pollEvent(window, &event)) {
			/* Tratam fiecare tip de eveniment diferit. */
			/* Evenimentul de inchidere de fereastra - cand apasam pe X. */
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(window);
		}

		/* Golire fereastra. Desenarea intregii ferestre cu o culoare
		   pentru a o pregati pentru urmatoarea etapa de randare. 
		   Aceasta operatie se realizeaza pe bufferul din spate. */
		sfColor bgColor = {
			.r = 17, .g = 81, .b = 137, .a = 255
		};
		sfRenderWindow_clear(window, bgColor);

		/* Desenare obiecte in buffer-ul din spate. */
		for (int i = 0; i < level.rows; i++) {
			for (int j = 0; j < level.cols; j++) {
				if(level.bricks_shape[i][j] != NULL)
					sfRenderWindow_drawShape(window, level.bricks_shape[i][j], NULL);
			}
		}

		/* Facem swap intre buffer-ul din fata (ecranul) si 
		   buffer-ul din spate. */
		sfRenderWindow_display(window);
	}

	/* Un programator trebuie tot timpul sa faca singur curat dupa el.
	   Este datoria noastra sa eliberam resursele folosite inainte ca
	   programul sa isi incheie executia. */
	sfRenderWindow_destroy(window);

	return EXIT_SUCCESS;
}