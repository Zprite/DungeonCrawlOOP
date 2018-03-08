
#include <iostream>
#include <String>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <vector>
#include <time.h> 
#define width 10
#define height 10
#define STASIS_VAL -1
#define ENTITY_VAL 1
#define ITEM_VAL 2
#define EXIT_VAL 3
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

int enemy_count = 5;
bool delete_enemy;
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
		int get_x_pos();
		int get_y_pos();
		void set_board(); // Used for setting mark position on board
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

void entity::set_board() {
	board[y_pos][x_pos] = mark;
}

void entity::set_mark(char x){
	mark = x;
}

int entity::get_x_pos() {
	return x_pos;
}
int entity::get_y_pos() {
	return y_pos;
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
	delete_enemy = 0; // Reset delete_enemy value
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
		print_board();
		cout << "Enemy encounter! Prepare for battle . . .\n";
		delete_enemy = 1;
		Sleep(500);
	}
	else if (rVal == 2) {
		//Get item
		print_board();
		cout << "PLAYER GOT ITEM!\n";
		Sleep(500);
	}
	else if (rVal == 3) {
		//Load next level
		print_board();
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


void spawn_enemy(vector<enemy> &Enemies) {
	for (int i = 0; i < Enemies.size(); i++) {
		Enemies[i].set_mark(ENEMY_MARK);
		Enemies[i].set_spawn();
	}
}

// Move function sets delete_enemy variable. This function deletes enemy if delete_enemy = true.
int check_delete_enemy(vector<enemy> &Enemies, player& player) {
	if (delete_enemy == 1) {
		for (int i = 0; i < Enemies.size(); i++) {
			int e_x = Enemies[i].get_x_pos();
			int e_y = Enemies[i].get_y_pos();
			int p_x = player.get_x_pos();
			int p_y = player.get_y_pos();
			if (e_x == p_x && e_y == p_y) {
				cout << "WOWWOWOOWOWW WHASAAATT?!! \n";
				cout << "encountered enemy " << i << "\n";
				Enemies.erase(Enemies.begin() + i);
				player.set_board();
				Sleep(1000);
				return i;
			}
		}
	}
}

int main() {
	
	srand(time(0));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			board[j][i] = EMPTY_MARK;
		}
	}
	player player;
	vector <enemy> Enemies(enemy_count);
	player.set_mark(PLAYER_MARK);
	print_board(); // Need to print board to update set_spawn
	player.set_spawn();
	spawn_enemy(Enemies);
	
	int x = 1;

	do {
		print_board();
		player.player_select_move(); 
		player.check_collision(0,0); //Player can move to items and exit, so 0 is added as prameters
		check_delete_enemy(Enemies, player); // Parameters are needed to check respective objects x and y positions.
		print_board();
		
		for (int i = 0; i < Enemies.size(); i++) {

			Enemies[i].enemy_select_move();
			Enemies[i].check_collision(EXIT_VAL, ITEM_VAL);
			check_delete_enemy(Enemies, player);
		}
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