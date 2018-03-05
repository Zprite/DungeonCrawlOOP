
#include <iostream>
#include <String>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h> 
#define width 10
#define height 10
#define ENTITY_VAL 1
#define ITEM_VAL 2
#define EXIT_VAL 3
#define STASIS_VAL -1
#define EMPTY_MARK '-'
#define ENEMY_MARK 'E'
#define PLAYER_MARK 'P'
#define ITEM_MARK 'I'
#define EXIT_MARK 'X'
#define WALL_MARK '*'
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4

using namespace std;

char board[width][height];
void print_board();

class entity {
	protected:
		int x_pos;
		int y_pos;
		int direction;
		char mark;
		int check_direction(int x, int y);
	public:
		void set_mark(char x);
		void set_spawn();
		void move_up();
		void move_left();
		void move_down();
		void move_right();
		void check_collision(int val1, int val2);
		
};

class player : public entity {
public:
	void player_select_move();
};

class enemy : public entity {
public:
	void enemy_select_move();
};

void entity::set_mark(char x){
	mark = x;
}

void entity::set_spawn() {
	bool valid_spawn;
	int spawn_x;
	int spawn_y;
	do {
		valid_spawn = 0;
		spawn_x = rand () % width-1;
		spawn_y = rand () % height-1;
		// If the the randomly generated spawnpoint is not empty, loop until empty space found.
		if (board[spawn_y][spawn_x] == EMPTY_MARK) {
			valid_spawn = 1;
		}
	} while (valid_spawn==0);
	x_pos = spawn_x;
	y_pos = spawn_y;
	board[y_pos][x_pos] = mark; // Set mark to board.
}

void entity :: move_up() {
	board[y_pos][x_pos] = EMPTY_MARK;
	y_pos -= 1;
	board[y_pos][x_pos] = mark;
	}
void entity::move_left() {
	board[y_pos][x_pos] = EMPTY_MARK;
	x_pos -= 1;
	board[y_pos][x_pos] = mark;
 }
void entity::move_down() {
	board[y_pos][x_pos] = EMPTY_MARK;
	y_pos += 1;
	board[y_pos][x_pos] = mark;
}
void entity::move_right() {
	board[y_pos][x_pos] = EMPTY_MARK;
	x_pos += 1;
	board[y_pos][x_pos] = mark;
}

void entity::check_collision(int val1, int val2) {
	int rVal;
	switch (direction) {
	case UP:
		rVal = check_direction(y_pos - 1, x_pos); // Check up, then move if possisble
		if (rVal != val1 && rVal != val2 && rVal!= STASIS_VAL) {
			move_up();
		}
		break;
	case LEFT:
		rVal = check_direction(y_pos, x_pos - 1); // Check left, then move if possible
		if (rVal != val1 && rVal != val2 && rVal != STASIS_VAL) {
			move_left();
		}
		break;
	case DOWN:
		rVal = check_direction(y_pos + 1, x_pos); // Check down, then move if possible
		if (rVal != val1 && rVal != val2 && rVal != STASIS_VAL) {
			move_down();
		}
		break;
	case RIGHT:
		rVal = check_direction(y_pos, x_pos + 1); // Check right, then move if possible
		if (rVal != val1 && rVal != val2 && rVal != STASIS_VAL) {
			move_right();
		}
		break;
	}
	if (rVal == 1) {
		// battle
		cout << "Enemy encounter! Prepare for battle . . .\n";
		Sleep(500);
	}
	else if (rVal == 2) {
		//Get item
		cout << "PLAYER GOT ITEM!\n";
		Sleep(500);
	}
	else if (rVal == 3) {
		//Load next level
		cout << "LOADING NEXT LEVEL\n";
		Sleep(500);
	}
}

int entity::check_direction(int y, int x) {

	if (board[y][x] == mark) {
		return STASIS_VAL; // If checking a tile with the same entity type, do nothing.
	}
	switch (board[y][x]) {
	case 'P':
	case 'E':
		//do battle
		return 1;
	case ITEM_MARK:
		// Item function
		return ITEM_VAL;
	case EXIT_MARK:
		return EXIT_VAL;
		// Load next level
	case WALL_MARK:
		return STASIS_VAL;
	default:
		return 4;
	}
}

void player::player_select_move() {
	char key;
	bool valid;
	do {
		valid = 1;
		key = _getch();
		switch (key) {
		case 'W':
		case 'w':
			direction = 1;
			break;
		case 'A':
		case 'a':
			direction = 2;
			break;
		case 'S':
		case 's':
			direction = 3;
			break;
		case 'D':
		case 'd':
			direction = 4;
			break;
		default:
			valid = 0; // If input is not valid.
		}	
	} while (!valid);
} 

void enemy::enemy_select_move() {
	direction = rand() % 4 + 1;
}

int main() {
	
	srand(time(0));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			board[j][i] = EMPTY_MARK;
		}
	}
	player player;
	enemy enemy[10];
	player.set_mark(PLAYER_MARK);
	enemy[0].set_mark(ENEMY_MARK);
	print_board(); // Need to print board to update set_spawn
	player.set_spawn();
	int x = 1;

	do {
		print_board();
		player.player_select_move(); 
		player.check_collision(0,0); //Player can move to items and exit, so 0 is added as prameters. 
		print_board();
		/*
		enemy1.enemy_select_move();
		enemy1.check_collision(ITEM_VAL, EXIT_VAL);
		*/
	} while (x==1);
}

void print_board() {
	system("CLS");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {

			// MAKE BORDERS OF THE GAME BOARD WALLS
			if (i == 0 || i == width - 1) {
				board[j][i] = WALL_MARK;
			}
			else if (j == 0 || j == height - 1) {
				board[j][i] = WALL_MARK;
			}
			cout << " " << board[j][i] << " ";
		}
		cout << "\n";
	}
}