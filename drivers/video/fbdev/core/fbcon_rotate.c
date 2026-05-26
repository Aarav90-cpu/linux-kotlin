/*
 *  linux/drivers/video/console/fbcon_rotate.c -- Software Rotation
 *
 *      Copyright (C) 2005 Antonino Daplas <adaplas @pol.net>
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive for
 *  more details.
 */

<<<<<<< HEAD
#include <linux/errno.h>
#include <linux/fb.h>
#include <linux/font.h>

=======
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fb.h>
#include <linux/vt_kern.h>
#include <linux/console.h>
#include <asm/types.h>
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
#include "fbcon.h"
#include "fbcon_rotate.h"

int fbcon_rotate_font(struct fb_info *info, struct vc_data *vc)
{
	struct fbcon_par *par = info->fbcon_par;
<<<<<<< HEAD
	unsigned char *buf;
	int ret;

	if (par->p->fontdata == par->rotated.fontdata && par->rotate == par->rotated.buf_rotate)
		return 0;

	par->rotated.fontdata = par->p->fontdata;
	par->rotated.buf_rotate = par->rotate;
=======
	int len, err = 0;
	int s_cellsize, d_cellsize, i;
	const u8 *src;
	u8 *dst;

	if (vc->vc_font.data == par->fontdata &&
	    par->p->con_rotate == par->cur_rotate)
		goto finished;

	src = par->fontdata = vc->vc_font.data;
	par->cur_rotate = par->p->con_rotate;
	len = vc->vc_font.charcount;
	s_cellsize = ((vc->vc_font.width + 7)/8) *
		vc->vc_font.height;
	d_cellsize = s_cellsize;

	if (par->rotate == FB_ROTATE_CW ||
	    par->rotate == FB_ROTATE_CCW)
		d_cellsize = ((vc->vc_font.height + 7)/8) *
			vc->vc_font.width;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

	if (info->fbops->fb_sync)
		info->fbops->fb_sync(info);

<<<<<<< HEAD
	buf = font_data_rotate(par->rotated.fontdata, vc->vc_font.width,
			       vc->vc_font.height, vc->vc_font.charcount,
			       par->rotated.buf_rotate, par->rotated.buf,
			       &par->rotated.bufsize);
	if (IS_ERR(buf)) {
		ret = PTR_ERR(buf);
		goto err_kfree;
	}

	par->rotated.buf = buf;

	return 0;

err_kfree:
	kfree(par->rotated.buf);
	par->rotated.buf = NULL; /* clear here to avoid output */
	par->rotated.bufsize = 0;

	return ret;
=======
	if (par->fd_size < d_cellsize * len) {
		kfree(par->fontbuffer);
		par->fontbuffer = NULL;
		par->fd_size = 0;

		dst = kmalloc_array(len, d_cellsize, GFP_KERNEL);

		if (dst == NULL) {
			err = -ENOMEM;
			goto finished;
		}

		par->fd_size = d_cellsize * len;
		par->fontbuffer = dst;
	}

	dst = par->fontbuffer;
	memset(dst, 0, par->fd_size);

	switch (par->rotate) {
	case FB_ROTATE_UD:
		for (i = len; i--; ) {
			rotate_ud(src, dst, vc->vc_font.width,
				  vc->vc_font.height);

			src += s_cellsize;
			dst += d_cellsize;
		}
		break;
	case FB_ROTATE_CW:
		for (i = len; i--; ) {
			rotate_cw(src, dst, vc->vc_font.width,
				  vc->vc_font.height);
			src += s_cellsize;
			dst += d_cellsize;
		}
		break;
	case FB_ROTATE_CCW:
		for (i = len; i--; ) {
			rotate_ccw(src, dst, vc->vc_font.width,
				   vc->vc_font.height);
			src += s_cellsize;
			dst += d_cellsize;
		}
		break;
	}

finished:
	return err;
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)
}
