#include <time.h>
#include <raylib.h>

#include <vector>
#include <array>

// Game config
#define WIDTH           20
#define HEIGHT          20
#define INITIAL_X       5
#define INITIAL_Y       5
#define INITIAL_LENGHT  3

// Display config
#define CELL_SIZE   32

// Color config
#define BORDER_COLOR        BLACK
#define BACKGROUND_COLOR    WHITE
#define SNAKE_COLOR         GREEN
#define HEAD_COLOR          DARKGREEN
#define APPLE_COLOR         RED

// Controls config
#define GO_UP       KEY_W
#define GO_DOWN     KEY_S
#define GO_LEFT     KEY_A
#define GO_RIGHT    KEY_D

enum direction {UP, DOWN, LEFT, RIGHT, NONE};

bool IsBorder(int x, int y);
bool IsOnSnake(int x, int y, std::vector<std::pair<int, int>> *snake);
bool IsOnSnakeExceptHead(int x, int y, std::vector<std::pair<int, int>> *snake);
void RandomizeApple(std::pair<int, int> *apple, std::vector<std::pair<int, int>> *snake);

int main() {

    std::vector<std::pair<int, int>> snake(INITIAL_LENGHT);
    std::pair<int, int> apple;

    direction direction = NONE;

    SetRandomSeed(time(NULL));

    for (int i = 0; i < INITIAL_LENGHT; i++) {
        snake[i].first = INITIAL_X - i;
        snake[i].second = INITIAL_Y;
    }

    RandomizeApple(&apple, &snake);

    InitWindow(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE, "SN");
    SetTargetFPS(10);

    while (!WindowShouldClose() 
        && !IsBorder(snake[0].first, snake[0].second) 
        && !IsOnSnakeExceptHead(snake[0].first, snake[0].second, &snake)) {

        if (IsKeyPressed(GO_UP) && direction != DOWN) {
            direction = UP;
        }

        else if (IsKeyPressed(GO_DOWN) && direction != UP) {
            direction = DOWN;
        }

        else if (IsKeyPressed(GO_LEFT) && direction != RIGHT) {
            direction = LEFT;
        }

        else if (IsKeyPressed(GO_RIGHT) && direction != LEFT) {
            direction = RIGHT;
        }

        if (direction != NONE) {
            for (int i = snake.size() - 1; i > 0; i--) {
                snake[i].first = snake[i - 1].first;
                snake[i].second = snake[i - 1].second;
            }
        }

        switch (direction)
        {
            case UP:
                snake[0].second -= 1;
                break;
            
            case DOWN:
                snake[0].second += 1;
                break;
            
            case LEFT:
                snake[0].first -= 1;
                break;

            case RIGHT:
                snake[0].first += 1;
                break;
        }

        if (snake[0].first == apple.first && snake[0].second == apple.second) {
            snake.push_back(snake[snake.size() - 1]);
            RandomizeApple(&apple, &snake);
        }

        BeginDrawing();

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, 
                    (apple.first == x && apple.second == y) ? APPLE_COLOR : 
                    (IsBorder(x, y) ? BORDER_COLOR : BACKGROUND_COLOR)
                );
            }
        }

        for (int i = snake.size() - 1; i >= 0; i--) {
            DrawRectangle(snake[i].first * CELL_SIZE, snake[i].second * CELL_SIZE, CELL_SIZE, CELL_SIZE,
                i == 0 ? HEAD_COLOR : SNAKE_COLOR
            );
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

bool IsBorder(int x, int y) {
    return (x == 0 || y == 0 || x == WIDTH - 1 || y == HEIGHT - 1);
}

bool IsOnSnake(int x, int y, std::vector<std::pair<int, int>> *snake) {
    return IsOnSnakeExceptHead(x, y, snake) || (snake->at(0).first == x && snake->at(0).second == y);
}

bool IsOnSnakeExceptHead(int x, int y, std::vector<std::pair<int, int>> *snake) {
    bool test = false;
    for (int i = 1; i < snake->size() && !test; i++) {
        test = snake->at(i).first == x && snake->at(i).second == y;
    }
    return test;
}

void RandomizeApple(std::pair<int, int> *apple, std::vector<std::pair<int, int>> *snake) {
    do {
        apple->first = GetRandomValue(0, WIDTH - 1);
        apple->second = GetRandomValue(0, HEIGHT - 1);
    } while (IsBorder(apple->first, apple->second) || IsOnSnake(apple->first, apple->second, snake));
}