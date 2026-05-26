#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Tests whether a suitable Kotlin/Native toolchain is available.

set -e

# Print a reference to the documentation.
print_docs_reference()
{
	echo >&2 "***"
	echo >&2 "*** Please see Documentation/kotlin/quick-start.rst for details"
	echo >&2 "*** on how to set up the Kotlin/Native support."
	echo >&2 "***"
}

# Print an explanation about the fact that the script is meant to be called from Kbuild.
print_kbuild_explanation()
{
	echo >&2 "***"
	echo >&2 "*** This script is intended to be called from Kbuild."
	echo >&2 "*** Please use the 'kotlinavailable' target to call it instead."
	echo >&2 "*** Otherwise, the results may not be meaningful."
	exit 1
}

# If the script fails for any reason, print a reference to the documentation.
trap 'if [ $? -ne 0 ]; then print_docs_reference; fi' EXIT

# Check that the expected environment variables are set.
if [ -z "${KOTLINC+x}" ]; then
	echo >&2 "***"
	echo >&2 "*** Environment variable 'KOTLINC' is not set."
	print_kbuild_explanation
fi

# Check that we have a konanc (Kotlin/Native compiler) available
# If not, download it automatically to the kernel tree!
KOTLIN_DIR="$(dirname $0)/../.kotlin-native"
if ! command -v "konanc" >/dev/null 2>&1 && ! command -v "kotlinc-native" >/dev/null 2>&1; then
	if [ ! -f "$KOTLIN_DIR/bin/kotlinc-native" ]; then
		echo >&2 "***"
		echo >&2 "*** Kotlin/Native compiler not found in PATH."
		echo >&2 "*** Automatically downloading Kotlin/Native 2.3.21..."
		echo >&2 "***"
		
		# Download and extract
		mkdir -p "$KOTLIN_DIR"
		wget -qO- "https://github.com/JetBrains/kotlin/releases/download/v2.3.21/kotlin-native-prebuilt-linux-x86_64-2.3.21.tar.gz" | \
			tar xz --strip-components=1 -C "$KOTLIN_DIR"
			
		if [ $? -ne 0 ]; then
			echo >&2 "*** Failed to download Kotlin/Native."
			exit 1
		fi
	fi
	
	# Override environment variables to use the local copy
	export KOTLINC="$KOTLIN_DIR/bin/kotlinc-native"
	export KOTLIN_CINTEROP="$KOTLIN_DIR/bin/cinterop"
fi

# Fallback defaults if they were just set but KOTLINC isn't fully exported upstream
if [ -z "$KOTLINC" ] || [ "$KOTLINC" = "kotlinc-native" ]; then
	if [ -f "$KOTLIN_DIR/bin/kotlinc-native" ]; then
		KOTLINC="$KOTLIN_DIR/bin/kotlinc-native"
	fi
fi

# Check that the compiler actually exists at the resolved path
if ! command -v "$KOTLINC" >/dev/null 2>&1; then
	echo >&2 "***"
	echo >&2 "*** Kotlin/Native compiler ('$KOTLINC') could not be found or executed."
	echo >&2 "***"
	exit 1
fi

# Check the Kotlin/Native compiler version.
kotlin_version_output=$(
	LC_ALL=C "$KOTLINC" -version 2>&1
) || kotlin_version_code=$?
if [ -n "$kotlin_version_code" ]; then
	echo >&2 "***"
	echo >&2 "*** Running '$KOTLINC' to check the Kotlin version failed with"
	echo >&2 "*** code $kotlin_version_code. See output below for details:"
	echo >&2 "***"
	echo >&2 "$kotlin_version_output"
	echo >&2 "***"
	exit 1
fi

kotlin_version=$(
	echo "$kotlin_version_output" \
		| sed -nE 's:.*[Kk]otlin[^ ]* ([0-9]+\.[0-9]+\.[0-9]+).*:\1:p' | head -1
)
if [ -z "$kotlin_version" ]; then
	echo >&2 "***"
	echo >&2 "*** Could not determine Kotlin version from output:"
	echo >&2 "***"
	echo >&2 "$kotlin_version_output"
	echo >&2 "***"
	exit 1
fi

# Check that the cinterop tool exists (for C header bindings)
if [ -n "${KOTLIN_CINTEROP+x}" ]; then
	if ! command -v "$KOTLIN_CINTEROP" >/dev/null 2>&1; then
		echo >&2 "***"
		echo >&2 "*** Kotlin/Native cinterop tool '$KOTLIN_CINTEROP' could not be found."
		echo >&2 "***"
		exit 1
	fi
fi

echo "Kotlin/Native version: $kotlin_version"
