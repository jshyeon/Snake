#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "snake.h"

#include <pthread.h>

int net_ready = 1;
int sockfd;
char buffer_[256];
char net_input = 'i';
Snake snake1p, snake2p;

void *thread_func(void *targp) { // maybe need lock
	int no = *(int *)targp;		// I don't use this, but it represent player_number
	int a;				// iterator
	int pointX, pointY;
	char net_output[3];
	
	pthread_detach(pthread_self());

 	bzero(buffer_, 256);
	read(sockfd, buffer_, 256);
	net_input = buffer_[0];

	for (a = 0; a < 4; a++) {
		bzero(net_output, 3);
		strncpy(&net_output[0], &buffer_[a*3+2], 1);	// there were
		strncpy(&net_output[1], &buffer_[a*3+3], 1);	// some errors
		strncpy(&net_output[2], &buffer_[a*3+4], 1);	// in strncpy

		switch (a) {
			case 0:
				pointX = atoi(net_output); break;	// if net_output == 001?
			case 1:
				pointY = atoi(net_output); 
				snake1p.NetHead(pointX, pointY);
				break;
			case 2:
				pointX = atoi(net_output); break;
			case 3:
				pointY = atoi(net_output); 
				snake2p.NetHead(pointX, pointY);
				break;
			default:
				break;
		}
	}

	if (buffer_[0] == 'p'){
		snake2p.SetDir(KEY_RIGHT);
	}
	else if (buffer_[0] == 'm'){
		snake2p.SetDir(KEY_LEFT);
	}
	else if (buffer_[1] == 'p'){
		snake2p.SetDir(KEY_DOWN);
	}
	else if (buffer_[1] == 'm'){
		snake2p.SetDir(KEY_UP);
	}

	net_ready = 1;
	pthread_exit((void *) 0);
}

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
	food_loc.x = (rand() % 30) * 20;
	food_loc.y = (rand() % 30) * 20;
	return food_loc;
}
void DrawFood(vec2 food_loc) {
	DrawBox(food_loc, 0, 200, 0);
}

int main(int argc, char **argv)
{
	int portno, status, playerno, a; 
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	char *token, serv_seed;
	pthread_t tid;

	if (argc != 2)
	{
		printf("Usage: %s <chatting server address>:<port number>\n", argv[0]);
		return -1;
	}


	token = strtok(argv[1], ":");
	server = gethostbyname(token);

	if (server == NULL)
	{
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	token = strtok(NULL, ":");

	if (token == NULL)
	{
		printf("Error: port number miss\n");
		return -1;
	}
	portno = atoi(token);

	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0) // connect
	{
		printf("Error: connection fail\n");
		return -1;
	}


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

	bzero(buffer, 256);
	read(sockfd, buffer, 256);
	// generate first food
	serv_seed = buffer[0];
	srand(serv_seed);
	vec2 food_loc = GenFood();


	// 1p or 2p?
	bzero(buffer, 256);
	read(sockfd, buffer, 256);
	if (buffer[0] == '1') {
		snake1p.Heading(140,140); snake2p.Heading(460,460);
		snake1p.NetHead(140,140); snake2p.NetHead(460,460);
		playerno = 1;
	} else {
		snake1p.Heading(460,460); snake2p.Heading(140,140);
		snake1p.NetHead(460,460); snake2p.NetHead(140,140);
		playerno = 2;
	}
	
	bool running = true;
	while(running) {

		// display what has been drawn on the background buffer
		al_clear_to_color(al_map_rgb(0, 0, 0));
		std::vector<vec2> body1 = snake1p.GetBodyPosition();
		std::vector<vec2> body2 = snake2p.GetBodyPosition();
		int body_length1 = static_cast<int>(body1.size());// this might be mallicious
		int body_length2 = static_cast<int>(body2.size());

		// draw snake at initial location
		for (int i = 0; i < body_length1; i++) {
			DrawBox(body1[i], 200, 0, 100);
		}
		for (int i = 0; i < body_length2; i++) {
			DrawBox(body2[i], 100, 200, 50);
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
				if (event.type == ALLEGRO_EVENT_TIMER) {
					if (event.timer.source == timer) {
						// wait until get keyboard input
						// when get arrow keyboard input call SetDir()

						// update snake location with calling Move()
						if (!snake1p.Die(snake2p) 
								&& !snake2p.Die(snake1p)) {
							if (snake1p.Eat(food_loc)) {
								snake1p.Grow();
								food_loc = GenFood();
							}
							else if (snake2p.Eat(food_loc)) {
								snake2p.Grow();
								food_loc = GenFood();
							}
							else {
								snake1p.Move();
								snake1p.CmpHead();
								snake2p.Move();
								snake2p.CmpHead();
							}
						}
						else if (snake1p.Die(snake2p) 
								&& snake2p.Die(snake1p)){
							printf("Snake 1 dead!\n");
							break;
						}
						else if (snake2p.Die(snake1p) 
								&& !snake1p.Die(snake2p)) {
							printf("Snake 2 dead!\n");
							break;
						}
						else if (snake1p.Die(snake2p) 
								&& snake2p.Die(snake1p)) {
							printf("Draw!\n");
							break;
						}
					}
				}
			}

			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {										// when multiple key is pressed it does not work
				if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT 
						&& (snake1p.GetDir().x != -1 || body_length1 == 1))
				{
					// snake1p.SetDir(KEY_RIGHT);
					printf("move right\n"); 
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'p';
					buffer[1] = 'z';
					write(sockfd, buffer, 2*sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT 
						&& (snake1p.GetDir().x != 1 || body_length1 == 1))
				{
					// snake1p.SetDir(KEY_LEFT); 
					printf("move left\n"); 
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'm';
					buffer[1] = 'z';
					write(sockfd, buffer, 2*sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN 
						&& (snake1p.GetDir().y != -1 || body_length1 == 1))
				{
					// snake1p.SetDir(KEY_DOWN); 
					printf("move down\n"); 
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'z';
					buffer[1] = 'p';
					write(sockfd, buffer, 2*sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_UP 
						&& (snake1p.GetDir().y != 1 || body_length1 == 1))
				{
					// snake1p.SetDir(KEY_UP); 
					printf("move up\n"); 
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'z';
					buffer[1] = 'm';
					write(sockfd, buffer, 2*sizeof(char));
				}
			}
		}

		if (net_ready == 1) {	// what is its meaning?
                	net_ready = 0;
			status = pthread_create(&tid, NULL, thread_func, &playerno);
			if (status != 0) {perror("pthread_create problem");}
		}
	}
	close(sockfd);
	
	return 0;
}
