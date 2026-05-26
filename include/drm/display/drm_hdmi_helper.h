/* SPDX-License-Identifier: MIT */

#ifndef DRM_HDMI_HELPER
#define DRM_HDMI_HELPER

#include <linux/hdmi.h>

struct drm_connector;
struct drm_connector_state;
struct drm_display_mode;
<<<<<<< HEAD
enum drm_output_color_format;
=======
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

void
drm_hdmi_avi_infoframe_colorimetry(struct hdmi_avi_infoframe *frame,
				   const struct drm_connector_state *conn_state);

void
drm_hdmi_avi_infoframe_bars(struct hdmi_avi_infoframe *frame,
			    const struct drm_connector_state *conn_state);

int
drm_hdmi_infoframe_set_hdr_metadata(struct hdmi_drm_infoframe *frame,
				    const struct drm_connector_state *conn_state);

void drm_hdmi_avi_infoframe_content_type(struct hdmi_avi_infoframe *frame,
					 const struct drm_connector_state *conn_state);

unsigned long long
drm_hdmi_compute_mode_clock(const struct drm_display_mode *mode,
<<<<<<< HEAD
			    unsigned int bpc, enum drm_output_color_format fmt);
=======
			    unsigned int bpc, enum hdmi_colorspace fmt);
>>>>>>> 34de6d11a83a (Added Spport for Kotlin and Java)

void
drm_hdmi_acr_get_n_cts(unsigned long long tmds_char_rate,
		       unsigned int sample_rate,
		       unsigned int *out_n,
		       unsigned int *out_cts);

#endif
