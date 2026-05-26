#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Tests whether Android SDK is available.

set -e

print_docs_reference()
{
	echo >&2 "***"
	echo >&2 "*** Please see Documentation/android/quick-start.rst for details."
	echo >&2 "***"
}

trap 'if [ $? -ne 0 ]; then print_docs_reference; fi' EXIT

if [ -z "$ANDROID_HOME" ] && [ -z "$ANDROID_SDK_ROOT" ]; then
	ANDROID_DIR="$(dirname $0)/../.android-sdk"
	if [ ! -d "$ANDROID_DIR/cmdline-tools" ]; then
		echo >&2 "***"
		echo >&2 "*** Android SDK not found in ANDROID_HOME."
		echo >&2 "*** Downloading command line tools to kernel tree..."
		echo >&2 "***"
		
		mkdir -p "$ANDROID_DIR/cmdline-tools"
		wget -q "https://dl.google.com/android/repository/commandlinetools-linux-11076708_latest.zip" -O "$ANDROID_DIR/cmdline-tools.zip"
		unzip -q "$ANDROID_DIR/cmdline-tools.zip" -d "$ANDROID_DIR/cmdline-tools"
		rm "$ANDROID_DIR/cmdline-tools.zip"
		
		# Android cmdline-tools expects to be in cmdline-tools/latest
		mv "$ANDROID_DIR/cmdline-tools/cmdline-tools" "$ANDROID_DIR/cmdline-tools/latest"
		
		yes | "$ANDROID_DIR/cmdline-tools/latest/bin/sdkmanager" --licenses > /dev/null
		"$ANDROID_DIR/cmdline-tools/latest/bin/sdkmanager" "platforms;android-34" "build-tools;34.0.0" > /dev/null
	fi
	export ANDROID_HOME="$ANDROID_DIR"
fi

echo "Android SDK is available at ${ANDROID_HOME:-$ANDROID_SDK_ROOT}"
