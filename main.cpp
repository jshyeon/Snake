#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "snake.h"

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

void DrawBox(vec2 coord, int r, int g, int b) {
	al_draw_filled_rectangle(coord.x, coord.y, coord.x + 20, coord.y + 20, al_map_rgb(r, g, b));
}
// Generate food at random rocation on the window
vec2 GenFood(void) {
	vec2 food_loc;
	srand(time(NULL));
	food_loc.x = (rand() % 30) * 20;
	food_loc.y = (rand() % 30) * 20;
	return food_loc;
}
void DrawFood(vec2 food_loc) {
	DrawBox(food_loc, 0, 200, 0);
}

int main(int argc, char **argv) {

	assert(init());
	
	// create window
	ALLEGRO_DISPLAY *display;
	ALLEGRO_EVENT_QUEUE *event_queue;
	display = al_create_display(600, 600);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	al_install_keyboard();// add handling keyboard setting fails
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	Snake snake1p;

	// generate first food
	vec2 food_loc = GenFood();

	bool running = true;
	while(running) {
		al_clear_to_color(al_map_rgb(0, 0, 0));
		std::vector<vec2> body = snake1p.GetBodyPosition();
		int body_length = static_cast<int>(body.size());// this might be mallicious

		// draw snake at initial location
		for (int i = 0; i < body_length; i++) {// it will draw whole body at the start of each itertaion
			DrawBox(body[i], 200, 0, 100);
		}

		DrawFood(food_loc);
		// wait until get keyboard input
		// when get arrow keyboard input call SetDir()
		ALLEGRO_KEYBOARD_STATE keyState;

		al_get_keyboard_state(&keyState);

		if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT))
			snake1p.SetDir(KEY_RIGHT);
		else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT))
			snake1p.SetDir(KEY_LEFT);
		else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN))
			snake1p.SetDir(KEY_DOWN);
		else if (al_key_down(&keyState, ALLEGRO_KEY_UP))
			snake1p.SetDir(KEY_UP);

		
		// update snake location with calling Move()
		if (!snake1p.Die()) {
			if (snake1p.Eat(food_loc)) {
				snake1p.Grow();
				GenFood();
				DrawFood(food_loc);
			}
			else {
				snake1p.Move();
			}
		}
		else {
			break;
		}

		// case handling for eating food

		// case handling for crush
		_sleep(100.0);
		// display what has been drawn on the background buffer
		al_flip_display();
	
	}
	
	al_destroy_display(display);

	return 0;
	
}
