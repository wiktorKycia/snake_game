#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

const size_t height = 10;
const size_t width = 50;
const char snake_piece = 'o';
int score = 0;

struct termios orig_termios;

void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios); // Zapisz oryginalne ustawienia
    
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);        // Wyłącz tryb kanoniczny i echo
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
}

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios); // Przywróć oryginał
}

typedef struct
{
    int x;
    int y;
} Piece_position;

typedef struct
{
    Piece_position* pieces;
    size_t count;
    size_t max_size; 
} Snake;

void print_display(const char display[height][width], Snake *snake)
{
    printf("+");
    for(size_t i = 0; i < width; i++)
    {
        printf("-");
    }
    printf("+\n");
    for (size_t i = 0; i < height; i++)
    {
        printf("|");
        for(size_t j = 0; j < width; j++)
        {
            // check if i j are in piece_positions of snake, if yes -> print 'o'
            if (display[i][j] == 0 || display[i][j] == ' ')
            {
                printf(" ");
            }
            else
            {
                printf("%c", display[i][j]);
            }
            
        }
        printf("|\n");
    }
    printf("+");
    for(size_t i = 0; i < width; i++)
    {
        printf("-");
    }
    printf("+\n");
}

enum Direction {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

bool handle_input(enum Direction *direction, int *x, int *y)
{
    enable_raw_mode();
    char input;
    read(STDIN_FILENO, &input, sizeof input);
    switch (input)
    {
    case 'q':
        return true;
    case 'w':
        if (*direction != DOWN)
            *direction = UP;
        break;
    
    case 's':
        if (*direction != UP)
            *direction = DOWN;
        break;
    
    case 'd':
        if (*direction != LEFT)
            *direction = RIGHT;
        break;

    case 'a':
        if (*direction != RIGHT)
            *direction = LEFT;
        break;
    default:
        break;
    }
    disable_raw_mode();
    switch (*direction)
    {
    case RIGHT:
        (*x)++;
        break;
    
    case LEFT:
        (*x)--;
        break;
    
    case UP:
        (*y)--;
        break;
    
    case DOWN:
        (*y)++;
        break;

    default:
        break;
    }
    return false;
}



Snake* alloc_snake(Piece_position starting_pos)
{
    size_t max_size = 2;
    size_t count = 1;
    Piece_position* pieces;
    pieces = (Piece_position*)malloc(max_size * sizeof(Piece_position));
    // memset(pieces, 0, sizeof(Piece_position));

    pieces[0] = starting_pos;

    Snake *snake = malloc(sizeof *snake);

    if (snake == NULL) // w c++ byłoby tu nullptr, bo malloc zwraca wskaźnik
    {
        free(pieces);
        return NULL;
    }

    snake->pieces = pieces;
    snake->count = count;
    snake->max_size = max_size;

    return snake;
}

void add_piece_to_snake(Snake *snake, Piece_position piece_position)
{
    if(snake->count >= snake->max_size)
    {
        snake->max_size *= 2;
        snake->pieces = realloc(snake->pieces, snake->max_size * sizeof(Piece_position));
    }
    snake->pieces[snake->count] = piece_position;
    snake->count++;
}

void remove_first_piece_of_snake(Snake *snake)
{
    for (size_t i = 0; i < snake->count - 1; i++)
    {
        snake->pieces[i] = snake->pieces[i+1];
    }
    snake->count--;
}

bool move_snake(char display[height][width], Snake *snake, int x, int y)
{
    // if snake moves out of bounds or in itself -> gameover
    if (x < 0 || y < 0 || x >= width || y >= height || display[y][x] == 'o') // checking == 'o' here will create a bug, where the snake bumps itself into a previous tail's location, that now just moved, but is not displayed yet
    {
        return true;
    }
    if (display[y][x] == '*')
    {
        score++;
        Piece_position piece_position = {x, y};
        add_piece_to_snake(snake, piece_position);
        display[y][x] = 'o';

        // randomize next apple
        int x_apple = rand() % width;
        int y_apple = rand() % height;
        display[y_apple][x_apple] = '*';
    }
    else
    {
        Piece_position piece_position = {x, y};
        display[y][x] = 'o';
        add_piece_to_snake(snake, piece_position);
        display[snake->pieces[0].y][snake->pieces[0].x] = ' ';
        remove_first_piece_of_snake(snake);
    }
    return false;
}

int main()
{
    // initialize board
    char display[height][width];
    memset(display, 0, sizeof display);

    // initial snake head coordinates and direction
    int y = (int)height/2;
    int x = (int)width/2;

    enum Direction current_direction = RIGHT;

    Piece_position starting_head_position = {x, y};
    Snake *snake = alloc_snake(starting_head_position);

    // randomize first apple
    srand(time(NULL));
    int x_apple = rand() % width;
    int y_apple = rand() % height;
    display[y_apple][x_apple] = '*';

    for(;;)
    {
        bool quit = handle_input(&current_direction, &x, &y);
        if(quit) return 0;
        
        bool game_over = move_snake(display, snake, x, y);
        if(game_over) break;

        // clear console and print game info
        system("clear");
        printf("Controls: WASD\nQ - quit\n");
        printf("Score: %d\n", score);

        // display the board
        print_display(display, snake);
        
        usleep(1000 * 1000);//microseconds
    }
    system("clear");
    printf("GAME OVER\n");
    return 0;
}
