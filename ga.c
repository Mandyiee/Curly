#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

void screenDimensionsWindows(int *screenWidth, int *screenHeight) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    *screenWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#define getScreenDimensions(x, y) screenDimensionsWindows(x, y)

#else
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>


int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void screenDimensionsUnix(int *screenWidth, int *screenHeight) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *screenWidth = w.ws_col;
    *screenHeight = w.ws_row;
}
#define getScreenDimensions(x, y) screenDimensionsUnix(x, y)
#endif

#define clearScreen() printf("\033[H\033[J")
#define SNAKE_MAX_LEN 256

int isGameOver = 1;
bool isFood = 1;
void printBoard(int x, int y);
int initializeBoard(int x, int y);
int initializeSnake(int x, int y);
void read_keyboard(void);
char **board;
char lastMove;
void setupFood (void);
void foodEaten (void);
void welcome (void);
void endGame (void);

int count = 0;

struct SnakePosition {
    int x;
    int y;
};

struct Snake {
    int length;
    struct SnakePosition position[SNAKE_MAX_LEN];
};

struct Snake snake;

struct foodPosition {
    int x;
    int y;
};

struct foodPosition food;

typedef struct Screen {
    int x;
    int y;
} screen;

screen gameScreen;

int main() {
    int screenWidth, screenHeight;
    getScreenDimensions(&screenWidth, &screenHeight);

    screenHeight -= 2;
    gameScreen.x = screenHeight;
    gameScreen.y = screenWidth;
    welcome();
    if (initializeBoard(screenHeight, screenWidth)) {
        clearScreen();
        printf("Malfunction");
        return 1;
    }
    if (initializeSnake(screenHeight, screenWidth)) {
        clearScreen();
        printf("Malfunction");
        return 1;
    }

    while (isGameOver) {
        clearScreen();
        printBoard(screenHeight, screenWidth);
        // use a variable to check for present food
        if (isFood) {
          setupFood();
        }
        read_keyboard();
        foodEaten();
    }
    return 0;
}

void welcome (void) {
  printf("Hey!!! \n");
  printf("Thid is a Snake game \n");
  printf("w - Up \n");
  printf("a - Left \n");
  printf("w - Right \n");
  printf("s - Down \n");
  printf("l - exit \n");
  printf("Press any key to continue \n");
  char ch = getch();
}

int initializeBoard (int x, int y) {
  clearScreen();
  board = (char **)malloc(x * sizeof(char*));
  
  if(board == NULL) {
  printf("Malfunction ");
  return 1;
  }
  
  for (int i = 0; i < x; i++) {
    board[i] = (char *)malloc(y * sizeof(char));
    if(board[i] == NULL) {
  printf("Malfunction ");
  return 1;
  }
  }
  
  for (int i = 0; i < x; i++) {
    for (int j = 0; j < y; j++) {
       if (i == 0  || j == 0 || j == (y - 1) || i == (x - 1)) {
         board[i][j] = '#';
       } else {
         board[i][j] = ' ';
       }
    }
  }
return 0;
}

void printBoard(int x, int y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    return;
}

int initializeSnake(int x, int y) {
    int snakeHeight = round(x / 2);
    int snakeWidth = round(y / 2);

    for (int i = 1; i < 3; i++) {
        board[snakeHeight][snakeWidth + i] = '+';
        snake.position[i + 1].x = snakeHeight;
        snake.position[i + 1].y = snakeWidth + i;
        //printf("%d, %d", snake.position[i + 1].y, snake.position[i + 1].x);
    }

    board[snakeHeight][snakeWidth] = '@';
    snake.position[0].x = snakeHeight;
    snake.position[0].y = snakeWidth;

    snake.length = 4;
    return 0;
}

void move_snake(int x, int y) {
    int newX = snake.position[0].x + x;
    int newY = snake.position[0].y + y;

    if (newX == 0 || newY == 0 || newY == (gameScreen.y - 1) || newX == (gameScreen.x - 1)) {
        
endGame();
    }

    board[snake.position[snake.length - 1].x][snake.position[snake.length - 1].y] = ' ';

    for (int i = snake.length - 1; i > 0; i--) {
        snake.position[i] = snake.position[i - 1];
    }

    snake.position[0].x = newX;
    snake.position[0].y = newY;

    for (int i = 0; i < snake.length; i++) {
        if (i == 0) {
            board[snake.position[i].x][snake.position[i].y] = '@';
        } else {
            board[snake.position[i].x][snake.position[i].y] = '+';
        }
    }
}

void read_keyboard(void) {
    char ch = getch();

    switch (ch) {
    case 'w':
        lastMove != 's' ? (move_snake(-1, 0), lastMove = ch) : (void)0;
        break;
    case 's':
        lastMove != 'w' ? (move_snake(1, 0), lastMove = ch) : (void)0;
        break;
    case 'a':
        lastMove != 'd' ? (move_snake(0, -1), lastMove = ch) : (void)0;
        break;
    case 'd':
        lastMove != 'a' ? (move_snake(0, 1), lastMove = ch) : (void)0;
        break;
    case 'l':
        exit(0);
        break;
    }
}

void setupFood (void) {
    srand(time(NULL));
    
    while(true) {
      int x = (rand() % (gameScreen.x - 3)) + 1;
    int y = (rand() % (gameScreen.y - 3)) + 1;
    
    bool isBody = true;
    for (int i = 0; i < snake.length; i++){

    if (snake.position[i].x == x && snake.position[i].y == y) {
        isBody = false;
    }
    }
    
    if (isBody) {
      board[x][y] = '*';
      isFood = 0;
      food.x = x;
      food.y = y;
        break;
    }
    }
}

void foodEaten (void) {
  if (snake.position[0].x == food.x && snake.position[0].y == food.y) {
  snake.length += 1;
  count += 1;
  isFood = 1;
  }
}

void endGame (void) {
  clearScreen();
  printf("Game Over ! \n");
  printf("You got %d points \n", count);
  exit(0);
}