#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LEFT_WARD 75
#define RIGHT_WARD 77
#define UP_WARD 72
#define DOWN_WARD 80
#define HOLD_KEY_C 99
#define SPIN_KEY_Z 122
#define HARD_DROP_SPACE 32

enum direction {
	LEFT,
	RIGHT,
	DOWN,
	UP
};
enum blockState {
	EMPTY,
	SOFT_BLOCK,
	HARD_BLOCK,
	WALL
};

struct block {
	int id;
	int rotationState;
	int y, x;
};

const int blockShapes[8][4][4][4] = {
	// ■
	// ■ ■ ■
	0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
	//     ■
	// ■ ■ ■
	0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
	// ■ ■
	// ■ ■
	0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0,
	// ■ ■ ■ ■
	0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0,
	//   ■
	// ■ ■ ■
	0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0,
	// ■ ■
	//   ■ ■
	0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	//   ■ ■
	// ■ ■
	0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	// a block for nothing(for hold feature)
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void gotoyx(int y, int x) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void createRandomBlock(struct block* b);
void initBlock(struct block* b);
void drawPreparingBlock(struct block b);
void drawHoldingBlock(struct block b);
void drawBlock(struct block b, enum blockState type);
void eraseBlock(struct block b);
int isNowConflict(enum blockState map[24][12], struct block b);
int willMoveConflict(enum direction way, enum blockState map[24][12], struct block b);
void moveBlock(enum direction way, struct block* b);
void destroyLine(enum blockState map[24][12], int line);
void rePrintMapTo(enum blockState map[24][12], int y);
void printScore(int score);
void printStage(int stage);
void loadNextBlock(struct block* currentBlock, struct block* preparingBlock);
//additional functions for opponent
void handleSocket(char msg[]);
struct block makeOpponentBlock(struct block b);
void printScoreOfOpponent(int score);
void printStageOfOpponent(int stage);


int main(void) {
	int i, j, x, y, timer, score = 0, stage = 1, scoreForNextLevel = 1500, speed = 1000, isHold, isGamePlayable = 1, *cacheForRotation, *counter, *numOfScoreLines;
	struct block preparingBlock, currentBlock, blockCache, holdingBlock;
	enum blockState map[24][12] = { EMPTY };

	system("cls");

	CONSOLE_CURSOR_INFO Curinfo;
	Curinfo.dwSize = 1;
	Curinfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Curinfo);
	// removing cursor

	for (i = 0; i < 12; i++)
		map[23][i] = WALL;
	for (i = 0; i < 24; i++) {
		map[i][0] = WALL;
		map[i][11] = WALL;
	}
	holdingBlock.id = 7;
	holdingBlock.rotationState = 0;
	srand(time(NULL));
	for (i = 0; i < 23; i++) {
		printf("▒                    ▒");
		for (j = 0; j < 8; j++)
			printf("  ");

		printf("▒                    ▒");
		puts("");
	}
	printf("▒▒▒▒▒▒▒▒▒▒▒▒                ▒▒▒▒▒▒▒▒▒▒▒▒");
	printStage(stage);
	printScore(score);
	createRandomBlock(&preparingBlock);
	// setting up stage

	while (isGamePlayable) { // loop for whole game
		isHold = 0;
		loadNextBlock(&currentBlock, &preparingBlock);
		drawHoldingBlock(holdingBlock);
		drawBlock(currentBlock, SOFT_BLOCK);
		while (!willMoveConflict(DOWN, map, currentBlock)) { // loop for a falling block
			eraseBlock(currentBlock);
			moveBlock(DOWN, &currentBlock);
			drawBlock(currentBlock, SOFT_BLOCK);
			timer = clock();
			while (clock() - timer < speed) {
				Sleep(1); // hack for optimization
				while (_kbhit()) {
					eraseBlock(currentBlock);
					switch (getch()) {
					case LEFT_WARD: // move left
						if (!willMoveConflict(LEFT, map, currentBlock))
							moveBlock(LEFT, &currentBlock);
						break;
					case RIGHT_WARD: // move right
						if (!willMoveConflict(RIGHT, map, currentBlock))
							moveBlock(RIGHT, &currentBlock);
						break;
					case DOWN_WARD: // move down
						if (!willMoveConflict(DOWN, map, currentBlock))
							moveBlock(DOWN, &currentBlock);
						break;
					case UP_WARD: // rotate block
						cacheForRotation = malloc(sizeof(int));
						*cacheForRotation = currentBlock.rotationState;
						currentBlock.rotationState++;
						currentBlock.rotationState %= 4;
						if (isNowConflict(map, currentBlock))
							currentBlock.rotationState = *cacheForRotation;
						else
							eraseBlock(currentBlock);
						free(cacheForRotation);
						break;
					case SPIN_KEY_Z:
						cacheForRotation = malloc(sizeof(int));
						*cacheForRotation = currentBlock.rotationState;
						if (!currentBlock.rotationState)
							currentBlock.rotationState = 3;
						else
							currentBlock.rotationState--;
						if (isNowConflict(map, currentBlock))
							currentBlock.rotationState = *cacheForRotation;
						else
							eraseBlock(currentBlock);
						free(cacheForRotation);
						break;
					case HOLD_KEY_C:
						if (!isHold) {
							isHold = 1;
							blockCache = holdingBlock;
							holdingBlock = currentBlock;
							holdingBlock.y = 3;
							holdingBlock.x = 4;
							drawHoldingBlock(holdingBlock);
							if (blockCache.id == 7) {
								loadNextBlock(&currentBlock, &preparingBlock);
								break;
							}
							currentBlock = blockCache;
						}
						break;
					case HARD_DROP_SPACE:
						while (!willMoveConflict(DOWN, map, currentBlock))
							moveBlock(DOWN, &currentBlock);
						timer = -1; // to stop the loop
					}
					drawBlock(currentBlock, SOFT_BLOCK);
				}
			}
		}
		for (i = currentBlock.y, y = 0; i < currentBlock.y + 4; i++, y++)
			for (j = currentBlock.x, x = 0; j < currentBlock.x + 4; j++, x++)
				if (blockShapes[currentBlock.id][currentBlock.rotationState][y][x])
					map[i][j] = HARD_BLOCK;
		// put hard block to map
		eraseBlock(currentBlock);
		for (i = 1; i < 11; i++)
			if (map[4][i] == HARD_BLOCK) {// check if it's game over
				isGamePlayable = 0;
				break;
			}
		drawBlock(currentBlock, HARD_BLOCK);
		counter = malloc(sizeof(int));
		numOfScoreLines = malloc(sizeof(int));
		for (i = currentBlock.y, *numOfScoreLines = 0; i < currentBlock.y + 4; i++) {
			for (j = 1, *counter = 0; j <= 10; j++)
				if (map[i][j] == HARD_BLOCK)
					(*counter)++;
			if ((*counter) == 10) { // check if it's able to score
				destroyLine(map, i);
				rePrintMapTo(map, i);
				(*numOfScoreLines)++;
			}
		}
		if (*numOfScoreLines) { // if able to score
			score += 100 * ((*numOfScoreLines) * 2 - 1); // 100 * lines + bonus score
			printScore(score);
			if (score >= scoreForNextLevel) { // adding stage
				scoreForNextLevel *= 2;
				printStage(++stage);
				speed /= 2;
			}
		}
		free(counter);
		free(numOfScoreLines);
	}
	system("cls");
	system("pause");
	return 0;
}
void createRandomBlock(struct block* b) {
	b->id = rand() % 7;
	b->rotationState = rand() % 4;
	initBlock(b);
}
void initBlock(struct block* b) {
	b->y = 2;
	b->x = 4;
}
void drawPreparingBlock(struct block b) {
	int i;
	gotoyx(0, 22);
	printf("▒▒▒▒▒▒");
	for (i = 0; i < 4;) {
		gotoyx(++i, 22);
		puts("▒        ▒");
	}
	gotoyx(i + 1, 22);
	printf("▒▒▒▒▒▒");
	b.y = 1;
	b.x = 12;
	eraseBlock(b);
	drawBlock(b, SOFT_BLOCK);
}
void drawHoldingBlock(struct block b) {
	int i;
	gotoyx(5, 22);
	printf("▒▒▒▒▒▒");
	for (i = 1; i < 5; i++) {
		gotoyx(5 + i, 22);
		puts("▒        ▒");
	}
	gotoyx(5 + i, 22);
	printf("▒▒▒▒▒▒");
	b.y = 6;
	b.x = 12;
	eraseBlock(b);
	drawBlock(b, SOFT_BLOCK);
}
void drawBlock(struct block b, enum blockState type) {
	int i, j, y, x;
	for (i = b.y, y = 0; i < b.y + 4; i++, y++)
		for (j = b.x * 2, x = 0; j < b.x * 2 + 8; j += 2, x++) {
			gotoyx(i, j);
			if (blockShapes[b.id][b.rotationState][y][x] != EMPTY)
				switch (type) {
				case SOFT_BLOCK:
					printf("□");
					break;
				case HARD_BLOCK:
					printf("■");
				}
			else
				gotoyx(i, j + 2);
		}
}
void eraseBlock(struct block b) {
	int i, j, y, x;
	for (i = b.y, y = 0; i < b.y + 4; i++, y++)
		for (j = b.x * 2, x = 0; j < b.x * 2 + 8; j += 2, x++) {
			gotoyx(i, j);
			blockShapes[b.id][b.rotationState][y][x] == SOFT_BLOCK ? printf("  ") : gotoyx(i, j + 2);
		}
}
int isNowConflict(enum blockState map[24][12], struct block b) {
	int i, j, y, x;
	for (i = b.y, y = 0; i < b.y + 4; i++, y++)
		for (j = b.x, x = 0; j < b.x + 4; j++, x++)
			if (map[i][j] != EMPTY && blockShapes[b.id][b.rotationState][y][x] == SOFT_BLOCK) // if wall and falling block conflicts
				return 1;
	return 0;
}
int willMoveConflict(enum direction way, enum blockState map[24][12], struct block b) {
	moveBlock(way, &b);
	return isNowConflict(map, b);
}
void moveBlock(enum direction way, struct block* b) {
	switch (way) {
	case LEFT:
		(b->x)--;
		break;
	case RIGHT:
		(b->x)++;
		break;
	case DOWN:
		(b->y)++;
	}
}
void destroyLine(enum blockState map[24][12], int line) {
	int i, j;
	for (i = line; i != 2; i--)
		for (j = 1; j <= 10; j++)
			map[i][j] = map[i - 1][j];
}
void rePrintMapTo(enum blockState map[24][12], int y) {
	int i, j, timer;
	for (i = 1; i < 11; i++) {
		timer = clock();
		while (clock() - timer < 20);
		gotoyx(y, i * 2);
		printf("□");
	}
	for (i = 1; i <= y; i++)
		for (j = 1; j < 11; j++) {
			gotoyx(i, j * 2);
			if (map[i][j] == EMPTY)
				printf("  ");
			else if (map[i][j] == HARD_BLOCK)
				printf("■");
		}
}
void printScore(int score) {
	gotoyx(14, 26);
	printf("Score: %d", score);
}
void printStage(int stage) {
	gotoyx(16, 26);
	printf("Stage: %d", stage);
}
void loadNextBlock(struct block* currentBlock, struct block* preparingBlock) {
	*currentBlock = *preparingBlock;
	createRandomBlock(preparingBlock);
	drawPreparingBlock(*preparingBlock);
}
struct block makeOpponentBlock(struct block b) {
	b.x += 20;
	return b;
}
void printScoreOfOpponent(int score) {
	gotoyx(14, 66);
	printf("Score: %d", score);
}
void printStageOfOpponent(int stage) {
	gotoyx(16, 66);
	printf("Stage: %d", stage);
}
