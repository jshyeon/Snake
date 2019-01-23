#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
//#include <WinSock2.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include "snake.h"

#include <pthread.h>
//
//#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)
//#define bcopy(b1,b2,len) (memmove((b2), (b1), (len)), (void) 0)

int net_ready = 1, sockfd;
char buffer_[256];
char net_input = 'i';

void *thread_func(void *targp)
{
	//	int *socketfd = (int*)targp;
	//pthread_detach(pthread_self());
	bzero(buffer_, 256);
	//    read(*socketfd, buffer_, 256);
	read(sockfd, buffer_, 256);
	net_input = buffer_[0];
	net_ready = 1;
	//pthread_exit((void*)0);
	return 0;
}

bool init(void)
{
	if (!al_init())
	{
		fprintf(stderr, "failed to initialize allegro!\n");
		return false;
	}
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize allegro primitives addon!\n");
		return false;
	}
	return true;
}

void DrawBox(vec2 coord, int r, int g, int b)
{
	al_draw_filled_rectangle(coord.x + 1, coord.y + 1, coord.x + 19, coord.y + 19, al_map_rgb(r, g, b));
}
// Generate food at random rocation on the window
vec2 GenFood(void)
{
	vec2 food_loc;
	food_loc.x = (rand() % 30) * 20;
	food_loc.y = (rand() % 30) * 20;
	return food_loc;
}
void DrawFood(vec2 food_loc)
{
	DrawBox(food_loc, 0, 200, 0);
}

