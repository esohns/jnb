#ifndef __GFX_H
#define __GFX_H

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

void set_scaling(int scale);
void open_screen(void);
void wait_vrt(int mix);
void draw_begin(void);
void draw_end(void);
void flippage(int page);
void draw_begin(void);
void draw_end(void);
void clear_lines(int page, int y, int count, int color);
int get_color(int color, char pal[768]);
int get_pixel(int page, int x, int y);
void set_pixel(int page, int x, int y, int color);
void setpalette(int index, int count, char *palette);
void fillpalette(int red, int green, int blue);
#ifdef DOS
void get_block(char page, short x, short y, short width, short height, char *buffer);
void put_block(char page, short x, short y, short width, short height, char *buffer);
#else
void get_block(int page, int x, int y, int width, int height, void *buffer);
void put_block(int page, int x, int y, int width, int height, void *buffer);
#endif
void put_text(int page, int x, int y, char *text, int align);
void put_pob(int page, int x, int y, int image, struct gob_t *gob, int mask, void *mask_pic);
int pob_width(int image, struct gob_t *gob);
int pob_height(int image, struct gob_t *gob);
int pob_hs_x(int image, struct gob_t *gob);
int pob_hs_y(int image, struct gob_t *gob);
int read_pcx(unsigned char * handle, void *buffer, int buf_len, char *pal);
void register_background(char *pixels, char pal[768]);
int register_gob(unsigned char *handle, struct gob_t *gob, int len);
void recalculate_gob(struct gob_t *gob, char pal[768]);
void register_mask(void *pixels);

#ifdef USE_SDL
/* long filelength(int handle); */
void fs_toggle();
//int intr_sysupdate();
#endif

#endif
