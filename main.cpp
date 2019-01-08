#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//#include <allegro5/allegro_color.h>
//checking synchronize
ALLEGRO_COLOR color_green = al_map_rgb(0,0xff,0);

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize allegro primitives addon!\n");
		return -1;
	}
	display = al_create_display(600, 600);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	al_clear_to_color(al_map_rgb(0xff, 0xff, 0xff));
	/* green filled rectangle */
	al_draw_filled_rectangle(10, 10, 60, 60, color_green);
	al_flip_display();

	al_rest(3.0);
	al_destroy_display(display);

	return 0;
}
