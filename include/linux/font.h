/*
 *  font.h -- `Soft' font definitions
 *
 *  Created 1995 by Geert Uytterhoeven
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#ifndef _VIDEO_FONT_H
#define _VIDEO_FONT_H

<<<<<<< HEAD
#include <linux/math.h>
#include <linux/types.h>

struct console_font;

/*
 * Glyphs
 */

/**
 * font_glyph_pitch - Calculates the number of bytes per scanline
 * @width: The glyph width in bits per scanline
 *
 * A glyph's pitch is the number of bytes in a single scanline, rounded
 * up to the next full byte. The parameter @width receives the number
 * of visible bits per scanline. For example, if width is 14 bytes per
 * scanline, the pitch is 2 bytes per scanline. If width is 8 bits per
 * scanline, the pitch is 1 byte per scanline.
 *
 * Returns:
 * The number of bytes in a single scanline of the glyph
 */
static inline unsigned int font_glyph_pitch(unsigned int width)
{
	return DIV_ROUND_UP(width, 8);
}

/**
 * font_glyph_size - Calculates the number of bytes per glyph
 * @width: The glyph width in bits per scanline
 * @vpitch: The number of scanlines in the glyph
 *
 * The number of bytes in a glyph depends on the pitch and the number
 * of scanlines. font_glyph_size automatically calculates the pitch
 * from the given width. The parameter @vpitch gives the number of
 * scanlines, which is usually the glyph's height in scanlines. Fonts
 * coming from user space can sometimes have a different vertical pitch
 * with empty scanlines between two adjacent glyphs.
 */
static inline unsigned int font_glyph_size(unsigned int width, unsigned int vpitch)
{
	return font_glyph_pitch(width) * vpitch;
}

/*
 * font_data_t and helpers
 */

/**
 * font_data_t - Raw font data
 *
 * Values of type font_data_t store a pointer to raw font data. The format
 * is monochrome. Each bit sets a pixel of a stored glyph. Font data does
 * not store geometry information for the individual glyphs. Users of the
 * font have to store glyph size, pitch and character count separately.
 *
 * Font data in font_data_t is not equivalent to raw u8. Each pointer stores
 * an additional hidden header before the font data. The layout is
 *
 * +------+-----------------------------+
 * | -16  |  CRC32 Checksum (optional)  |
 * | -12  |  <Unused>                   |
 * |  -8  |  Number of data bytes       |
 * |  -4  |  Reference count            |
 * +------+-----------------------------+
 * |   0  |  Data buffer                |
 * |  ... |                             |
 * +------+-----------------------------+
 *
 * Use helpers to access font_data_t. Use font_data_buf() to get the stored data.
 */
typedef const unsigned char font_data_t;

/**
 * font_data_buf() - Returns the font data as raw bytes
 * @fd: The font data
 *
 * Returns:
 * The raw font data. The provided buffer is read-only.
 */
static inline const unsigned char *font_data_buf(font_data_t *fd)
{
	return (const unsigned char *)fd;
}

font_data_t *font_data_import(const struct console_font *font, unsigned int vpitch,
			      u32 (*calc_csum)(u32, const void *, size_t));
void font_data_get(font_data_t *fd);
bool font_data_put(font_data_t *fd);
unsigned int font_data_size(font_data_t *fd);
bool font_data_is_equal(font_data_t *lhs, font_data_t *rhs);
int font_data_export(font_data_t *fd, struct console_font *font, unsigned int vpitch);

/* font_rotate.c */
void font_glyph_rotate_90(const unsigned char *glyph, unsigned int width, unsigned int height,
			  unsigned char *out);
void font_glyph_rotate_180(const unsigned char *glyph, unsigned int width, unsigned int height,
			   unsigned char *out);
void font_glyph_rotate_270(const unsigned char *glyph, unsigned int width, unsigned int height,
			   unsigned char *out);
unsigned char *font_data_rotate(font_data_t *fd, unsigned int width, unsigned int height,
				unsigned int charcount, unsigned int steps,
				unsigned char *buf, size_t *bufsize);

/*
 * Font description
 */

=======
#include <linux/types.h>

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
struct font_desc {
    int idx;
    const char *name;
    unsigned int width, height;
    unsigned int charcount;
<<<<<<< HEAD
    font_data_t *data;
    int pref;
};

=======
    const void *data;
    int pref;
};

#define VGA8x8_IDX	0
#define VGA8x16_IDX	1
#define PEARL8x8_IDX	2
#define VGA6x11_IDX	3
#define FONT7x14_IDX	4
#define	FONT10x18_IDX	5
#define SUN8x16_IDX	6
#define SUN12x22_IDX	7
#define ACORN8x8_IDX	8
#define	MINI4x6_IDX	9
#define FONT6x10_IDX	10
#define TER16x32_IDX	11
#define FONT6x8_IDX	12
#define TER10x18_IDX	13

extern const struct font_desc	font_vga_8x8,
			font_vga_8x16,
			font_pearl_8x8,
			font_vga_6x11,
			font_7x14,
			font_10x18,
			font_sun_8x16,
			font_sun_12x22,
			font_acorn_8x8,
			font_mini_4x6,
			font_6x10,
			font_ter_16x32,
			font_6x8,
			font_ter_10x18;

>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
/* Find a font with a specific name */

extern const struct font_desc *find_font(const char *name);

/* Get the default font for a specific screen size */

extern const struct font_desc *get_default_font(int xres, int yres,
						unsigned long *font_w,
						unsigned long *font_h);

/* Max. length for the name of a predefined font */
#define MAX_FONT_NAME	32

<<<<<<< HEAD
/*
 * Built-in fonts
 */

extern const struct font_desc font_10x18;
extern const struct font_desc font_6x10;
extern const struct font_desc font_6x8;
extern const struct font_desc font_7x14;
extern const struct font_desc font_acorn_8x8;
extern const struct font_desc font_mini_4x6;
extern const struct font_desc font_pearl_8x8;
extern const struct font_desc font_sun_12x22;
extern const struct font_desc font_sun_8x16;
extern const struct font_desc font_ter_10x18;
extern const struct font_desc font_ter_16x32;
extern const struct font_desc font_vga_6x11;
extern const struct font_desc font_vga_8x16;
extern const struct font_desc font_vga_8x8;
=======
/* Extra word getters */
#define REFCOUNT(fd)	(((int *)(fd))[-1])
#define FNTSIZE(fd)	(((int *)(fd))[-2])
#define FNTCHARCNT(fd)	(((int *)(fd))[-3])
#define FNTSUM(fd)	(((int *)(fd))[-4])

#define FONT_EXTRA_WORDS 4

struct font_data {
	unsigned int extra[FONT_EXTRA_WORDS];
	const unsigned char data[];
} __packed;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

#endif /* _VIDEO_FONT_H */
