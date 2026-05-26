#!/bin/sh
# SPDX-License-Identifier: GPL-2.0
#
# Tests whether Gradle is available.

set -e

print_docs_reference()
{
	echo >&2 "***"
	echo >&2 "*** Please see Documentation/java/quick-start.rst for details."
	echo >&2 "***"
}

trap 'if [ $? -ne 0 ]; then print_docs_reference; fi' EXIT

if [ -z "${GRADLE+x}" ]; then
	echo >&2 "*** Environment variable 'GRADLE' is not set."
	exit 1
fi

GRADLE_DIR="$(dirname $0)/../.gradle-tools"
if ! command -v "$GRADLE" >/dev/null; then
	if [ ! -f "$GRADLE_DIR/gradle/bin/gradle" ]; then
		echo >&2 "***"
		echo >&2 "*** Gradle not found in PATH."
		echo >&2 "*** Automatically downloading Gradle..."
		echo >&2 "***"
		
		mkdir -p "$GRADLE_DIR"
		wget -qO- "https://services.gradle.org/distributions/gradle-8.7-bin.zip" > "$GRADLE_DIR/gradle.zip"
		unzip -q "$GRADLE_DIR/gradle.zip" -d "$GRADLE_DIR"
		mv "$GRADLE_DIR/gradle-8.7" "$GRADLE_DIR/gradle"
		rm "$GRADLE_DIR/gradle.zip"
	fi
	export GRADLE="$GRADLE_DIR/gradle/bin/gradle"
fi

# Fallback check
if [ -z "$GRADLE" ] || [ "$GRADLE" = "gradle" ]; then
	if [ -f "$GRADLE_DIR/gradle/bin/gradle" ]; then
		GRADLE="$GRADLE_DIR/gradle/bin/gradle"
	fi
fi

if ! command -v "$GRADLE" >/dev/null; then
	echo >&2 "*** Gradle could not be found."
	exit 1
fi

gradle_version=$("$GRADLE" -v | grep -oP 'Gradle \K[0-9.]+' || true)
echo "Gradle version: $gradle_version"
