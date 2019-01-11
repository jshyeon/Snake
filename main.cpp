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
	al_draw_filled_rectangle(coord.x+1, coord.y + 1, coord.x + 19, coord.y + 19, al_map_rgb(r, g, b));
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
	ALLEGRO_TIMER *timer;
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
	timer = al_create_timer(0.2);

	al_install_keyboard();// add handling keyboard setting fails

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	Snake snake1p;

	// generate first food
	vec2 food_loc = GenFood();

	bool running = true;
	while(running) {


		// display what has been drawn on the background buffer
		al_clear_to_color(al_map_rgb(0, 0, 0));
		std::vector<vec2> body = snake1p.GetBodyPosition();
		int body_length = static_cast<int>(body.size());// this might be mallicious

		// draw snake at initial location
		for (int i = 0; i < body_length; i++) {// it will draw whole body at the start of each itertaion
			DrawBox(body[i], 200, 0, 100);
		}

		DrawFood(food_loc);
		al_flip_display();

		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		if (!al_is_event_queue_empty(event_queue)) {
			al_wait_for_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				running = false;
			else {
				//if (event.type == ALLEGRO_EVENT_KEY_DOWN) {										// when multiple key is pressed it does not work
				//	if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && (snake1p.GetDir().x != -1 || body_length == 1))
				//		snake1p.SetDir(KEY_RIGHT);
				//	else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && (snake1p.GetDir().x != 1 || body_length == 1))
				//		snake1p.SetDir(KEY_LEFT);
				//	else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && (snake1p.GetDir().y != -1 || body_length == 1))
				//		snake1p.SetDir(KEY_DOWN);
				//	else if (event.keyboard.keycode == ALLEGRO_KEY_UP && (snake1p.GetDir().y != 1 || body_length == 1))
				//		snake1p.SetDir(KEY_UP);
				//	/*switch (event.keyboard.keycode) {
				//	case ALLEGRO_KEY_RIGHT:
				//		snake1p.SetDir(KEY_RIGHT);
				//		break;
				//	case ALLEGRO_KEY_LEFT:
				//		snake1p.SetDir(KEY_LEFT);
				//		break;
				//	case ALLEGRO_KEY_DOWN:
				//		snake1p.SetDir(KEY_DOWN);
				//		break;
				//	case ALLEGRO_KEY_UP:
				//		snake1p.SetDir(KEY_UP);
				//		break;
				//	}*/
				//}
				if (event.type == ALLEGRO_EVENT_TIMER) {
					if (event.timer.source == timer) {

						// wait until get keyboard input
						// when get arrow keyboard input call SetDir()


						// update snake location with calling Move()
						if (!snake1p.Die()) {
							if (snake1p.Eat(food_loc)) {
								snake1p.Grow();
								food_loc = GenFood();
							}
							else {
								snake1p.Move();
							}
						}
						else {
							break;
						}
					}
				}
			}
		

		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {										// when multiple key is pressed it does not work
			if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && (snake1p.GetDir().x != -1 || body_length == 1))
			{
				snake1p.SetDir(KEY_RIGHT); printf("move right\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && (snake1p.GetDir().x != 1 || body_length == 1))
			{
				snake1p.SetDir(KEY_LEFT); printf("move left\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && (snake1p.GetDir().y != -1 || body_length == 1))
			{
				snake1p.SetDir(KEY_DOWN); printf("move down\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
			}
			else if (event.keyboard.keycode == ALLEGRO_KEY_UP && (snake1p.GetDir().y != 1 || body_length == 1))
			{
				snake1p.SetDir(KEY_UP); printf("move up\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
			}
		}
		}
		//al_get_keyboard_state(&keyState);
		//if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT) && (snake1p.GetDir().x != -1 || body_length == 1))
		//{
		//	snake1p.SetDir(KEY_RIGHT); printf("move right\n");
		//}
		//else if (al_key_down(&keyState, ALLEGRO_KEY_LEFT) && (snake1p.GetDir().x != 1 || body_length == 1))
		//{
		//	snake1p.SetDir(KEY_LEFT); printf("move left\n");
		//}
		//else if (al_key_down(&keyState, ALLEGRO_KEY_DOWN) && (snake1p.GetDir().y != -1 || body_length == 1))
		//{
		//	snake1p.SetDir(KEY_DOWN); printf("move down\n");
		//}
		//else if (al_key_down(&keyState, ALLEGRO_KEY_UP) && (snake1p.GetDir().y != 1 || body_length == 1))
		//{
		//	snake1p.SetDir(KEY_UP); printf("move up\n");
		//}
		//al_clear_keyboard_state();
		// case handling for eating food

		// case handling for crush
		//_sleep(100.0);
	
		al_clear_to_color(al_map_rgb(0, 0, 0));
		body = snake1p.GetBodyPosition();
		body_length = static_cast<int>(body.size());// this might be mallicious

		// draw snake at initial location
		for (int i = 0; i < body_length; i++) {// it will draw whole body at the start of each itertaion
			DrawBox(body[i], 200, 0, 100);
		}

		DrawFood(food_loc);
		al_flip_display();
	}
	
	al_destroy_display(display);
	
	return 0;
	
}
