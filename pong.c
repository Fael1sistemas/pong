// Using libs SDL, glibc
#include <SDL.h>    // SDL version 2.0
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 1280   // window width
#define SCREEN_HEIGHT 720   // window height

// function prototypes
int init(int w, int h, int argc, char *args[]);

typedef struct ball_s {
    int x, y;    /* position on the screen */
    int w, h;    // ball width and height
    int dx, dy;  /* movement vector */
} ball_t;

typedef struct paddle {
    int x, y;
    int w, h;
} paddle_t;

// Program globals
static ball_t ball;
static paddle_t paddle[2];
int score[] = {0, 0};
int width, height;      // used if fullscreen

// Game modes:
// game_mode: 0 = 1 Player (vs CPU), 1 = 2 Players (vs Player)
int game_mode = 0;
int selected_mode = 0;

SDL_Window* window = NULL;    // The window we'll be rendering to
SDL_Renderer *renderer;      // The renderer SDL will use to draw to the screen

// surfaces
static SDL_Surface *screen;
static SDL_Surface *title;
static SDL_Surface *numbermap;
static SDL_Surface *end;

// textures
SDL_Texture *screen_texture;

// Simple 8x8 font bitmap data for rendering menu text
static const unsigned char font8x8_basic[128][8] = {
    [' '] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    ['-'] = {0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00},
    ['.'] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00},
    [':'] = {0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00},
    ['<'] = {0x0E, 0x1C, 0x38, 0x70, 0x38, 0x1C, 0x0E, 0x00},
    ['>'] = {0x70, 0x38, 0x1C, 0x0E, 0x1C, 0x38, 0x70, 0x00},
    ['('] = {0x0E, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x0E, 0x00},
    [')'] = {0x70, 0x38, 0x38, 0x38, 0x38, 0x38, 0x70, 0x00},
    ['/'] = {0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, 0x00},
    ['0'] = {0x3C, 0x66, 0x6E, 0x76, 0x66, 0x66, 0x3C, 0x00},
    ['1'] = {0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00},
    ['2'] = {0x3C, 0x66, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00},
    ['3'] = {0x3C, 0x66, 0x06, 0x1C, 0x06, 0x66, 0x3C, 0x00},
    ['4'] = {0x0C, 0x1C, 0x3C, 0x6C, 0x7E, 0x0C, 0x0C, 0x00},
    ['5'] = {0x7E, 0x60, 0x7C, 0x06, 0x06, 0x66, 0x3C, 0x00},
    ['6'] = {0x3C, 0x66, 0x60, 0x7C, 0x66, 0x66, 0x3C, 0x00},
    ['7'] = {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x00},
    ['8'] = {0x3C, 0x66, 0x66, 0x3C, 0x66, 0x66, 0x3C, 0x00},
    ['9'] = {0x3C, 0x66, 0x66, 0x3E, 0x06, 0x66, 0x3C, 0x00},
    ['A'] = {0x3C, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00},
    ['B'] = {0x7C, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x7C, 0x00},
    ['C'] = {0x3C, 0x66, 0x60, 0x60, 0x60, 0x66, 0x3C, 0x00},
    ['D'] = {0x78, 0x6C, 0x66, 0x66, 0x66, 0x6C, 0x78, 0x00},
    ['E'] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x7E, 0x00},
    ['F'] = {0x7E, 0x60, 0x60, 0x7C, 0x60, 0x60, 0x60, 0x00},
    ['G'] = {0x3C, 0x66, 0x60, 0x6E, 0x66, 0x66, 0x3C, 0x00},
    ['H'] = {0x66, 0x66, 0x66, 0x7E, 0x66, 0x66, 0x66, 0x00},
    ['I'] = {0x3E, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x3E, 0x00},
    ['J'] = {0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x6C, 0x38, 0x00},
    ['K'] = {0x66, 0x6C, 0x78, 0x70, 0x78, 0x6C, 0x66, 0x00},
    ['L'] = {0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x7E, 0x00},
    ['M'] = {0x63, 0x77, 0x7F, 0x6B, 0x63, 0x63, 0x63, 0x00},
    ['N'] = {0x66, 0x76, 0x7E, 0x7E, 0x6E, 0x66, 0x66, 0x00},
    ['O'] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},
    ['P'] = {0x7C, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x00},
    ['Q'] = {0x3C, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x0E, 0x00},
    ['R'] = {0x7C, 0x66, 0x66, 0x7C, 0x70, 0x68, 0x66, 0x00},
    ['S'] = {0x3C, 0x66, 0x60, 0x3C, 0x06, 0x66, 0x3C, 0x00},
    ['T'] = {0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00},
    ['U'] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x00},
    ['V'] = {0x66, 0x66, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x00},
    ['W'] = {0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00},
    ['X'] = {0x66, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0x66, 0x00},
    ['Y'] = {0x66, 0x66, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x00},
    ['Z'] = {0x7E, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x7E, 0x00},
};

