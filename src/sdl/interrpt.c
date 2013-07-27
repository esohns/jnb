/*
 * interrpt.c
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

#include "globals.h"
#include "interrpt.h"
#include "input.h"
#include "gfx.h"

//#include <stdio.h>
//#include <time.h>
//
//#ifndef _MSC_VER
//#include <unistd.h>
//#endif

int
intr_sysupdate()
{
	SDL_Event e;
	int i = 0;
	static int last_time = 0;
	int now, time_diff;

	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		/* *TODO* map KEY_PLx_ACK function to mouse controller(s)... */
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if(e.button.state == SDL_PRESSED &&
					((key_pressed(KEY_PL3_LEFT) && e.button.button == SDL_BUTTON_RIGHT) ||
					(key_pressed(KEY_PL3_RIGHT) && e.button.button == SDL_BUTTON_LEFT) ||
					(e.button.button == SDL_BUTTON_LEFT && e.button.button == SDL_BUTTON_RIGHT) ||
          e.button.button == SDL_BUTTON_MIDDLE))
				{
				addkey(KEY_PL3_JUMP & 0x7f);
				}
			else if(e.button.state == SDL_RELEASED &&
					((key_pressed(KEY_PL3_LEFT) && e.button.button == SDL_BUTTON_RIGHT) ||
					(key_pressed(KEY_PL3_RIGHT) && e.button.button == SDL_BUTTON_LEFT) ||
          e.button.button == SDL_BUTTON_MIDDLE))
				{
				addkey((KEY_PL3_JUMP & 0x7f) | 0x8000);
				}

			if(e.button.button == SDL_BUTTON_LEFT)
				{
				SDLKey sym = KEY_PL3_LEFT;
				sym &= 0x7f;
				if(e.button.state == SDL_RELEASED)
					{
					if(key_pressed(KEY_PL3_JUMP) && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_RIGHT)))
						addkey(KEY_PL3_RIGHT & 0x7f);
					else
						sym |= 0x8000;
					}
				addkey(sym);
				}
			else if(e.button.button == SDL_BUTTON_RIGHT)
				{
				SDLKey sym = KEY_PL3_RIGHT;
				sym &= 0x7f;
				if (e.button.state == SDL_RELEASED)
					{
					if(key_pressed(KEY_PL3_JUMP) && (SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(SDL_BUTTON_LEFT)))
						addkey(KEY_PL3_LEFT & 0x7f);
					else
						sym |= 0x8000;
					}
				addkey(sym);
				}
			break;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			switch (e.key.keysym.sym) {
			case SDLK_F12:
				if (e.type == SDL_KEYDOWN) {
					SDL_Quit();
					exit(1);
				}
				break;
			case SDLK_F10:
				if (e.type == SDL_KEYDOWN) {
					fs_toggle();
				}
				break;
			case SDLK_1:
				if (e.type == SDL_KEYUP)
					ai[0] = !ai[0];

				/* Release keys, otherwise it will continue moving that way */
				addkey((KEY_PL1_LEFT  & 0x7f) | 0x8000);
				addkey((KEY_PL1_RIGHT & 0x7f) | 0x8000);
				addkey((KEY_PL1_JUMP  & 0x7f) | 0x8000);
				addkey((KEY_PL1_ACK   & 0x7f) | 0x8000);
				break;
			case SDLK_2:
				if (e.type == SDL_KEYUP)
					ai[1] = !ai[1];

				/* Release keys, otherwise it will continue moving that way */
				addkey((KEY_PL2_LEFT  & 0x7f) | 0x8000);
				addkey((KEY_PL2_RIGHT & 0x7f) | 0x8000);
				addkey((KEY_PL2_JUMP  & 0x7f) | 0x8000);
				addkey((KEY_PL2_ACK   & 0x7f) | 0x8000);
				break;
			case SDLK_3:
				if (e.type == SDL_KEYUP)
					ai[2] = !ai[2];

				/* Release keys, otherwise it will continue moving that way */
				addkey((KEY_PL3_LEFT  & 0x7f) | 0x8000);
				addkey((KEY_PL3_RIGHT & 0x7f) | 0x8000);
				addkey((KEY_PL3_JUMP  & 0x7f) | 0x8000);
				addkey((KEY_PL3_ACK   & 0x7f) | 0x8000);
				break;
			case SDLK_4:
				if (e.type == SDL_KEYUP)
					ai[3] = !ai[3];

				/* Release keys, otherwise it will continue moving that way */
				addkey((KEY_PL4_LEFT  & 0x7f) | 0x8000);
				addkey((KEY_PL4_RIGHT & 0x7f) | 0x8000);
				addkey((KEY_PL4_JUMP  & 0x7f) | 0x8000);
				addkey((KEY_PL4_ACK   & 0x7f) | 0x8000);
				break;
			case SDLK_ESCAPE:
				if (e.type == SDL_KEYUP)
					addkey(1 | 0x8000);
				else
					addkey(1 & 0x7f);
				break;
			default:
				e.key.keysym.sym &= 0x7f;
				if (e.type == SDL_KEYUP)
					e.key.keysym.sym |= 0x8000;
				addkey(e.key.keysym.sym);

				break;
			}
			break;
		case SDL_USEREVENT:
			switch (e.user.code) {
			case JNB_EVT_ACK_EXP:
				(*(struct player_t*)e.user.data1).ack_flag = 0;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		i++;
	}

	SDL_Delay(1);
	now = SDL_GetTicks();
	time_diff = now - last_time;
	if (time_diff>0) {
		i = time_diff / (1000 / 60);
		if (i) {
			last_time = now;
		} else {
			int tmp;

			tmp = (1000/60) - i - 10;
			if (tmp>0)
				SDL_Delay(tmp);
		}
	}
/*
	if (!then)
		SDL_Delay(1);
	else {
		then = (1000 / 60) - (now - then);
		if (then > 0 && then < 1000)
			SDL_Delay(then);
	}
	then = now;
*/

#ifdef USE_KAILLERA
	if (my_player >= 0) {
		update_kaillera_keys();
		i=1;
	}
#endif /* USE_KAILLERA */

	return i;
}
