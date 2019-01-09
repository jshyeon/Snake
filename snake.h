//basic snake class: definition of functions


#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

typedef struct {
	unsigned x;
	unsigned y;
}vec2;


class Snake {
private:
	vec2 head;
	vec2 dir;
	std::vector <vec2> body;

public:
	std::vector<vec2> Snake::GetBodyPosition(void) {return body;}
	void SetDir(int key);
	void Move(void);
	void Grow(void);
	bool Eat(vec2);
	bool Die(void);
	
};

#endif //SNAKE_H