static void draw_char(char c, int x, int y, int scale, Uint32 color) {
    if ((unsigned char)c >= 128) return;
    const unsigned char *glyph = font8x8_basic[(unsigned char)c];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << (7 - col))) {
                SDL_Rect pixel_rect = { x + col * scale, y + row * scale, scale, scale };
                SDL_FillRect(screen, &pixel_rect, color);
            }
        }
    }
}

static void draw_string(const char *str, int x, int y, int scale, Uint32 color) {
    int cur_x = x;
    while (*str) {
        draw_char(*str, cur_x, y, scale, color);
        cur_x += 8 * scale;
        str++;
    }
}

static void draw_string_centered(const char *str, int y, int scale, Uint32 color) {
    int len = strlen(str);
    int total_width = len * 8 * scale;
    int x = (screen->w - total_width) / 2;
    draw_string(str, x, y, scale, color);
}

// initialise starting position and sizes of game elements
static void init_game() {
    ball.w = 14;
    ball.h = 14;
    ball.x = screen->w / 2 - ball.w / 2;
    ball.y = screen->h / 2 - ball.h / 2;
    ball.dy = (rand() % 2 == 0) ? 4 : -4;
    ball.dx = (rand() % 2 == 0) ? 6 : -6;

    paddle[0].w = 14;
    paddle[0].h = 80;
    paddle[0].x = 30;
    paddle[0].y = screen->h / 2 - paddle[0].h / 2;

    paddle[1].w = 14;
    paddle[1].h = 80;
    paddle[1].x = screen->w - 30 - paddle[1].w;
    paddle[1].y = screen->h / 2 - paddle[1].h / 2;
}

int check_score() {
    int i;
    for(i = 0; i < 2; i++) {
        if (score[i] >= 10) {
            int winner = i + 1;
            score[0] = 0;
            score[1] = 0;
            return winner;
        }
    }
    return 0;
}

// if return value is 1 collision occurred. if return is 0, no collision.
int check_collision(ball_t a, paddle_t b) {
    int left_a, left_b;
    int right_a, right_b;
    int top_a, top_b;
    int bottom_a, bottom_b;

    left_a = a.x;
    right_a = a.x + a.w;
    top_a = a.y;
    bottom_a = a.y + a.h;

    left_b = b.x;
    right_b = b.x + b.w;
    top_b = b.y;
    bottom_b = b.y + b.h;

    if (left_a > right_b) return 0;
    if (right_a < left_b) return 0;
    if (top_a > bottom_b) return 0;
    if (bottom_a < top_b) return 0;

    return 1;
}

