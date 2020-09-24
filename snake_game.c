/* Template para programa basico Allegro
 * que salva uma imagem em arquivo
 *
 * Para lilar use:
 *     $gcc salvalle.c -o salvalle.x -Wall `allegro-config --cflags --libs`
 */


/* Definicoes de algumas cores */

#define CORBRANCO (makecol(255,255,255))
#define CORPRETO 1
#define CORCINZA (makecol(160,160,160))
#define CORAZUL (makecol(0, 0, 255))
#define CORVERDE (makecol(0, 255, 0))
#define CORAMARELO (makecol(255,255,100))
#define CORVERMELHO (makecol(255, 0, 0))

#define IMAGENAME "bola.bmp" /* nome do arquivo de imagem */

#include <stdio.h>
#include <allegro.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define PI 3.14


typedef enum snake_directions{left, up, right, down} t_directions;

typedef struct position_game /* snake places history*/
{
	unsigned short  x[100000];
	unsigned short  y[100000];

}t_position;

typedef struct snake
{
	int x; /* fruit x position */
	int y; /* fruit y position */
	int steps;
	int points;
	t_position pos;
	t_directions dir;
	int size;
	int eat;

}t_snake;

t_snake move(t_snake g);
t_snake check_eat(t_snake g);
bool check_dead(t_snake g);

int main(void)
{

	t_snake g;
	g.steps = 0;
	g.points = 0;
	g.size = 10; /* Snake size */
	g.eat = 0;
	g.x = 500; /* Inicial position */
	g.y = 500; /* Inicial position */

	for(int k = 0; k < g.size; k++)
	{
		g.pos.x[k] = 300 - k;
		g.pos.y[k] = 300;
	}

	g.dir = right;

	BITMAP *buff;
	allegro_init();
	install_timer();
	install_keyboard();
	set_color_depth(16);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,1000, 900, 0, 0);
	set_window_title("SNAKE GAME");

	buff = create_bitmap(901,801);

	srand(time(NULL));

	while(!key[KEY_E])
	{

		g = move(g); /* Move snake */

		if(check_dead(g)) /* Check if the snake died */
		{

			textprintf_ex(buff, font, 400, 400, CORVERDE, CORPRETO, " VOCE MORREU");
			draw_sprite(screen, buff, 50,50);
			sleep(5);
			break;
		}

		for(int j = 0; j < g.size; j++) /* Print the full snake */
		{
			circlefill(buff, g.pos.x[j], g.pos.y[j], 5, CORVERMELHO);
		}

		textprintf_ex(screen, font,10 , 10, CORVERDE, CORPRETO, " Points eaten: %d", g.points);
		rect(buff, 0,0, 900, 800, CORVERDE); /* game frame */
		circlefill(buff, g.x, g.y, 5, CORVERDE); /* print fruit */
		g = check_eat(g); /* check if fruit is eaten */
		draw_sprite(screen, buff, 50,50); /* refresh screen */
		clear(buff);
		usleep(1000);
	}

	destroy_bitmap(buff);
	allegro_exit();
	return EXIT_SUCCESS;
}

END_OF_MAIN()

/* Fuction to move the snake */
t_snake move(t_snake g)
{
	/* Check which arrow was pressed */
	if(key[KEY_UP] && g.dir != down)
		g.dir = up;

	else if(key[KEY_DOWN] && g.dir != up)
		g.dir = down;

	else if(key[KEY_LEFT] && g.dir != right)
		g.dir = left;

	else if(key[KEY_RIGHT] && g.dir != left)
		g.dir = right;


	for(int k = g.size - 1; k > 0; k--) /* refresh snake body position */
	{
		g.pos.x[k] = g.pos.x[k - 1];
		g.pos.y[k] = g.pos.y[k - 1];
	} 

	/* Snake head new position based on direction */
	if(g.dir == up || g.dir == down)
	{
		if(g.pos.y[0] == 799 && g.dir == down)
			g.pos.y[0] = 2;
		else if(g.pos.y[0] == 2 && g.dir == up)
			g.pos.y[0] = 799;
		else
			g.pos.y[0] +=  (g.dir - 2)*1;
	}

	if(g.dir == left || g.dir == right)
	{
		if(g.pos.x[0] == 899 && g.dir == right)
			g.pos.x[0] = 2;
		else if(g.pos.x[0] == 2 && g.dir == left)
			g.pos.x[0] = 899;
		else
			g.pos.x[0] += (g.dir - 1)*1;
	}
	return g;

}

/* Check if ate the dot and create new one */
t_snake check_eat(t_snake g)
{
	int sx = g.pos.x[0]; /* snake position */
	int sy = g.pos.y[0]; /* snake position */
	int px = g.x; /* point x position */
	int py = g.y; /* point y position */

	/* Detect when snake eat point */
	if(sqrt((sx - px)*(sx - px) + (sy - py)*(sy - py)) < 10)
	{
		g.x = rand()% 840 + 55;
		g.y = rand()% 740 + 55;
		g.size += 10;
		g.points += 1;
	}

	return g;
}

/* Check if snake ate itself */
bool check_dead(t_snake g)
{
	for( int k = 1; k <= g.size; k++)
	{
		if(g.pos.x[0] == g.pos.x[k] && g.pos.y[0] == g.pos.y[k]) /* Check if head touch any part of the body */
			return true;
	}
	return false;
}

