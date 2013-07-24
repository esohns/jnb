/*
 * input.c
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

#ifdef USE_SDL
#include "SDL.h"
#endif /* USE_SDL */

#include "input.h"
#include "net.h"

int num_joys = 0;
SDL_Joystick* joys[4];

struct joy_t joy;
struct mouse_t mouse;

/* assumes joysticks have at least one button, could check numbuttons first? */
#define JOY_LEFT(num) (num_joys>num && SDL_JoystickGetAxis(joys[num], 0)<-3200)
#define JOY_RIGHT(num) (num_joys>num && SDL_JoystickGetAxis(joys[num], 0)>3200)
/* I find using the vertical axis to be annoying -- dnb */
#define JOY_JUMP(num) (num_joys>num && SDL_JoystickGetButton(joys[num], 0))

#ifdef USE_KAILLERA
#include "SDL_thread.h"
#include "SDL_mutex.h"
#include <kailleraclient.h>

char local_keyb[256];
#endif /* USE_KAILLERA */

char keyb[256];
char last_keys[50];

#ifdef USE_KAILLERA
/* information about the party in this session */
int my_player = -1;
int my_numplayers = -1;

/* semaphore for controlling kaillera thread */
SDL_sem *game_start_sem = NULL;

/* keys supported on my end */
int my_player_up = -1;
int my_player_left = -1;
int my_player_right = 1;

/* values for the kaillera client interface */
static char kaillera_app_name[] = "Jump 'n Bump";
static char kaillera_game_name[] = "Jump 'n Bump\0\0";

static int player_keys[4][3] = {
	{
		KEY_PL1_LEFT,
		KEY_PL1_RIGHT,
		KEY_PL1_JUMP
	},                        
	{
		KEY_PL2_LEFT,
		KEY_PL2_RIGHT,
		KEY_PL2_JUMP
	},
	{
		KEY_PL3_LEFT,
		KEY_PL3_RIGHT,
		KEY_PL3_JUMP
	},
	{
		KEY_PL4_LEFT,
		KEY_PL4_RIGHT,
		KEY_PL4_JUMP
	}
};

static int WINAPI
kaillera_game_callback(char *game, int player, int numplayers)
{
	int length;
	int urand;
	unsigned char random[8];

	if (strcmp(game, kaillera_game_name) != 0) {
		printf("unknown game selected: %s\n", game);

		my_player = -1;
		goto release;
	}

	printf("start network game with %d players\n", numplayers);
	printf("I am player %d\n", player);

	my_player = player;
	my_numplayers = numplayers;

	my_player_up = player_keys[player-1][0] & 0xff;
	my_player_left = player_keys[player-1][1] & 0xff;
	my_player_right = player_keys[player-1][2] & 0xff;

	/* initialize randomizer agreed by all players */
	random[0] = time(0) & 0xff;
	random[1] = random[2] = random[3] = 0x00;
	length = kailleraModifyPlayValues(&random, sizeof(random[0]));
	if (length < 0) {
		goto release;
	}

	urand = random[3] << 24 | random[2] << 16 | random[1] << 8 | random[0];
	srand(urand);

release:

	SDL_SemPost(game_start_sem);
	return 0;
}

static kailleraInfos kaillera_data = {
	kaillera_app_name,
	kaillera_game_name,
	kaillera_game_callback,
	NULL,
	NULL,
	NULL
};

static void
print_version()
{
	char version[16];

	kailleraGetVersion(version);
	printf("using kaillera version %s\n", version);
}

static int
kaillera_thread(void *arg)
{
	kailleraInit();
	
	/* print_version(); */

	kailleraSetInfos(&kaillera_data);

	kailleraSelectServerDialog(0);
	if (SDL_SemValue(game_start_sem) == 0) {
		/* server dialog returned and game didnt start */
		
		/* release blocking thread */
		my_player = -1;
		SDL_SemPost(game_start_sem);
	}

	return 0;
}

static int
start_kaillera_thread(void)
{
	SDL_Thread *thread;

	game_start_sem = SDL_CreateSemaphore(0);

	thread = SDL_CreateThread(kaillera_thread, NULL);
	if (!thread) {
		printf("SDL_CreateThread failed\n");
		return -1;
	}
	
	return 0;
}	

int
addkey(unsigned int key)
{
	/* it doesnt matter if a player presses keys
	 * that control other bunnies. whatever is sent 
	 * is packed by pack_keys()
	 */
	if (!(key & 0x8000)) {
		local_keyb[key & 0x7fff] = 1;
	} else
		local_keyb[key & 0x7fff] = 0;
	return 0;
}

void
remove_keyb_handler(void)
{
	kailleraShutdown();
}

int
pack_keys(void)
{
	int rv;

	rv = local_keyb[my_player_up];
	rv |= local_keyb[my_player_left] << 1;
	rv |= local_keyb[my_player_right] << 2;
	rv |= local_keyb[1] << 3;
	return rv;
}

void
unpack_keys(int player, char value)
{
	keyb[player_keys[player][0] & 0xff] = (value >> 0) & 1;
	keyb[player_keys[player][1] & 0xff] = (value >> 1) & 1;
	keyb[player_keys[player][2] & 0xff] = (value >> 2) & 1;

	/* escape key is shared among all users */
	keyb[1] |= (value >> 3) & 1;
}

