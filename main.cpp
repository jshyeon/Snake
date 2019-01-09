#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//#include <allegro5/allegro_color.h>


bool init(void) {
	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}
	if (!al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize allegro primitives addon!\n");
		return false;
	}
	return true;
}

void GenFood(void) {
	// Generate food at random rocation on the window
}

int main(int argc, char **argv) {

	assert(init());
	
	// create window
	ALLEGRO_DISPLAY *display;
	display = al_create_display(600, 600);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	// generate first food
	GenFood();

	bool running = true;
	while(running) {
		// draw snake at initial location
		for () {// it will draw whole body at the start of each itertaion
			al_draw_filled_rectangle(0, 0, 10, 10, al_map_rgb(0, 200, 0));

		}
		// wait until get keyboard input

		// when get arrow keyboard input call SetDir()

		// update snake location with calling Move()

		// case handling for eating food

		// case handling for crush

		// display what has been drawn on the background buffer
		al_flip_display();
	}
	
	al_destroy_display(display);

	return 0;
	
}
