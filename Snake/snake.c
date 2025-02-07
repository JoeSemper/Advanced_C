#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y 2
#define N_CONTROLS 12
enum
{
    LEFT = 1,
    UP,
    RIGHT,
    DOWN,
    STOP_GAME = KEY_F(10),
    PAUSE = KEY_F(9),
};
enum
{
    MAX_TAIL_SIZE = 100,
    START_TAIL_SIZE = 15,
    MAX_FOOD_SIZE = 20,
    FOOD_EXPIRE_SECONDS = 10
};

// // Здесь храним коды управления змейкой
// struct control_buttons
// {
//     int down[N_CONTROLS];
//     int up[N_CONTROLS];
//     int left[N_CONTROLS];
//     int right[N_CONTROLS];
// } control_buttons;

// struct control_buttons default_controls = {
//     {KEY_DOWN, 'S', 's'},
//     {KEY_UP, 'W', 'w'},
//     {KEY_LEFT, 'A', 'a'},
//     {KEY_RIGHT, 'D', 'd'}};

int default_controls[N_CONTROLS] = {KEY_LEFT, 'A', 'a', KEY_UP, 'W', 'w', KEY_RIGHT, 'D', 'd', KEY_DOWN, 'S', 's'};

/*
 Голова змейки содержит в себе
 x,y - координаты текущей позиции
 direction - направление движения
 tsize - размер хвоста
 *tail -  ссылка на хвост
 */
typedef struct snake_t
{
    int x;
    int y;
    int direction;
    size_t tsize;
    struct tail_t *tail;
    int *controls;
} snake_t;

/*
 Хвост это массив состоящий из координат x,y
 */
typedef struct tail_t
{
    int x;
    int y;
} tail_t;

void initTail(struct tail_t t[], size_t size, int head_x, int head_y)
{
    struct tail_t init_t = {0, 0};
    for (size_t i = 0; i < size; i++)
    {
        t[i] = init_t;
    }
    for (size_t i = 0; i < START_TAIL_SIZE; i++)
    {
        t[i].x = head_x - i;
        t[i].y = head_y;
    }
}
void initHead(struct snake_t *head, int x, int y)
{
    head->x = x;
    head->y = y;
    head->direction = RIGHT;
}

void initSnake(snake_t *head, size_t size, int x, int y)
{
    tail_t *tail = (tail_t *)malloc(MAX_TAIL_SIZE * sizeof(tail_t));
    initTail(tail, MAX_TAIL_SIZE, x, y);
    initHead(head, x, y);
    head->tail = tail; // прикрепляем к голове хвост
    head->tsize = size + 1;
    head->controls = default_controls;
}

/*
 Движение головы с учетом текущего направления движения
 */
void go(struct snake_t *head)
{
    char ch = '@';
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);  // macro - размер терминала
    mvprintw(head->y, head->x, " "); // очищаем один символ
    switch (head->direction)
    {
    case LEFT:
        if (head->x <= 0)
            head->x = max_x;
        mvprintw(head->y, --(head->x), "%c", ch);
        break;
    case RIGHT:
        if (head->x >= max_x)
            head->x = 0;
        mvprintw(head->y, ++(head->x), "%c", ch);
        break;
    case UP:
        if (head->y <= MIN_Y)
            head->y = max_y;
        mvprintw(--(head->y), head->x, "%c", ch);
        break;
    case DOWN:
        if (head->y >= max_y)
            head->y = MIN_Y;
        mvprintw(++(head->y), head->x, "%c", ch);
        break;
    default:
        break;
    }
    refresh();
}

int getDirection(int32_t key, int *control_buttons)
{
    for (int i = 0; i < N_CONTROLS; i++)
    {
        if (key == control_buttons[i])
        {
            switch (i / 3 + 1)
            {
            case 1:
                return LEFT;
                break;
            case 2:
                return UP;
                break;
            case 3:
                return RIGHT;
                break;
            case 4:
                return DOWN;
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

int checkDirection(snake_t *snake, int32_t key)
{
    int new_direction = getDirection(key, snake->controls);

    switch (snake->direction)
    {
    case LEFT:
        if (new_direction == RIGHT)
            return 0;
        break;
    case RIGHT:
        if (new_direction == LEFT)
            return 0;
        break;
    case UP:
        if (new_direction == DOWN)
            return 0;
        break;
    case DOWN:
        if (new_direction == UP)
            return 0;
        break;
    default:
        return 1;
    }
}

void changeDirection(struct snake_t *snake, const int32_t key)
{
    if (checkDirection(snake, key) && getDirection(key, snake->controls))
    {
        snake->direction = getDirection(key, snake->controls);
    }
}

/*
 Движение хвоста с учетом движения головы
 */
void goTail(struct snake_t *head)
{
    char ch = '*';
    mvprintw(head->tail[head->tsize - 1].y, head->tail[head->tsize - 1].x, " ");
    for (size_t i = head->tsize - 1; i > 0; i--)
    {
        head->tail[i] = head->tail[i - 1];
        if (head->tail[i].y || head->tail[i].x)
            mvprintw(head->tail[i].y, head->tail[i].x, "%c", ch);
    }
    head->tail[0].x = head->x;
    head->tail[0].y = head->y;
}

void pause(int *key_pressed)
{
    *key_pressed = 0;
    mvprintw(1, 0, "Pause");
    while (*key_pressed != PAUSE)
    {
        *key_pressed = getch();
    };
    mvprintw(1, 0, "     ");
}

int checkTailCollision(struct snake_t *snake)
{

    for (int i = 1; i < snake->tsize; i++)
    {
        if (snake->x == snake->tail[i].x && snake->y == snake->tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}


int main()
{
    snake_t *snake = (snake_t *)malloc(sizeof(snake_t));
    initSnake(snake, START_TAIL_SIZE, 20, 10);
    initscr();
    keypad(stdscr, TRUE); // Включаем F1, F2, стрелки и т.д.
    raw();                // Отключаем line buffering
    noecho();             // Отключаем echo() режим при вызове getch
    curs_set(FALSE);      // Отключаем курсор
    mvprintw(0, 0, "Use arrows for control. Press 'F10' for EXIT. Press 'F9' to PAUSE");
    timeout(0); // Отключаем таймаут после нажатия клавиши в цикле
    int key_pressed = 0;
    while (key_pressed != STOP_GAME)
    {
        key_pressed = getch(); // Считываем клавишу
        if (key_pressed == PAUSE)
        {
            pause(&key_pressed);
        }
        go(snake);
        goTail(snake);
        if (checkTailCollision(snake))
        {
            mvprintw(1, 0, "Game over");
            while (key_pressed != STOP_GAME)
            {
                key_pressed = getch();
            };
        }
        timeout(100); // Задержка при отрисовке
        changeDirection(snake, key_pressed);
    }
    free(snake->tail);
    free(snake);
    endwin(); // Завершаем режим curses mod
    return 0;
}