#include <iostream>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <ncurses.h>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <ostream>
#include <stdint.h>


using namespace std;

int WIDTH = 40;
int HEIGHT = 20;

bool isRunningApp;
// Структура для координат
struct Point {
    int x, y;
};

vector<Point> snake;
Point food;
int direction;
bool gameOver;

// Инициализация игры
void initGame(){
    gameOver = false;
    direction = 1; // Начальное направление — вправо
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    
    // Инициализация ncurses
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    
    // Генерация первой еды
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;
}

// Отрисовка поля
void draw() {
    clear();
    
    // Рисуем границы
    for (int x = 0; x < WIDTH; x++) {
        mvprintw(0, x, "#");
        mvprintw(HEIGHT - 1, x, "#");
    }
    for (int y = 0; y < HEIGHT; y++) {
        mvprintw(y, 0, "#");
        mvprintw(y, WIDTH - 1, "#");
    }
    
    // Рисуем еду
    mvprintw(food.y, food.x, "@");
    
    // Рисуем змейку
    for (size_t i = 0; i < snake.size(); i++) {
        if (i == 0) {
            mvprintw(snake[i].y, snake[i].x, "O"); // Голова
        } else {
            mvprintw(snake[i].y, snake[i].x, "o"); // Тело
        }
    }
    
    refresh();
}

// Обработка ввода
void input() {
    int key = getch();
    
    switch (key) {
        case 'w':
        case KEY_UP:
            if (direction != 2) direction = 0;
            break;
        case 'd':
        case KEY_RIGHT:
            if (direction != 3) direction = 1;
            break;
        case 's':
        case KEY_DOWN:
            if (direction != 0) direction = 2;
            break;
        case 'a':
        case KEY_LEFT:
            if (direction != 1) direction = 3;
            break;
        case 'x':
        case 'q':
            gameOver = true;
            break;
    }
}

// Логика игры
void logic() {
    // Сохраняем текущую голову
    Point newHead = snake[0];

    // Обновляем позицию головы
    switch (direction) {
        case 0: newHead.y--; break;
        case 1: newHead.x++; break; 
        case 2: newHead.y++; break; 
        case 3: newHead.x--; break; 
    }

    // Проверка столкновения с границами
    if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || newHead.y <= 0 || newHead.y >= HEIGHT - 1) {
        gameOver = true;
        return;
    }

    
    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
            gameOver = true;
            return;
        }
    }

    // Добавляем новую голову
    snake.insert(snake.begin(), newHead);

    // Проверка поедания еды
    if (newHead.x == food.x && newHead.y == food.y) {
        // Генерируем новую еду (не на теле змейки)
        bool foodOnSnake;
        do {
            foodOnSnake = false;
            food.x = rand() % (WIDTH - 2) + 1;
            food.y = rand() % (HEIGHT - 2) + 1;
            
            for (const auto& segment : snake) {
                if (segment.x == food.x && segment.y == food.y) {
                    foodOnSnake = true;
                    break;
                }
            }
        } while (foodOnSnake);
    } else {
        snake.pop_back();
    }
}

int main() {
    isRunningApp = true;
    string menuChoice;
    srand(time(0));
    while (isRunningApp)
    {
        cout << "1.Start\n"
                    "2.Exit" << endl;
        cout << "Select option:";
        cin >> menuChoice;
        
        if (menuChoice == "Start" || menuChoice == "1" || menuChoice == "start"){
            system("clear");
            initGame();
            while (!gameOver)
            {
                    draw();
                    input();
                    logic();
                    this_thread::sleep_for(chrono::milliseconds(100));
            }
        endwin(); // Завершить ncurses
        cout << "Game Over! Score: " << snake.size() - 1 << endl;
    }
    else if (menuChoice == "exit" || menuChoice == "Exit" || menuChoice == "2"){
            break;
        }
    else {
            cerr << "error: No such" << endl;
        }
    }
    return 0;
}