int
update_kaillera_keys(void)
{
	char keys[8];
	int length;
	int player;

	keys[0] = pack_keys();
	length = kailleraModifyPlayValues(&keys, sizeof(keys[0]));

	if (length < 0) {
		/* terminate session */
		printf("** LOST CONNECTION **\n");
		kailleraEndGame();
		my_player = -1;
		return -1;
	}

	for (player=0; player<length; player++) {
		unpack_keys(player, keys[player]);
	}

	return 0;
}

int
hook_keyb_handler(void)
{
	SDL_EnableUNICODE(1);
	memset((void *) last_keys, 0, sizeof(last_keys));

	start_kaillera_thread();
	SDL_SemWait(game_start_sem);
	if (my_player < 0) {
		printf("GAME ABORTED!\n");
		return -1;
	}

	printf("GAME STARTS!\n");
	return 0;
}

int
key_pressed(int key)
{
	if (key == 1 && my_player < 0) {
		/* if game completed or aborted, post ESC */
		return 1;
	}

	return keyb[(unsigned char) key];
}
#else /* USE_KAILLERA */
int
addkey(const unsigned int key)
{
	int c1;

	if (!(key & 0x8000)) {
		keyb[key & 0x7fff] = 1;
		for (c1 = 48; c1 > 0; c1--)
			last_keys[c1] = last_keys[c1 - 1];
		last_keys[0] = key & 0x7fff;
	} else
		keyb[key & 0x7fff] = 0;
	return 0;
}

void
remove_keyb_handler(void)
{
}

int
hook_keyb_handler(void)
{
	SDL_EnableUNICODE(1);
	memset((void *) last_keys, 0, sizeof(last_keys));

	return 0;
}

int
key_pressed(const int key)
{
	return keyb[(unsigned char)key];
}
#endif /* USE_KAILLERA */

int
calib_joy(int type)
{
	return 1;
}

void 
update_player_actions(void)
{
	int tmp;

	if (client_player_num < 0) {
		tmp = (key_pressed(KEY_PL1_LEFT) == 1) || JOY_LEFT(3);
		if (tmp != player[0].action_left)
			tellServerPlayerMoved(0, MOVEMENT_LEFT, tmp);
		tmp = (key_pressed(KEY_PL1_RIGHT) == 1) || JOY_RIGHT(3);
		if (tmp != player[0].action_right)
			tellServerPlayerMoved(0, MOVEMENT_RIGHT, tmp);
		tmp = (key_pressed(KEY_PL1_JUMP) == 1) || JOY_JUMP(3);
		if (tmp != player[0].action_up)
			tellServerPlayerMoved(0, MOVEMENT_UP, tmp);

		tmp = (key_pressed(KEY_PL2_LEFT) == 1) || JOY_LEFT(2);
		if (tmp != player[1].action_left)
			tellServerPlayerMoved(1, MOVEMENT_LEFT, tmp);
		tmp = (key_pressed(KEY_PL2_RIGHT) == 1) || JOY_RIGHT(2);
		if (tmp != player[1].action_right)
			tellServerPlayerMoved(1, MOVEMENT_RIGHT, tmp);
		tmp = (key_pressed(KEY_PL2_JUMP) == 1) || JOY_JUMP(2);
		if (tmp != player[1].action_up)
			tellServerPlayerMoved(1, MOVEMENT_UP, tmp);

		tmp = (key_pressed(KEY_PL3_LEFT) == 1) || JOY_LEFT(1);
		if (tmp != player[2].action_left)
			tellServerPlayerMoved(2, MOVEMENT_LEFT, tmp);
		tmp = (key_pressed(KEY_PL3_RIGHT) == 1) || JOY_RIGHT(1);
		if (tmp != player[2].action_right)
			tellServerPlayerMoved(2, MOVEMENT_RIGHT, tmp);
		tmp = (key_pressed(KEY_PL3_JUMP) == 1) || JOY_JUMP(1);
		if (tmp != player[2].action_up)
			tellServerPlayerMoved(2, MOVEMENT_UP, tmp);

		tmp = (key_pressed(KEY_PL4_LEFT) == 1) || JOY_LEFT(0);
		if (tmp != player[3].action_left)
		tellServerPlayerMoved(3, MOVEMENT_LEFT, tmp);
		tmp = (key_pressed(KEY_PL4_RIGHT) == 1) || JOY_RIGHT(0);
		if (tmp != player[3].action_right)
		tellServerPlayerMoved(3, MOVEMENT_RIGHT, tmp);
		tmp = (key_pressed(KEY_PL4_JUMP) == 1) || JOY_JUMP(0);
		if (tmp != player[3].action_up)
		tellServerPlayerMoved(3, MOVEMENT_UP, tmp);
	} else {
		tmp = (key_pressed(KEY_PL1_LEFT) == 1) || JOY_LEFT(0);
		if (tmp != player[client_player_num].action_left)
			tellServerPlayerMoved(client_player_num, MOVEMENT_LEFT, tmp);
		tmp = (key_pressed(KEY_PL1_RIGHT) == 1) || JOY_RIGHT(0);
		if (tmp != player[client_player_num].action_right)
			tellServerPlayerMoved(client_player_num, MOVEMENT_RIGHT, tmp);
		tmp = (key_pressed(KEY_PL1_JUMP) == 1) || JOY_JUMP(0);
		if (tmp != player[client_player_num].action_up)
			tellServerPlayerMoved(client_player_num, MOVEMENT_UP, tmp);
	}
}

void
init_inputs(void)
{
	int i;

	num_joys = SDL_NumJoysticks();
	for(i = 0; i < 4 && i < num_joys; ++i)
		joys[i] = SDL_JoystickOpen(i);

	main_info.mouse_enabled = 0;
	main_info.joy_enabled = 0;
}
