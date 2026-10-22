#!/bin/sh
python3 "$(dirname "$0")/../tools/run_tests.py" "$@"
STATUS=$?

# Only wait when run interactively.
if [ -t 0 ]; then
    printf "\nPress Enter to close..."
    read -r _
fi

exit $STATUS
