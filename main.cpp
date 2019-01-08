//#include <iostream>
//
//using namespace std;
//
//int main() {
//
//    return 0;
//}
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;

	if (!al_init() || !al_init_primitives_addon()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	display = al_create_display(600, 600);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	al_clear_to_color(al_map_rgb(0xff, 0xff, 0xff));

	al_flip_display();
	al_rest(3.0);
	al_destroy_display(display);

	return 0;
}