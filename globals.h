/*
 * globals.h
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * This file is part of Jump'n'Bump.
 *
 * Jump'n'Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump'n'Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef __GLOBALS_H
#define __GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "config.h"
#include "gfx.h"

#include <stdlib.h>
#include <stdio.h>
//#include <cstdlib>
//#include <cstdio>
#include <assert.h>
#include <string.h>
#ifndef _MSC_VER
#include <strings.h>
#endif
#include <time.h>
#include <math.h>
#include "dj.h"

#ifdef DOS
# include <conio.h>
# include <dpmi.h>
# include <sys/nearptr.h>
# include <pc.h>
#endif

#ifdef _MSC_VER
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <sys/stat.h>
# include <io.h>
# include "SDL.h"
# if USE_SDL_MIXER
#  include "SDL_mixer.h"
# endif
#else
# ifdef USE_SDL
#  include <sys/stat.h>
#  include "SDL.h"
#  if USE_SDL_MIXER
#   include "SDL_mixer.h"
#  endif
# endif
#endif

#define JNB_MAX_PLAYERS 4

#define JNB_INETPORT 11111

#define MOVEMENT_LEFT  1
#define MOVEMENT_RIGHT 2
#define MOVEMENT_UP    3

#define JNB_VERSION "1.51"

#define JNB_WIDTH  400
#define JNB_HEIGHT 256

int screen_width;
int screen_height;
int screen_pitch;
int scale_up;

int ai[JNB_MAX_PLAYERS];

#ifndef USE_SDL
#define KEY_PL1_LEFT  0xcb
#define KEY_PL1_RIGHT	0xcd
#define KEY_PL1_JUMP  0xc8
#define KEY_PL2_LEFT  0x1e
#define KEY_PL2_RIGHT	0x20
#define KEY_PL2_JUMP  0x11
#else
#define KEY_PL1_LEFT  SDLK_LEFT
#define KEY_PL1_RIGHT	SDLK_RIGHT
#define KEY_PL1_JUMP  SDLK_UP
#define KEY_PL2_LEFT  SDLK_a
#define KEY_PL2_RIGHT	SDLK_d
#define KEY_PL2_JUMP  SDLK_w
#define KEY_PL3_LEFT  SDLK_j
#define KEY_PL3_RIGHT	SDLK_l
#define KEY_PL3_JUMP  SDLK_i
#define KEY_PL4_LEFT  SDLK_KP4
#define KEY_PL4_RIGHT	SDLK_KP6
#define KEY_PL4_JUMP  SDLK_KP8
#endif

#define NUM_POBS      200
#define NUM_OBJECTS   200
#define NUM_FLIES     20
#define NUM_LEFTOVERS 50

#define OBJ_SPRING      0
#define OBJ_SPLASH      1
#define OBJ_SMOKE       2
#define OBJ_YEL_BUTFLY  3
#define OBJ_PINK_BUTFLY 4
#define OBJ_FUR         5
#define OBJ_FLESH       6
#define OBJ_FLESH_TRACE 7

#define OBJ_ANIM_SPRING            0
#define OBJ_ANIM_SPLASH            1
#define OBJ_ANIM_SMOKE             2
#define OBJ_ANIM_YEL_BUTFLY_RIGHT  3
#define OBJ_ANIM_YEL_BUTFLY_LEFT   4
#define OBJ_ANIM_PINK_BUTFLY_RIGHT 5
#define OBJ_ANIM_PINK_BUTFLY_LEFT  6
#define OBJ_ANIM_FLESH_TRACE       7

#define MOD_MENU   0
#define MOD_GAME   1
#define MOD_SCORES 2

#define SFX_JUMP   0
#define SFX_LAND   1
#define SFX_DEATH  2
#define SFX_SPRING 3
#define SFX_SPLASH 4
#define SFX_FLY    5

#define NUM_SFX 6

#define SFX_JUMP_FREQ   15000
#define SFX_LAND_FREQ   15000
#define SFX_DEATH_FREQ  20000
#define SFX_SPRING_FREQ 15000
#define SFX_SPLASH_FREQ 12000
#define SFX_FLY_FREQ    12000

#define BAN_VOID	 0
#define BAN_SOLID	 1
#define BAN_WATER	 2
#define BAN_ICE		 3
#define BAN_SPRING 4

#ifndef DATA_PATH
#ifdef __APPLE__
#define	DATA_PATH "data/jumpbump.dat"
#elif _WIN32
#define	DATA_PATH "data\\jumpbump.dat"
#else
#define	DATA_PATH "%%PREFIX%%/share/jumpnbump/jumpbump.dat"
#endif
#endif

struct main_info_t
{
	int joy_enabled, mouse_enabled;
	int no_sound, music_no_sound, no_gore, fireworks;
	char error_str[256];
	int draw_page, view_page;
	struct {
		int num_pobs;
		struct {
			int x, y;
			int image;
			struct gob_t *pob_data;
			int back_buf_ofs;
		} pobs[NUM_POBS];
	} page_info[2];
	void* pob_backbuf[2];
};

struct player_t
{
  int action_left,action_up,action_right;
	int enabled, dead_flag;
	int bumps;
	int bumped[JNB_MAX_PLAYERS];
	int x, y;
	int x_add, y_add;
	int direction, jump_ready, jump_abort, in_water;
	int anim, frame, frame_tick, image;
};

struct player_anim_t
{
	int num_frames;
	int restart_frame;
	struct {
		int image;
		int ticks;
	} frame[4];
};

struct object_t
{
	int used, type;
	int x, y;
	int x_add, y_add;
	int x_acc, y_acc;
	int anim;
	int frame, ticks;
	int image;
};

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

struct main_info_t main_info;
struct player_t player[JNB_MAX_PLAYERS];
struct player_anim_t player_anims[7];
struct object_t objects[NUM_OBJECTS];
struct joy_t joy;
struct mouse_t mouse;

char datfile_name[2048];

char* background_pic;
char* mask_pic;

struct gob_t rabbit_gobs;
struct gob_t font_gobs;
struct gob_t object_gobs;
struct gob_t number_gobs;

///* fireworks.c */
//
//void fireworks(void);
//

/* main.c */
void steer_players();
void position_player(int); // player num
void fireworks();
void add_object(int,  // type
                int,  // x
                int,  // y
                int,  // x_add
                int,  // y_add
                int,  // anim
                int); // frame
void update_objects();
int add_pob(int,            // page
            int,            // x
            int,            // y
            int,            // image
            struct gob_t*); // pob_data
void draw_flies(int); // page
void draw_pobs(int); // page
void redraw_flies_background(int); // page
void redraw_pob_backgrounds(int); // page
int add_leftovers(int,            // page
                  int,            // x
                  int,            // y
                  int,            // image
                  struct gob_t*); // pob_data
void draw_leftovers(int); // page
int init_level(int,    // level
               char*); // pal
void deinit_level();
int init_program(int,    // argc
                 char**, // argv
                 char*); // pal
void deinit_program();
unsigned short rnd(unsigned short); // max
int read_level();
void write_calib_data();

#ifdef __cplusplus
}
#endif

#endif
