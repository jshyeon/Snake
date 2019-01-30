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
	vec2 head = {300, 300};
	vec2 dir = {0, 0};
	std::vector <vec2> body{ head };
	vec2 last_movement = {0, 0};
	vec2 net_head = {0, 0};

public:
	Snake() {}
	Snake(int X, int Y) {head.x = X; head.y = Y;}
	std::vector<vec2> GetBodyPosition(void);
	vec2 GetDir(void);
	void SetDir(int key);
	void Move(void);
	void Grow(void);
	void CmpHead();
	bool Eat(vec2);
	bool Die(Snake);
	void Heading(int x, int y);
	vec2 NetHead(int X, int Y);
	
};

#endif //SNAKE_H
