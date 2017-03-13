#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t** Grid;

uint8_t grid[9][9] = {
{8,0,0, 0,0,0, 0,0,0},
{0,0,3, 6,0,0, 0,0,0},
{0,7,0, 0,9,0, 2,0,0},

{0,5,0, 0,0,7, 0,0,0},
{0,0,0, 0,4,5, 7,0,0},
{0,0,0, 1,0,0, 0,3,0},

{0,0,1, 0,0,0, 0,6,8},
{0,0,8, 5,0,0, 0,1,0},
{0,9,0, 0,0,0, 4,0,0}
};

uint8_t grid2[9][9] = {
{5,9,0, 0,0,0, 7,0,0},
{0,0,0, 0,9,0, 0,0,1},
{0,0,4, 6,0,0, 0,2,0},

{2,0,5, 0,0,7, 0,0,0},
{0,0,0, 9,2,3, 0,0,0},
{0,0,0, 5,0,0, 2,0,8},

{0,3,0, 0,0,1, 5,0,0},
{4,0,0, 0,7,0, 0,0,0},
{0,0,6, 0,0,0, 0,4,9}
};


int check_col(Grid grid, int x) {
    uint8_t counters[10] = {0};
    for (int i = 0; i < 9; i++) {
        uint8_t v = grid[i][x];
        if (v != 0 && counters[v] > 0)
            return 0;
        counters[v]++;
    }
    return 1;
}


int check_row(Grid grid, int y) {
    uint8_t counters[10] = {0};
    for (int i = 0; i < 9; i++) {
        uint8_t v = grid[y][i];
        if (v != 0 && counters[v] > 0)
            return 0;
        counters[v]++;
    }
    return 1;
}


int check_sqr(Grid grid, int x, int y) {
    uint8_t counters[10] = {0};
    y = (y % 3);
    x = (x % 3);
    for (int i = y*3; i < (y*3)+3; i++) {
        for (int j = x*3; j < (x*3)+3; j++) {
            uint8_t v = grid[i][j];
            if (v != 0 && counters[v] > 0)
                return 0;
            counters[v]++;
        }
    }
    return 1;
}

int guess(Grid grid, int x, int y) {
    int next_x = (x+1);
    int next_y = y;
    if (next_x > 8) {
        next_x = 0; next_y++;
    }
    if (grid[y][x] != 0) {
        if (next_y > 8)
            return 1;
        return guess(grid, next_x, next_y);
    } else {
        for(int v = 1; v < 10; v++) {
            grid[y][x] = v;
            int r = check_row(grid, y);
            int c = check_col(grid, x);
            int s = check_sqr(grid, x, y);
            if (c && r && s) {
                if (next_y > 8)
                    return 1;
                int r = guess(grid, next_x, next_y);
                if (r) {
                    return 1;
                }
            }
        }
        grid[y][x] = 0;
    }
    return 0;
}


void solve(Grid grid) {
    int r = guess(grid, 0, 0);
    if (!r) {
        printf("Could not solve it!\n");
    }
}


char* digits = "0123456789";
void display_orig(Grid grid, Grid orig) {
    puts("┌───────┬───────┬───────┐");
    for(int i = 0; i < 9; i++) {
        printf("│ ");
        for(int j = 0; j < 9; j++) {
            uint8_t v = grid[i][j];
            if (orig && orig[i][j])
                printf("\e[1m%c\e[0m ", digits[v]);
            else
                printf("%c ", v ? digits[v] : '.');
            if ((j+1) % 3 == 0) printf("│ ");
        }
        putchar('\n');
        if ( i != 8 && (i+1) % 3 == 0)
        puts("├───────┼───────┼───────┤");
    }
    puts("└───────┴───────┴───────┘");
}


void display(Grid grid) {
    display_orig(grid, 0);
}


Grid copy_grid(Grid g) {
    Grid grid = (Grid)malloc(9*sizeof(uint8_t*));
    for(int i = 0; i < 9; i++) {
        grid[i] = (uint8_t*)malloc(9*sizeof(uint8_t));
    }
    for(int i = 0; i < 9; i++) {
        memcpy(grid[i], g[i], 9*sizeof(uint8_t));
    }
    return grid;
}


Grid array_to_grid(uint8_t g[9][9]) {
    Grid grid = (Grid)malloc(9*sizeof(uint8_t*));
    for(int i = 0; i < 9; i++) {
        grid[i] = (uint8_t*)malloc(9*sizeof(uint8_t));
    }
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            grid[i][j] = g[i][j];
        }
    }
    return grid;
}


int main(int argc, char** argv) {
    Grid g = array_to_grid(grid);
    Grid orig = copy_grid(g);
    display(g);
    solve(g);
    display_orig(g, orig);
    return 0;
}
