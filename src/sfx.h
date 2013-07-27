/*
 * sfx.h
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2002 Florian Schulze - crow@icculus.org
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

#ifndef __SFX_H
#define __SFX_H

//#ifdef __cplusplus
//extern "C" {
//#endif

#define DJ_SD_TYPE_NOSOUND 0
#define DJ_SD_TYPE_SBLASTER 1

#define DJ_SFX_TYPE_SSS 0
#define DJ_SFX_TYPE_WAV 1
#define DJ_SFX_TYPE_SMP 2

#define SFX_CHANNEL_FLIES 4
#define MAX_CHANNELS 32

#define SAMPLECOUNT 512

#define SFX_JUMP   0
#define SFX_ACK    1
#define SFX_DEATH  2
#define SFX_SPRING 3
#define SFX_SPLASH 4
#define SFX_FLY    5
#define NUM_SFX	6

#define SFX_JUMP_FREQ   15000
#define SFX_ACK_FREQ    25000
#define SFX_DEATH_FREQ  20000
#define SFX_SPRING_FREQ 15000
#define SFX_SPLASH_FREQ 12000
#define SFX_FLY_FREQ    12000

struct dj_hardware_info {
	char sd_type;
  short sd_version;
	short port;
	char irq;
	char dma;
};

struct dj_mixing_info {
  char sfx_volume, num_sfx_channels;
  char mod_volume, num_mod_channels;
	char stereo_mix, auto_mix;
  unsigned short mixing_freq;
  unsigned short dma_time, dmabuf_len;
  char cur_dmabuf;
	unsigned long dmabuf_address[2];
  char* mixed_buf;
};

struct sfx_data {
  char priority;
  unsigned short default_freq;
  char default_volume;
  unsigned long length;
  char loop;
  unsigned long loop_start, loop_length;
  unsigned char* buf;
};

struct dj_mod_info {
  char num_channels;
	char speed;
	short bpm;
	char order_pos;
	char pat_pos;
	char name[20];
	struct {
		char name[22];
		unsigned short length;
		char finetune;
		char volume;
		unsigned short loop_start;
		unsigned short loop_length;
		char* buf;
	} samples[31];
	char song_length;
	char num_pat;
	char pat_order[128];
  char* pat[128];
};

struct channel_info_t {
	/* loop flag */
	int loop;
	/* The channel step amount... */
	unsigned int step;
	/* ... and a 0.16 bit remainder of last step. */
	unsigned int stepremainder;
	unsigned int samplerate;
	/* The channel data pointers, start and end. */
	signed short* data;
	signed short* startdata;
	signed short* enddata;
	/* Hardware left and right channel volume lookup. */
	int leftvol;
	int rightvol;
};

/* Sample rate in samples/second */
extern int audio_rate;
extern int global_sfx_volume;

char dj_init(void);
void dj_deinit(void);
void dj_start(void);
void dj_stop(void);
void dj_set_nosound(char flag);
char dj_set_sd(char sd_type, short port, char irq, char dma);
char dj_autodetect_sd(void);
void dj_get_sd_string(char *strbuf);
char dj_set_stereo(char flag);
void dj_reverse_stereo(char flag);
void dj_set_auto_mix(char flag);
unsigned short dj_set_mixing_freq(unsigned short freq);
void dj_set_dma_time(unsigned short time);
char dj_get_hardware_info(struct dj_hardware_info *ptr);
char dj_get_mixing_info(struct dj_mixing_info* ptr);
char dj_get_mod_info(char mod_num, struct dj_mod_info* ptr);
void dj_set_fake_vu_speed(unsigned char speed);
unsigned char dj_get_fake_vu(char channel);
char dj_reset_sd(void);

char dj_mix_needed(void);
void dj_mix(void);

char dj_set_num_sfx_channels(char num_channels);
void dj_set_sfx_volume(char volume);
char dj_get_sfx_volume(void);
void dj_play_sfx(unsigned char sfx_num, unsigned short freq, char volume, char panning, unsigned short delay, char channel);
char dj_get_sfx_settings(unsigned char sfx_num, struct sfx_data* data);
char dj_set_sfx_settings(unsigned char sfx_num, struct sfx_data* data);
void dj_set_sfx_channel_volume(char channel_num, char volume);
void dj_stop_sfx_channel(char channel_num);
char dj_load_sfx(unsigned char* file_handle, char* filename, int file_length, char sfx_type, unsigned char sfx_num);
void dj_free_sfx(unsigned char sfx_num);

char dj_ready_mod(char mod_num);
char dj_start_mod(void);
void dj_stop_mod(void);
void dj_set_mod_volume(char volume);
char dj_get_mod_volume(void);
char dj_load_mod(unsigned char* file_handle, char* filename, char mod_num);
void dj_free_mod(char mod_num);

//#ifdef __cplusplus
//}
//#endif

#endif
