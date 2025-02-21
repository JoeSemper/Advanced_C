#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <inttypes.h>
#include <string.h>
#include <unistd.h>

#define MIN_Y 2
#define N_CONTROLS 12
double DELAY = 0.1;

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
    START_TAIL_SIZE = 3,
    MAX_FOOD_SIZE = 20,
    FOOD_EXPIRE_SECONDS = 10,
    SEED_NUMBER = 3
};

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

struct food
{
    int x;
    int y;
    time_t put_time;
    char point;
    uint8_t enable;
} food[MAX_FOOD_SIZE];

void initFood(struct food f[], size_t size)
{
    struct food init = {0, 0, 0, 0, 0};
    for (size_t i = 0; i < size; i++)
    {
        f[i] = init;
    }
}

int getRandX()
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    return rand() % (max_x - 1);
}

int getRandY()
{
    int max_x = 0, max_y = 0;
    getmaxyx(stdscr, max_y, max_x);
    return rand() % (max_y - 2) + 1;
}

/*
 Обновить/разместить текущее зерно на поле
 */
void putFoodSeed(struct food *fp)
{
    char spoint[2] = {0};
    mvprintw(fp->y, fp->x, " ");
    fp->x = getRandX();
    fp->y = getRandY(); // Не занимаем верхнюю строку
    fp->put_time = time(NULL);
    fp->point = '$';
    fp->enable = 1;
    spoint[0] = fp->point;
    mvprintw(fp->y, fp->x, "%s", spoint);
}

/*
 Разместить еду на поле
 */
void putFood(struct food f[], size_t number_seeds)
{
    for (size_t i = 0; i < number_seeds; i++)
    {
        putFoodSeed(&f[i]);
    }
}

void refreshFood(struct food f[], int nfood)
{
    for (size_t i = 0; i < nfood; i++)
    {
        if (f[i].put_time)
        {
            if (!f[i].enable || (time(NULL) - f[i].put_time) > FOOD_EXPIRE_SECONDS)
            {
                putFoodSeed(&f[i]);
            }
        }
    }
}

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

_Bool haveEat(struct snake_t *head, struct food f[])
{
    for (int i = 0; i < MAX_FOOD_SIZE; i++)
    {
        if (head->x == f[i].x && head->y == f[i].y)
        {
            f[i].enable = 0;
            return 1;
        }
    }

    return 0;
}

/*
 Увеличение хвоста на 1 элемент
 */

void addTail(struct snake_t *head)
{
    head->tsize++;
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

void repairSeed(struct food f[], size_t nfood, struct snake_t *head)
{
    for (size_t i = 0; i < head->tsize; i++)
        for (size_t j = 0; j < nfood; j++)
        {
            while (head->tail[i].x == f[j].x && head->tail[i].y == f[j].y)
            {
                f[j].x = getRandX();
                f[j].y = getRandY();
            }
        }
    for (size_t i = 0; i < nfood; i++)
        for (size_t j = 0; j < nfood; j++)
        {
            while (i != j && f[i].x == f[j].x && f[i].y == f[j].y)
            {
                f[j].x = getRandX();
                f[j].y = getRandY();
            }
        }
}

void update(struct snake_t *head, struct food f[], const int32_t key)
{
    clock_t begin = clock();
    go(head);
    goTail(head);
    changeDirection(head, key);
    refreshFood(food, SEED_NUMBER); // Обновляем еду
    if (haveEat(head, food))
    {
        addTail(head);
    }
    refresh(); // Обновление экрана, вывели кадр анимации
    while ((double)(clock() - begin) / CLOCKS_PER_SEC < DELAY)
    {
    }
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
    initFood(food, MAX_FOOD_SIZE);
    putFood(food, SEED_NUMBER); // Кладем зерна
    int key_pressed = 0;

    while (key_pressed != STOP_GAME)
    {
        key_pressed = getch(); // Считываем клавишу

        if (key_pressed == PAUSE)
            pause(&key_pressed);

        update(snake, food, key_pressed);
        repairSeed(food, SEED_NUMBER, snake);

        if (checkTailCollision(snake))
        {
            mvprintw(1, 0, "Game over");
            while (key_pressed != STOP_GAME)
            {
                key_pressed = getch();
            };
        }
    }

    free(snake->tail);
    free(snake);
    endwin(); // Завершаем режим curses mod
    return 0;
}