int main(int argc, char **argv)
{
	//	int sockfd,
	int portno, n = 0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];

	if (argc != 2)
	{
		printf("Usage: %s <chatting server address>:<port number>\n", argv[0]);
		return -1;
	}

	char *token;

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

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
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
	if (!display)
	{
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}
	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}
	timer = al_create_timer(0.2);

	al_install_keyboard(); // add handling keyboard setting fails

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	Snake snake1p, snake2p;

	char serv_seed;
	bzero(buffer, 256);
	read(sockfd, buffer, 256);

	// generate first food
	serv_seed = buffer[0];

	if (serv_seed < 16)
	{
		snake1p.SetHead(140, 140);
		snake2p.SetHead(460, 460);
	}
	else
	{
		snake1p.SetHead(460, 460);
		snake2p.SetHead(140, 140);
	}

	serv_seed = serv_seed & 0x0F;
	srand(serv_seed);
	vec2 food_loc = GenFood();

	std::vector<vec2> body1, body2;
	int body_length1, body_length2;
	pthread_t tid;

	bool running = true;
	while (running)
	{

		//printf("%i while loop\n",n);
		// display what has been drawn on the background buffer
		al_clear_to_color(al_map_rgb(0, 0, 0));
		body1 = snake1p.GetBodyPosition();
		body2 = snake2p.GetBodyPosition();
		body_length1 = static_cast<int>(body1.size()); // this might be mallicious
		body_length2 = static_cast<int>(body2.size());

		// draw snake at initial location
		for (int i = 0; i < body_length1; i++)
		{ // it will draw whole body at the start of each itertaion
			DrawBox(body1[i], 200, 0, 100);
		}
		for (int i = 0; i < body_length2; i++)
		{
			DrawBox(body2[i], 100, 200, 50);
		}

		DrawFood(food_loc);
		al_flip_display();

		ALLEGRO_EVENT event;
		ALLEGRO_KEYBOARD_STATE keyState;

		if (!al_is_event_queue_empty(event_queue))
		{
			al_wait_for_event(event_queue, &event);
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				running = false;
			else
			{
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
				if (event.type == ALLEGRO_EVENT_TIMER)
				{
					if (event.timer.source == timer)
					{

						// wait until get keyboard input
						// when get arrow keyboard input call SetDir()

						// update snake location with calling Move()
						if (!snake1p.Die(snake2p) && !snake2p.Die(snake1p))
						{
							if (snake1p.Eat(food_loc))
							{
								snake1p.Grow();
								//snake2p.Move();
								food_loc = GenFood();
							}
							else if (snake2p.Eat(food_loc))
							{
								snake2p.Grow();
								//snake1p.Move();
								food_loc = GenFood();
							}
							else
							{
								snake1p.Move();
								snake2p.Move();
							}
						}
						else if (snake1p.Die(snake2p) && snake2p.Die(snake1p))
						{
							printf("Snake 1 dead!\n");
							break;
						}
						else if (snake2p.Die(snake1p) && !snake1p.Die(snake2p))
						{
							printf("Snake 2 dead!\n");
							break;
						}
						else if (snake1p.Die(snake2p) && snake2p.Die(snake1p))
						{
							printf("Draw!\n");
							break;
						}
					}
				}
			}

			if (event.type == ALLEGRO_EVENT_KEY_DOWN)
			{ // when multiple key is pressed it does not work
				if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT && (snake1p.GetDir().x != -1 || body_length1 == 1))
				{
					snake1p.SetDir(KEY_RIGHT); //printf("move right\n");
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'r';
					//snprintf(buffer, 256, "%c",'u');
					//scanf("%s", buffer);
					write(sockfd, buffer, sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_LEFT && (snake1p.GetDir().x != 1 || body_length1 == 1))
				{
					snake1p.SetDir(KEY_LEFT);
					printf("move left\n");
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'l';
					write(sockfd, buffer, sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && (snake1p.GetDir().y != -1 || body_length1 == 1))
				{
					snake1p.SetDir(KEY_DOWN);
					printf("move down\n");
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'd';
					write(sockfd, buffer, sizeof(char));
				}
				else if (event.keyboard.keycode == ALLEGRO_KEY_UP && (snake1p.GetDir().y != 1 || body_length1 == 1))
				{
					snake1p.SetDir(KEY_UP);
					printf("move up\n");
					event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
					bzero(buffer, 256);
					buffer[0] = 'u';
					write(sockfd, buffer, sizeof(char));
				}

				// else if (event.keyboard.keycode == ALLEGRO_KEY_D && (snake2p.GetDir().x != -1 || body_length2 == 1))
				// {
				// 	snake2p.SetDir(KEY_RIGHT); printf("move right\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
				// }
				// else if (event.keyboard.keycode == ALLEGRO_KEY_A && (snake2p.GetDir().x != 1 || body_length2 == 1))
				// {
				// 	snake2p.SetDir(KEY_LEFT); printf("move left\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
				// }
				// else if (event.keyboard.keycode == ALLEGRO_KEY_S && (snake2p.GetDir().y != -1 || body_length2 == 1))
				// {
				// 	snake2p.SetDir(KEY_DOWN); printf("move down\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
				// }
				// else if (event.keyboard.keycode == ALLEGRO_KEY_W && (snake2p.GetDir().y != 1 || body_length2 == 1))
				// {
				// 	snake2p.SetDir(KEY_UP); printf("move up\n"); event.keyboard.type = ALLEGRO_EVENT_KEY_UP;
				// }
			}
		}

		if (net_ready == 1)
		{
			net_ready = 0;
			printf("%i thread creation\n", n);
			pthread_create(&tid, NULL, thread_func, (void *)sockfd);
		}
		n++;

		if (net_input == 'r')
		{
			snake2p.SetDir(KEY_RIGHT);
		}
		else if (net_input == 'l')
		{
			snake2p.SetDir(KEY_LEFT);
		}
		else if (net_input == 'd')
		{
			snake2p.SetDir(KEY_DOWN);
		}
		else if (net_input == 'u')
		{
			snake2p.SetDir(KEY_UP);
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

		//al_clear_to_color(al_map_rgb(0, 0, 0));
		//body = snake1p.getbodyposition();
		//body_length = static_cast<int>(body.size());// this might be mallicious

		//// draw snake at initial location
		//for (int i = 0; i < body_length; i++) {// it will draw whole body at the start of each itertaion
		//	drawbox(body[i], 200, 0, 100);
		//}

		//drawfood(food_loc);
		//al_flip_display();
	}
	close(sockfd);
	//al_destroy_display(display);

	return 0;
}
