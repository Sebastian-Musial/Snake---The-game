#!/bin/sh
printf '\033c\033]0;%s\a' Snake the game
base_path="$(dirname "$(realpath "$0")")"
"$base_path/Snake - MVP test version.x86_64" "$@"