/* Move ball and handle screen & paddle collisions */
static void move_ball() {
    ball.x += ball.dx;
    ball.y += ball.dy;

    if (ball.x < 0) {
        score[1] += 1; // Right player scores
        init_game();
    }

    if (ball.x > screen->w - ball.w) {
        score[0] += 1; // Left player scores
        init_game();
    }

    if (ball.y < 0) {
        ball.y = 0;
        ball.dy = -ball.dy;
    }

    if (ball.y > screen->h - ball.h) {
        ball.y = screen->h - ball.h;
        ball.dy = -ball.dy;
    }

    int i;
    for (i = 0; i < 2; i++) {
        int c = check_collision(ball, paddle[i]);

        if (c == 1) {
            if (ball.dx < 0) {
                ball.dx -= 1;
            } else {
                ball.dx += 1;
            }

            ball.dx = -ball.dx;

            float relative_hit = (float)((ball.y + ball.h / 2) - (paddle[i].y + paddle[i].h / 2)) / (paddle[i].h / 2.0f);
            if (relative_hit < -1.0f) relative_hit = -1.0f;
            if (relative_hit > 1.0f) relative_hit = 1.0f;

            ball.dy = (int)(relative_hit * 6.0f);

            if (ball.dx > 0) { // Ball hit left paddle, moving right
                if (ball.x < paddle[0].x + paddle[0].w) {
                    ball.x = paddle[0].x + paddle[0].w;
                }
            } else { // Ball hit right paddle, moving left
                if (ball.x > paddle[1].x - ball.w) {
                    ball.x = paddle[1].x - ball.w;
                }
            }
        }
    }
}

/* AI controls Right paddle (paddle[1]) in 1 Player mode */
static void move_paddle_ai() {
    int center = paddle[1].y + (paddle[1].h / 2);
    int screen_center = screen->h / 2;
    int ball_speed = ball.dy;

    if (ball_speed < 0) {
        ball_speed = -ball_speed;
    }
    if (ball_speed < 5) {
        ball_speed = 5;
    }

    if (ball.dx > 0) { // Ball moving right towards CPU
        if (ball.y > center + 6) {
            paddle[1].y += ball_speed;
        } else if (ball.y < center - 6) {
            paddle[1].y -= ball_speed;
        }
    } else { // Return to center when ball moves away
        if (center < screen_center - 10) {
            paddle[1].y += 4;
        } else if (center > screen_center + 10) {
            paddle[1].y -= 4;
        }
    }

    if (paddle[1].y < 0) paddle[1].y = 0;
    if (paddle[1].y > screen->h - paddle[1].h) paddle[1].y = screen->h - paddle[1].h;
}

static void move_paddle_0(int d) { // Left paddle (Player 1)
    if (d == 0) { // Down
        if (paddle[0].y >= screen->h - paddle[0].h) {
            paddle[0].y = screen->h - paddle[0].h;
        } else {
            paddle[0].y += 8;
        }
    }
    if (d == 1) { // Up
        if (paddle[0].y <= 0) {
            paddle[0].y = 0;
        } else {
            paddle[0].y -= 8;
        }
    }
}

static void move_paddle_1(int d) { // Right paddle (Player 2)
    if (d == 0) { // Down
        if (paddle[1].y >= screen->h - paddle[1].h) {
            paddle[1].y = screen->h - paddle[1].h;
        } else {
            paddle[1].y += 8;
        }
    }
    if (d == 1) { // Up
        if (paddle[1].y <= 0) {
            paddle[1].y = 0;
        } else {
            paddle[1].y -= 8;
        }
    }
}

static void draw_game_over(int p) {
    SDL_Rect p1, p2, cpu, dest;

    p1.x = 0; p1.y = 0; p1.w = end->w; p1.h = 75;
    p2.x = 0; p2.y = 75; p2.w = end->w; p2.h = 75;
    cpu.x = 0; cpu.y = 150; cpu.w = end->w; cpu.h = 75;

    dest.x = (screen->w / 2) - (end->w / 2);
    dest.y = (screen->h / 2) - (75 / 2);
    dest.w = end->w;
    dest.h = 75;

    switch (p) {
        case 1:
            SDL_BlitSurface(end, &p1, screen, &dest);
            break;
        case 2:
            SDL_BlitSurface(end, &p2, screen, &dest);
            break;
        default:
            SDL_BlitSurface(end, &cpu, screen, &dest);
    }

    // Helper message below game over
    Uint32 col_white = SDL_MapRGB(screen->format, 255, 255, 255);
    draw_string_centered("PRESS SPACE OR ENTER TO MAIN MENU", dest.y + dest.h + 30, 1, col_white);
}

