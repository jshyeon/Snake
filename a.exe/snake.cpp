//implementation of functions of Snake class
#include <vector>
#include "snake.h"
using namespace std;

//int size = 20;                  // it isn't read
void Snake::CmpHead() {
	if (head.x == net_head.x && head.y == net_head.y) {
		return;
	}
	else {
		Heading(net_head.x, net_head.y);	// I'm not sure
	}
}
vec2 Snake::NetHead(int X, int Y) {
	net_head.x = X;
	net_head.y = Y;
	return Snake::net_head;
}
vector <vec2> Snake::GetBodyPosition(void) {
	return Snake::body;
}
vec2 Snake::GetDir(void) {
	return Snake::last_movement;
}
void Snake::SetDir(int key) {   // KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_DOWN, find these numbers
    if          (key == KEY_LEFT) {
        dir.x = -1;
        dir.y = 0;
    } else if   (key == KEY_RIGHT) {
        dir.x = 1;
        dir.y = 0;
    } else if   (key == KEY_UP) {
        dir.x = 0;
        dir.y = -1;
    } else if   (key == KEY_DOWN) {
        dir.x = 0;
        dir.y = 1;
    } else {}                   // wrong input -> nothing
}
void Snake::Heading(int x, int y) {
	head.x = x;
	head.y = y;
}
void Snake::Move(void) {        // should i make new vec2 using pointer?
    head.x = head.x + dir.x * 20;
    head.y = head.y + dir.y * 20;
    body.insert(body.begin(), head);
    body.pop_back();
	
    last_movement.x = dir.x;
    last_movement.y = dir.y;
}
void Snake::Grow(void) {        // it will be positioned after "Eat"
	head.x = head.x + dir.x * 20;
	head.y = head.y + dir.y * 20;
	body.insert(body.begin(), head);
}
bool Snake::Eat(vec2 food) {    // it should be positioned after "move", so I can check whether the head touch the food
    if (head.x + dir.x*20 == food.x && head.y + dir.y*20 == food.y) {
        return true;
    }
	return false;
}
bool Snake::Die(Snake snake2) {
    // condition 1
    for (vector<vec2>::iterator a = body.begin()+1; a != body.end(); a++) {
        if (head.x == a->x && head.y == a->y) {
            return true;
        }
    }
    // condition collision
    vector<vec2> body2 = snake2.GetBodyPosition();
    for (vector<vec2>::iterator a = body2.begin(); a != body2.end(); a++) {
        if (head.x == a->x && head.y == a->y) return true;
    }
    // condition 3
    if (head.x > 580 || head.x <0) {return true;}
    if (head.y > 580 || head.y <0) {return true;}

    return false;
}
