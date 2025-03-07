#!/bin/sh

# Check if ~/ICVast exists
if [ ! -d "$HOME/ICVast" ]; then
    echo "\033[31mWarning: ~/ICVast does not exist. Please move it to your home directory before running this script.\033[0m"
    exit 1
fi

# Determine the rc file based on the default shell
if echo "$SHELL" | grep -qi "zsh"; then
    RC_FILE="$HOME/.zshrc"
elif echo "$SHELL" | grep -qi "bash"; then
    RC_FILE="$HOME/.bashrc"
else
    echo "Unsupported shell. Only bash and zsh are supported. Please add the export manually."
    exit 1
fi

EXPORT_LINE='export CVAST_STDLIB="~/ICVast/stdlib"'

# Check if the export line is already in the file to avoid duplicates
if ! grep -qF "$EXPORT_LINE" "$RC_FILE"; then
    echo "$EXPORT_LINE" >> "$RC_FILE"
    echo "Added export line to $RC_FILE"
else
    echo "The export line is already present in $RC_FILE"
fi