static void draw_menu() {
    SDL_Rect src, dest;

    src.x = 0; src.y = 0; src.w = title->w; src.h = title->h;
    dest.x = (screen->w / 2) - (src.w / 2);
    dest.y = (screen->h * 0.15);
    dest.w = title->w;
    dest.h = title->h;

    SDL_BlitSurface(title, &src, screen, &dest);

    Uint32 col_selected_fg = SDL_MapRGB(screen->format, 0, 255, 100);
    Uint32 col_selected_bg = SDL_MapRGB(screen->format, 0, 50, 20);
    Uint32 col_normal_fg   = SDL_MapRGB(screen->format, 160, 160, 160);
    Uint32 col_normal_bg   = SDL_MapRGB(screen->format, 30, 30, 30);
    Uint32 col_white       = SDL_MapRGB(screen->format, 255, 255, 255);
    Uint32 col_grey        = SDL_MapRGB(screen->format, 140, 140, 140);

    int menu_base_y = (screen->h * 0.45);

    // Option 1 Box: 1 PLAYER (VS CPU)
    Uint32 fg1 = (selected_mode == 0) ? col_selected_fg : col_normal_fg;
    Uint32 bg1 = (selected_mode == 0) ? col_selected_bg : col_normal_bg;

    SDL_Rect box1 = { (screen->w - 380) / 2, menu_base_y, 380, 45 };
    SDL_FillRect(screen, &box1, bg1);

    SDL_Rect b1_top = { box1.x, box1.y, box1.w, 3 };
    SDL_Rect b1_bot = { box1.x, box1.y + box1.h - 3, box1.w, 3 };
    SDL_Rect b1_left = { box1.x, box1.y, 3, box1.h };
    SDL_Rect b1_right = { box1.x + box1.w - 3, box1.y, 3, box1.h };
    SDL_FillRect(screen, &b1_top, fg1);
    SDL_FillRect(screen, &b1_bot, fg1);
    SDL_FillRect(screen, &b1_left, fg1);
    SDL_FillRect(screen, &b1_right, fg1);

    const char *text1 = (selected_mode == 0) ? "> 1 PLAYER  (VS CPU) <" : "  1 PLAYER  (VS CPU)  ";
    draw_string(text1, box1.x + 14, box1.y + 14, 2, fg1);

    // Option 2 Box: 2 PLAYERS (PVP)
    Uint32 fg2 = (selected_mode == 1) ? col_selected_fg : col_normal_fg;
    Uint32 bg2 = (selected_mode == 1) ? col_selected_bg : col_normal_bg;

    SDL_Rect box2 = { (screen->w - 380) / 2, menu_base_y + 65, 380, 45 };
    SDL_FillRect(screen, &box2, bg2);

    SDL_Rect b2_top = { box2.x, box2.y, box2.w, 3 };
    SDL_Rect b2_bot = { box2.x, box2.y + box2.h - 3, box2.w, 3 };
    SDL_Rect b2_left = { box2.x, box2.y, 3, box2.h };
    SDL_Rect b2_right = { box2.x + box2.w - 3, box2.y, 3, box2.h };
    SDL_FillRect(screen, &b2_top, fg2);
    SDL_FillRect(screen, &b2_bot, fg2);
    SDL_FillRect(screen, &b2_left, fg2);
    SDL_FillRect(screen, &b2_right, fg2);

    const char *text2 = (selected_mode == 1) ? "> 2 PLAYERS (P1 VS P2) <" : "  2 PLAYERS (P1 VS P2)  ";
    draw_string(text2, box2.x + 6, box2.y + 14, 2, fg2);

    // Menu Controls Footer
    int footer_y = menu_base_y + 140;
    draw_string_centered("PRESS UP/DOWN OR 1/2 TO SELECT MODE", footer_y, 1, col_grey);
    draw_string_centered("PRESS SPACE OR ENTER TO START", footer_y + 25, 1, col_white);
    draw_string_centered("P1 CONTROLS: W / S KEYS", footer_y + 55, 1, col_grey);
    draw_string_centered("P2 CONTROLS: UP / DOWN ARROWS", footer_y + 75, 1, col_grey);
}

