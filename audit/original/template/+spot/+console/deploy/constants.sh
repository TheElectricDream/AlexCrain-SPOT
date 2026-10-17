#!/usr/bin/env bash
# SPOT's constants for the shell scripts. The deploy and setup scripts have no MATLAB or Python of
# the project to hand, so they read Constants.m (the one source of every parameter) through
# this file:
#   source "$(dirname "$0")/constants.sh"          # from +spot/+console/deploy
#   TX2_IP=$(spot_constant IP_LAB_CONSOLE)           # a 'string' or a number
# spot_constant NAME prints the value of the line "NAME = value;" or "NAME = 'value';", and fails
# (which stops a script run with set -e) when NAME isn't there. Plain values only: a constant
# defined by arithmetic on others isn't worked out (the scripts need addresses, login names, ports
# and device paths).
# Constants.m is three folders up from this file (the project folder); on the TX2, where
# deploy_tx2.sh puts the project's code for the Documentation tab, it is in docs_source.
SPOT_CONSTANTS_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SPOT_CONSTANTS_FILE=""
for spot_candidate in "$SPOT_CONSTANTS_DIR/../../../Constants.m" \
                      "$SPOT_CONSTANTS_DIR/../docs_source/Constants.m"; do
    if [ -f "$spot_candidate" ]; then SPOT_CONSTANTS_FILE="$spot_candidate"; break; fi
done
unset spot_candidate
[ -n "$SPOT_CONSTANTS_FILE" ] || { echo "constants.sh: no Constants.m near $SPOT_CONSTANTS_DIR" >&2; exit 1; }

spot_constant() {   # NAME: prints its value from Constants.m
    local value
    value=$(sed -n -E "s/^[[:space:]]*$1[[:space:]]*=[[:space:]]*'?([^';]*)'?[[:space:]]*;.*$/\1/p" \
                "$SPOT_CONSTANTS_FILE" | head -n 1)
    [ -n "$value" ] || { echo "constants.sh: $1 is not in $SPOT_CONSTANTS_FILE" >&2; return 1; }
    printf '%s\n' "$value"
}
