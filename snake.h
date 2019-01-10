//basic snake class: definition of functions


#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

typedef struct {
	unsigned x;
	unsigned y;
}vec2;

enum {
	KEY_LEFT = 1,
	KEY_RIGHT = 2,
	KEY_UP = 3,
	KEY_DOWN = 4
};

class Snake {
private:
	vec2 head = { 300,300 };
	vec2 dir = { 0,0 };
	std::vector <vec2> body{ head };

public:
	std::vector<vec2> Snake::GetBodyPosition(void) {return body;}
	void SetDir(int key);
	void Move(void);
	void Grow(void);
	bool Eat(vec2);
	bool Die(void);
	
};

#endif //SNAKE_H