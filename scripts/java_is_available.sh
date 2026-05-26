#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Tests whether a suitable Java toolchain is available.

set -e

# Print a reference to the documentation.
print_docs_reference()
{
	echo >&2 "***"
	echo >&2 "*** Please see Documentation/java/quick-start.rst for details"
	echo >&2 "*** on how to set up Java support."
	echo >&2 "***"
}

# Print an explanation about the fact that the script is meant to be called from Kbuild.
print_kbuild_explanation()
{
	echo >&2 "***"
	echo >&2 "*** This script is intended to be called from Kbuild."
	echo >&2 "*** Please use the 'javaavailable' target to call it instead."
	echo >&2 "*** Otherwise, the results may not be meaningful."
	exit 1
}

# If the script fails for any reason, print a reference to the documentation.
trap 'if [ $? -ne 0 ]; then print_docs_reference; fi' EXIT

# Check that the expected environment variables are set.
if [ -z "${JAVAC+x}" ]; then
	echo >&2 "***"
	echo >&2 "*** Environment variable 'JAVAC' is not set."
	print_kbuild_explanation
fi

# Check that the Java compiler exists.
if ! command -v "$JAVAC" >/dev/null; then
	echo >&2 "***"
	echo >&2 "*** Java compiler '$JAVAC' could not be found."
	echo >&2 "***"
	echo >&2 "*** Install OpenJDK 17 or later."
	echo >&2 "***   - Debian/Ubuntu: sudo apt install openjdk-17-jdk"
	echo >&2 "***   - Arch Linux: sudo pacman -S jdk17-openjdk"
	echo >&2 "***"
	exit 1
fi

# Check the Java compiler version.
java_version_output=$(
	LC_ALL=C "$JAVAC" -version 2>&1
) || java_version_code=$?
if [ -n "$java_version_code" ]; then
	echo >&2 "***"
	echo >&2 "*** Running '$JAVAC' to check the Java version failed with"
	echo >&2 "*** code $java_version_code. See output below for details:"
	echo >&2 "***"
	echo >&2 "$java_version_output"
	echo >&2 "***"
	exit 1
fi

java_version=$(
	echo "$java_version_output" \
		| sed -nE 's:.*javac ([0-9]+\.[0-9]+\.[0-9]+).*:\1:p' | head -1
)
if [ -z "$java_version" ]; then
	# Sometimes format is just "javac 17.0.8"
	java_version=$(
		echo "$java_version_output" \
			| sed -nE 's:.*javac ([0-9]+).*:\1:p' | head -1
	)
	if [ -z "$java_version" ]; then
		echo >&2 "***"
		echo >&2 "*** Could not determine Java version from output:"
		echo >&2 "***"
		echo >&2 "$java_version_output"
		echo >&2 "***"
		exit 1
	fi
fi

echo "Java version: $java_version"