static void draw_net() {
    SDL_Rect net;
    net.x = screen->w / 2 - 2;
    net.y = 10;
    net.w = 4;
    net.h = 15;

    Uint32 col_white = SDL_MapRGB(screen->format, 255, 255, 255);
    while (net.y < screen->h - 10) {
        SDL_FillRect(screen, &net, col_white);
        net.y += 30;
    }
}

static void draw_ball() {
    SDL_Rect src;
    src.x = ball.x;
    src.y = ball.y;
    src.w = ball.w;
    src.h = ball.h;

    Uint32 col_white = SDL_MapRGB(screen->format, 255, 255, 255);
    int r = SDL_FillRect(screen, &src, col_white);
    if (r != 0) {
        printf("fill rectangle failed in func draw_ball()\n");
    }
}

static void draw_paddle() {
    SDL_Rect src;
    int i;
    Uint32 col_white = SDL_MapRGB(screen->format, 255, 255, 255);

    for (i = 0; i < 2; i++) {
        src.x = paddle[i].x;
        src.y = paddle[i].y;
        src.w = paddle[i].w;
        src.h = paddle[i].h;

        int r = SDL_FillRect(screen, &src, col_white);
        if (r != 0) {
            printf("fill rectangle failed in func draw_paddle()\n");
        }
    }
}

static void draw_player_0_score() {
    SDL_Rect src, dest;
    src.x = 0; src.y = 0; src.w = 64; src.h = 64;
    dest.x = (screen->w / 2) - src.w - 20;
    dest.y = 10;
    dest.w = 64; dest.h = 64;

    if (score[0] > 0 && score[0] < 10) {
        src.x += src.w * score[0];
    }
    SDL_BlitSurface(numbermap, &src, screen, &dest);
}

static void draw_player_1_score() {
    SDL_Rect src, dest;
    src.x = 0; src.y = 0; src.w = 64; src.h = 64;
    dest.x = (screen->w / 2) + 20;
    dest.y = 10;
    dest.w = 64; dest.h = 64;

    if (score[1] > 0 && score[1] < 10) {
        src.x += src.w * score[1];
    }
    SDL_BlitSurface(numbermap, &src, screen, &dest);
}

