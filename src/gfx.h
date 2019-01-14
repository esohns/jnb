
#ifndef __GFX_H
#define __GFX_H

/*#ifdef __cplusplus
extern "C" {
#endif*/

extern int screen_width;
extern int screen_height;
extern int screen_pitch;
extern int scale_up;

extern char* background_pic;
extern char* mask_pic;

#define BAN_VOID	 0
#define BAN_SOLID	 1
#define BAN_WATER	 2
#define BAN_ICE		 3
#define BAN_SPRING 4

extern unsigned int ban_map[17][22];
#define GET_BAN_MAP_XY(x, y) ban_map[(y) >> 4][(x) >> 4]

struct gob_t
{
	int    num_images;
	int*   width;
	int*   height;
	int*   hs_x;
	int*   hs_y;
	void** data;
	void** orig_data;
};

extern struct gob_t rabbit_gobs;
extern struct gob_t font_gobs;
extern struct gob_t object_gobs;
extern struct gob_t number_gobs;

extern int flip;
extern char pal[768];
extern char cur_pal[768];

void set_scaling(int scale);
void open_screen(void);
void wait_vrt(int mix);
void draw_begin(void);
void draw_end(void);
void flip_pixels(unsigned char* pixels);
void flippage(int page);
void draw_begin(void);
void draw_end(void);
void clear_lines(int page, int y, int count, int color);
int get_color(int color, char pal[768]);
int get_pixel(int page, int x, int y);
void set_pixel(int page, int x, int y, int color);
void setpalette(int index, int count, char* palette);
void fillpalette(int red, int green, int blue);

#ifdef DOS
void get_block(char page, short x, short y, short width, short height, char* buffer);
void put_block(char page, short x, short y, short width, short height, char* buffer);
#else
void get_block(int page, int x, int y, int width, int height, void* buffer);
void put_block(int page, int x, int y, int width, int height, void* buffer);
#endif /* platform */

void put_text(int page, int x, int y, char* text, int align);
void put_pob(int page, int x, int y, int image, struct gob_t* gob, int mask, void* mask_pic);
int pob_width(int image, struct gob_t* gob);
int pob_height(int image, struct gob_t* gob);
int pob_hs_x(int image, struct gob_t* gob);
int pob_hs_y(int image, struct gob_t* gob);
int read_pcx(unsigned char* handle, void* buffer, int buf_len, char* pal);
void register_background(char* pixels, char pal[768]);
int register_gob(unsigned char* handle, struct gob_t* gob, int len);
void recalculate_gob(struct gob_t* gob, char pal[768]);
void register_mask(void* pixels);

#ifdef USE_SDL
/* long filelength(int handle); */
void fs_toggle();
#endif /* USE_SDL */

/*#ifdef __cplusplus
}
#endif*/

#endif /* __GFX_H */

