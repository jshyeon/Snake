//implementation of functions of Snake class

#include "snake.h"
using namespace std;

//int size = 20;                  // it isn't read
//vec2 Snake::GetDir(void) {}
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
void Snake::Move(void) {        // should i make new vec2 using pointer?
    head.x = head.x + dir.x * 20;
    head.y = head.y + dir.y * 20;
    body.insert(body.begin(), head);
    body.pop_back();
}
void Snake::Grow(void) {        // it will be positioned after "Eat"
    body.insert(body.end(), head);   // end() or end()-1 ?
}
bool Snake::Eat(vec2 food) {    // it should be positioned after "move", so I can check whether the head touch the food
    if (head.x == food.x && head.y == food.y) {
        return true;
    }
	return false;
}
bool Snake::Die(void) {         // 1. hit my body(trouble with grow / solution: "Grow"->"Die" order), 2. push exact reverse direction, 
                                // 3. hit wall, 4. hit other worm, "5. hit other worm face to face" : condition 1 & 3
    /*bool D;                   is it necessary?*/
    // condition 1
    for (vector<vec2>::iterator a = body.begin()+1; a != body.end(); a++) {
        if (head.x == a->x && head.y == a->y) {
            return true;
        } else {}
    }
    // condition 3
    if (head.x > 580 || head.x <0) {return true;}
    if (head.y > 580 || head.y <0) {return true;}

    return false;
}
