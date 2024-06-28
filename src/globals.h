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

#ifdef USE_SDL
#include <SDL.h>
#endif /* USE_SDL */

#ifdef __cplusplus
extern "C" {
#endif

#define JNB_MAX_PLAYERS 4

#define JNB_ACK_PERIOD 10000 /* ms */
#define JNB_EVT_ACK_EXP 0

#define JNB_INETPORT	11111

#define JNB_WIDTH  400
#define JNB_HEIGHT 256

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

#ifndef DATA_PATH
#ifdef __APPLE__
#define	DATA_PATH "data/jnb.dat"
#elif defined _WIN32 || defined _WIN64
#define	DATA_PATH "data\\jnb.dat"
#else
#define	DATA_PATH "%%PREFIX%%/share/jnb/jnb.dat"
#endif /* platform */
#endif /* DATA_PATH */

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
			struct gob_t* pob_data;
			int back_buf_ofs;
		} pobs[NUM_POBS];
	} page_info[2];
	void* pob_backbuf[2];
};

struct player_t
{
  int action_left, action_up, action_right, action_down;
	int enabled, ack_flag, dead_flag;
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

struct object_anim_t {
	int num_frames;
	int restart_frame;
	struct {
		int image;
		int ticks;
	} frame[10];
};

extern struct main_info_t main_info;
extern struct player_t player[JNB_MAX_PLAYERS];
extern struct player_anim_t player_anims[7];
extern struct object_t objects[NUM_OBJECTS];
extern struct object_anim_t object_anims[8];

extern char datfile_name[2048];

extern int ai[JNB_MAX_PLAYERS];

/* main.c */
void steer_players();
void position_player(int); /* player num */
void fireworks();
void add_object(int,  /* type */
                int,  /* x */
                int,  /* y */
                int,  /* x_add */
                int,  /* y_add */
                int,  /* anim */
                int); /* frame */
void update_objects();
int add_pob(int,            /* page */
            int,            /* x */
            int,            /* y */
            int,            /* image */
            struct gob_t*); /* pob_data */
void draw_flies(int); /* page */
void draw_pobs(int); /* page */
void redraw_flies_background(int); /* page */
void redraw_pob_backgrounds(int); /* page */
int add_leftovers(int,            /* page */
                  int,            /* x */
                  int,            /* y */
                  int,            /* image */
                  struct gob_t*); /* pob_data */
void draw_leftovers(int); /* page */
int init_level(int,    /* level */
               char*); /* pal */
void deinit_level();
int init_program(int,    /* argc */
                 char**, /* argv */
                 char*); /* pal */
void deinit_program();
unsigned short rnd(unsigned short); /* max */
int read_level();
void write_calib_data();
#ifdef USE_SDL
Uint32 expire_ack_cb(Uint32 interval, void* parameter);
#endif /* USE_SDL */

#ifdef __cplusplus
}
#endif

#endif /* __GLOBALS_H */
