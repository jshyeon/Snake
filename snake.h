//basic snake class: definition of functions


#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

typedef struct {
	int x;
	int y;
}vec2;

enum {
	KEY_LEFT = 1,
	KEY_RIGHT = 2,
	KEY_UP = 3,
	KEY_DOWN = 4
};

class Snake {
private:
	vec2 head;// = { 300,300 };
	vec2 dir = { 0,0 };
	std::vector <vec2> body;// = { head };
	vec2 last_movement = { 0,0 };

public:
	Snake() {}
	Snake(int X, int Y) {head.x = X; head.y = Y;body.push_back(head);}
	std::vector<vec2> GetBodyPosition(void) {return body;}
	vec2 GetDir(void) { return last_movement; }
	void SetDir(int key);
	void SetHead(int x, int y) {head.x = x;head.y = y;body.push_back(head);}
	void Move(void);
	void Grow(void);
	bool Eat(vec2);
	bool Die(Snake);
	
};

#endif //SNAKE_H
