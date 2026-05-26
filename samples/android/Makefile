# SPDX-License-Identifier: GPL-2.0

# We don't link android apps into vmlinux, but we hook them into the build
# so that they get built when the sample is enabled.

quiet_cmd_gradle = GRADLE  $@
      cmd_gradle = \
	$(GRADLE) -p $(src)/minimal assembleDebug && \
	cp $(src)/minimal/app/build/outputs/apk/debug/app-debug.apk $@

$(obj)/minimal.apk: $(src)/minimal FORCE
	$(call if_changed,gradle)

# If the config is built as a module or built-in, we just produce the APK
always-$(CONFIG_SAMPLE_ANDROID_MINIMAL) += minimal.apk

clean-files += minimal.apk
