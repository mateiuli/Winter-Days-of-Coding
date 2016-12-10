#pragma once

#include <stdio.h>
#include <math.h>
#include <SFML/Graphics.h>
#include "level.h"
#include "utils.h"

#define EXIT_FAILURE	-1
#define EXIT_SUCCESS	 0

/* Paddle */
void paddle_initialize(sfRectangleShape *paddle, sfVideoMode videoMode);
void paddle_update(sfRectangleShape *paddle);

/* Ball */
const float ball_speed = 3.f;
sfVector2f ball_velocity;
void ball_initialize(sfCircleShape *ball, sfVideoMode videoMode);
void ball_update(sfCircleShape *ball);
void ball_check_collision_paddle(sfCircleShape *ball, sfRectangleShape *paddle);
void ball_check_collision_level(sfCircleShape *ball, level_t *level);
int ball_check_collision_walls(sfCircleShape *ball, sfVideoMode videoMode);

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

	/* Activare limitator de Frames per Second la nivelul 
	 * maxim pe care il suporta monitorul. */
	sfRenderWindow_setVerticalSyncEnabled(window, 1);

	/* Citim level-ul din fisier. */
	level_t level;
	if (!level_read("resurse/levels/level1.txt", &level)) {
		printf("Nu am putut incarca level-ul!\n");
		return EXIT_FAILURE;
	}

	/* Definire si initializare platforma mobila (paddle). */
	sfRectangleShape *paddle = sfRectangleShape_create();
	paddle_initialize(paddle, videoMode);

	/* Definire si initializare bila (ball). */
	sfCircleShape *ball = sfCircleShape_create();
	ball_initialize(ball, videoMode);

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

			if(event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape)
				sfRenderWindow_close(window);
		}
		 
		ball_check_collision_walls(ball, videoMode);
		ball_check_collision_paddle(ball, paddle);
		ball_check_collision_level(ball, &level);

		ball_update(ball);
		paddle_update(paddle);

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
				if(level.bricks_life[i][j] != 0)
					sfRenderWindow_drawRectangleShape(window, level.bricks_shape[i][j], NULL);
			}
		}

		/* Desenare paddle. */
		sfRenderWindow_drawRectangleShape(window, paddle, NULL);

		/* Desenare ball. */
		sfRenderWindow_drawCircleShape(window, ball, NULL);

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

void paddle_initialize(sfRectangleShape *paddle, sfVideoMode videoMode)
{
	sfVector2f paddle_size = {
		.x = 100,
		.y = 10
	};

	sfVector2f paddle_pos = {
		.x = videoMode.width / 2.f,
		.y = videoMode.height - 50.f
	};

	sfColor paddle_color = {
		.r = 232,
		.g = 158,
		.b = 0,
		.a = 255
	};

	sfVector2f paddle_origin;
	paddle_origin.x = paddle_size.x / 2.f;
	paddle_origin.y = 0; // paddle_size.y / 2.f;

	sfRectangleShape_setSize(paddle, paddle_size);
	sfRectangleShape_setFillColor(paddle, paddle_color);
	sfRectangleShape_setOrigin(paddle, paddle_origin);
	sfRectangleShape_setPosition(paddle, paddle_pos);
}

void paddle_update(sfRectangleShape *paddle)
{
	if (sfKeyboard_isKeyPressed(sfKeyLeft)) {
		sfVector2f offset_left = { -4.0f, 0.f };
		sfRectangleShape_move(paddle, offset_left);
	}

	if (sfKeyboard_isKeyPressed(sfKeyRight)) {
		sfVector2f offset_right = { 4.0f, 0.f };
		sfRectangleShape_move(paddle, offset_right);
	}
}

void ball_initialize(sfCircleShape *ball, sfVideoMode videoMode)
{
	float ball_radius = 5.f;

	ball_velocity.x = ball_speed;
	ball_velocity.y = ball_speed;

	sfVector2f ball_pos = {
		.x = videoMode.width / 2.f - ball_radius / 2.f,
		.y = videoMode.height - 80.f
	};

	sfVector2f ball_origin;
	ball_origin.x = ball_radius / 2.f;
	ball_origin.y = ball_radius / 2.f;

	sfCircleShape_setRadius(ball, ball_radius);
	sfCircleShape_setPosition(ball, ball_pos);
	sfCircleShape_setFillColor(ball, sfWhite);
	sfCircleShape_setOrigin(ball, ball_origin);
}

void ball_update(sfCircleShape *ball)
{	
	sfCircleShape_move(ball, ball_velocity);
}

void ball_check_collision_paddle(sfCircleShape *ball, sfRectangleShape *paddle)
{
	sfVector2f ball_pos		= sfCircleShape_getPosition(ball);
	sfVector2f paddle_pos	= sfRectangleShape_getPosition(paddle);
	
	float ball_radius	= sfCircleShape_getRadius(ball);
	float paddle_width	= sfRectangleShape_getSize(paddle).x;
	
	/* Daca mingea atinge nivelul platformei. */
	if (ball_pos.y + ball_radius > paddle_pos.y)
		/* Daca mingea se afla in interiorul platformei. */
		if (fabs(ball_pos.x - paddle_pos.x) < paddle_width / 2) {
			/* Inversam deplasarea pe verticala. */
			ball_velocity.y = -ball_velocity.y;
			
			/* Inversam deplasarea pe orizontala. */
			if (ball_pos.x < paddle_pos.x)
				ball_velocity.x = -ball_speed;
			else
				ball_velocity.x = ball_speed;
		}
}

int ball_check_collision_walls(sfCircleShape *ball, sfVideoMode videoMode)
{
	sfVector2f ball_pos = sfCircleShape_getPosition(ball);
	float ball_radius	= sfCircleShape_getRadius(ball);

	/* Collision with walls and ceiling. */
	if (ball_pos.y - ball_radius < 0)
		ball_velocity.y = -ball_velocity.y;

	if (ball_pos.x - ball_radius < 0)
		ball_velocity.x = -ball_velocity.x;

	if (ball_pos.x + ball_radius > videoMode.width)
		ball_velocity.x = -ball_velocity.x;

	/* Daca mingea atinge muchia de jos - jocul se incheie. */
	if (ball_pos.y + ball_radius > videoMode.height)
		return 1;

	return 0;
}

void ball_check_collision_level(sfCircleShape *ball, level_t *level)
{
	/* Collision with bricks. */

	for (int i = 0; i < level->rows; i++) {
		for (int j = 0; j < level->cols; j++) {
			if (level->bricks_life[i][j] == 0)
				continue;

			collision_t collision = get_collision(level->bricks_shape[i][j], ball);
			if (collision == FROM_LEFT || collision == FROM_RIGHT) {
				level->bricks_life[i][j]--;
				ball_velocity.x = -ball_velocity.x;
			}
			else if (collision == FROM_TOP || collision == FROM_BOTTOM) {
				level->bricks_life[i][j]--;
				ball_velocity.y = -ball_velocity.y;
			}
		}
	}
}