int main (int argc, char *args[]) {
    if (init(SCREEN_WIDTH, SCREEN_HEIGHT, argc, args) == 1) {
        return 0;
    }

    SDL_GetWindowSize(window, &width, &height);

    int sleep = 0;
    int quit = 0;
    int state = 0;
    int r = 0;
    Uint32 next_game_tick = SDL_GetTicks();

    int key_pressed_up = 0;
    int key_pressed_down = 0;

    init_game();

    while(quit == 0) {
        SDL_PumpEvents();

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_ESCAPE]) {
            quit = 1;
        }

        Uint32 col_black = SDL_MapRGB(screen->format, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_FillRect(screen, NULL, col_black);

        // State 0: Main Menu
        if (state == 0) {
            if (keystate[SDL_SCANCODE_1] || keystate[SDL_SCANCODE_KP_1]) {
                selected_mode = 0;
            }
            if (keystate[SDL_SCANCODE_2] || keystate[SDL_SCANCODE_KP_2]) {
                selected_mode = 1;
            }

            if (keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W]) {
                if (!key_pressed_up) {
                    selected_mode = 0;
                    key_pressed_up = 1;
                }
            } else {
                key_pressed_up = 0;
            }

            if (keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S]) {
                if (!key_pressed_down) {
                    selected_mode = 1;
                    key_pressed_down = 1;
                }
            } else {
                key_pressed_down = 0;
            }

            if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_RETURN]) {
                game_mode = selected_mode;
                score[0] = 0;
                score[1] = 0;
                init_game();
                state = 1;
                SDL_Delay(200);
            }

            draw_menu();

        // State 2: Game Over
        } else if (state == 2) {
            if (keystate[SDL_SCANCODE_SPACE] || keystate[SDL_SCANCODE_RETURN]) {
                state = 0;
                SDL_Delay(300);
            }

            if (r == 1) {
                draw_game_over(1); // Player 1 Wins
            } else if (r == 2) {
                if (game_mode == 0) {
                    draw_game_over(3); // Computer Wins
                } else {
                    draw_game_over(2); // Player 2 Wins
                }
            }

        // State 1: Playing
        } else if (state == 1) {
            // Player 1 Controls (Left paddle): W and S keys
            if (keystate[SDL_SCANCODE_W]) {
                move_paddle_0(1);
            }
            if (keystate[SDL_SCANCODE_S]) {
                move_paddle_0(0);
            }

            if (game_mode == 0) { // 1 Player Mode (Player vs CPU)
                // Also allow arrow keys for P1 in 1 Player mode
                if (keystate[SDL_SCANCODE_UP]) {
                    move_paddle_0(1);
                }
                if (keystate[SDL_SCANCODE_DOWN]) {
                    move_paddle_0(0);
                }

                // Move CPU paddle
                move_paddle_ai();
            } else { // 2 Players Mode (Player 1 vs Player 2)
                // Player 2 Controls (Right paddle): UP and DOWN arrow keys
                if (keystate[SDL_SCANCODE_UP]) {
                    move_paddle_1(1);
                }
                if (keystate[SDL_SCANCODE_DOWN]) {
                    move_paddle_1(0);
                }
            }

            r = check_score();

            if (r != 0) {
                state = 2; // Game Over
            }

            move_ball();
            draw_net();
            draw_paddle();
            draw_ball();
            draw_player_0_score();
            draw_player_1_score();
        }

        SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->w * sizeof (Uint32));
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        next_game_tick += 1000 / 60;
        sleep = next_game_tick - SDL_GetTicks();
        if (sleep >= 0 && sleep < 100) {
            SDL_Delay(sleep);
        } else {
            next_game_tick = SDL_GetTicks();
        }
    }

    SDL_FreeSurface(screen);
    SDL_FreeSurface(title);
    SDL_FreeSurface(numbermap);
    SDL_FreeSurface(end);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

int init(int width, int height, int argc, char *args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int i;
    int is_fullscreen = 0;
    for (i = 0; i < argc; i++) {
        if (strcmp(args[i], "-f") == 0) {
            is_fullscreen = 1;
            break;
        }
    }

    if (is_fullscreen) {
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP, &window, &renderer);
    } else {
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
    }

    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    if (screen == NULL) {
        printf("Could not create the screen surface! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    screen_texture = SDL_CreateTextureFromSurface(renderer, screen);
    if (screen_texture == NULL) {
        printf("Could not create screen_texture! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    title = SDL_LoadBMP("title.bmp");
    if (title == NULL) {
        printf("Could not Load title image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    numbermap = SDL_LoadBMP("numbermap.bmp");
    if (numbermap == NULL) {
        printf("Could not Load numbermap image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    end = SDL_LoadBMP("gameover.bmp");
    if (end == NULL) {
        printf("Could not Load gameover image! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    Uint32 colorkey = SDL_MapRGB(title->format, 255, 0, 255);
    SDL_SetColorKey(title, SDL_TRUE, colorkey);
    SDL_SetColorKey(numbermap, SDL_TRUE, colorkey);
    SDL_SetColorKey(end, SDL_TRUE, colorkey);

    return 0;
}
