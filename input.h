
#ifndef __INPUT_H
#define __INPUT_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#define MOVEMENT_LEFT  1
#define MOVEMENT_RIGHT 2
#define MOVEMENT_UP    3
#define MOVEMENT_DOWN  4

#ifndef USE_SDL
#define KEY_PL1_LEFT  0xcb /* (keypad) left */
#define KEY_PL1_RIGHT	0xcd /* (keypad) right */
#define KEY_PL1_JUMP  0xc8 /* (keypad) up */
#define KEY_PL1_ACK   0xd0 /* (keypad) down */
#define KEY_PL2_LEFT  0x1e /* a */
#define KEY_PL2_RIGHT	0x20 /* d */
#define KEY_PL2_JUMP  0x11 /* w */
#define KEY_PL2_ACK   0x1f /* s */
#else
#define KEY_PL1_LEFT  SDLK_LEFT
#define KEY_PL1_RIGHT	SDLK_RIGHT
#define KEY_PL1_JUMP  SDLK_UP
#define KEY_PL1_ACK   SDLK_DOWN
#define KEY_PL2_LEFT  SDLK_a
#define KEY_PL2_RIGHT	SDLK_d
#define KEY_PL2_JUMP  SDLK_w
#define KEY_PL2_ACK   SDLK_s
#define KEY_PL3_LEFT  SDLK_j
#define KEY_PL3_RIGHT	SDLK_l
#define KEY_PL3_JUMP  SDLK_i
#define KEY_PL3_ACK   SDLK_k
#define KEY_PL4_LEFT  SDLK_KP4
#define KEY_PL4_RIGHT	SDLK_KP6
#define KEY_PL4_JUMP  SDLK_KP8
#define KEY_PL4_ACK   SDLK_KP2
#endif

extern char last_keys[50];

struct joy_t
{
	int x, y;
	int raw_x, raw_y;
	int but1, but2;
	struct {
		int x1, x2, x3;
		int y1, y2, y3;
	} calib_data;
};

struct mouse_t
{
	int but1, but2, but3;
};

extern struct joy_t joy;
extern struct mouse_t mouse;

int hook_keyb_handler();
void remove_keyb_handler();
int key_pressed(const int);     // key
int addkey(const unsigned int); // key

void update_player_actions();
void init_inputs();
int calib_joy(int);             // type

//#ifdef __cplusplus
//}
//#endif

#endif
