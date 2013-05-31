
#ifndef __INPUT_H
#define __INPUT_H

static char last_keys[50];

int hook_keyb_handler();
void remove_keyb_handler();
int key_pressed(const int);     // key
int addkey(const unsigned int); // key

void update_player_actions();
void init_inputs();
int calib_joy(int);             // type

#endif
