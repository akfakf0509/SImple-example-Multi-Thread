#include <iostream>
#include <thread>
#include <queue>
#include <conio.h>
#include <Windows.h>

using namespace std;

typedef enum Direction { up, down, left, right }Direction;
typedef struct Vector2 { int x; int y; };

int map[10][10] = { {1, }, };
Vector2 player_position = Vector2{ 0, 0 };
Direction player_direction = Direction::up;

void PrintMap();
void MovePlayer(int);
void Fire();

int main() {	
	char key;

	CONSOLE_CURSOR_INFO cursorinfo = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorinfo);

	PrintMap();
	do {
		key = _getch();
		MovePlayer(key);
	} while (key != 'q');
}

void PrintMap() {
	thread th([]() {
		while (1) {
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 });
			for (int j = 0; j < 10; j++) {
				for (int i = 0; i < 10; i++) {
					switch (map[j][i]) {
					case 0:
						cout << "  ";
						break;
					case 1:
						cout << "¡Ü";
						break;
					case 100:
						cout << "¡è";
						break;
					case 101:
						cout << "¡é";
						break;
					case 102:
						cout << "¡ç";
						break;
					case 103:
						cout << "¡æ";
						break;
					}
				}
				cout << endl;
			}
			cout << player_position.x << " " << player_position.y;
		}
	});

	th.detach();
}

void MovePlayer(int key) {
	switch (key) {
	case 'w':
		player_direction = Direction::up;
		if (player_position.y - 1 >= 0) {
			map[player_position.y][player_position.x] = 0;
			map[--player_position.y][player_position.x] = 1;
		}
		break;
	case 's':
		player_direction = Direction::down;
		if (player_position.y + 1 < 10) {
			map[player_position.y][player_position.x] = 0;
			map[++player_position.y][player_position.x] = 1;
		}
		break;
	case 'd':
		player_direction = Direction::right;
		if (player_position.x + 1 < 10) {
			map[player_position.y][player_position.x] = 0;
			map[player_position.y][++player_position.x] = 1;
		}
		break;
	case 'a':
		player_direction = Direction::left;
		if (player_position.x - 1 >= 0) {
			map[player_position.y][player_position.x] = 0;
			map[player_position.y][--player_position.x] = 1;
		}
		break;
	case ' ':
		thread th(Fire);

		th.detach();
		break;
	}
}

void Fire() {
	queue<Vector2> particle;
	Vector2 point = player_position;
	Direction direction = player_direction;

	for (int i = 0; i < 10; i++) {
		switch (direction)
		{
		case Direction::up:
			if (--point.y >= 0) {
				map[point.y][point.x] = 100;
				particle.push(point);
			}
			break;
		case Direction::down:
			if (++point.y < 10) {
				map[point.y][point.x] = 101;
				particle.push(point);
			}
			break;
		case Direction::left:
			if (--point.x >= 0) {
				map[point.y][point.x] = 102;
				particle.push(point);
			}
			break;
		case Direction::right:
			if (++point.x < 10) {
				map[point.y][point.x] = 103;
				particle.push(point);
			}
			break;
		}
		Sleep(50);
	}
	Sleep(100);
	while (!particle.empty()) {
		if(100 <= map[particle.front().y][particle.front().x] && map[particle.front().y][particle.front().x] <= 103)
			map[particle.front().y][particle.front().x] = 0;
		particle.pop();
		Sleep(50);
	}
}