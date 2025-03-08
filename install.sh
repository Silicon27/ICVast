#!/bin/sh

# ASCII art and colors
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
BLINK='\033[5m'
RESET='\033[0m'

header() {
    clear
    printf "${CYAN}"
    printf "  ██╗ ██████╗██╗   ██╗ █████╗ ███████╗████████╗\n"
    printf "  ██║██╔════╝██║   ██║██╔══██╗██╔════╝╚══██╔══╝\n"
    printf "  ██║██║     ██║   ██║███████║███████╗   ██║   \n"
    printf "  ██║██║     ██║   ██║██╔══██║╚════██║   ██║   \n"
    printf "  ██║╚██████╗╚██████╔╝██║  ██║███████║   ██║   \n"
    printf "  ╚═╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═╝╚══════╝   ╚═╝   \n"
    printf "▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n"
    printf "█      SETUP SCRIPT      █\n"
    printf "▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀\n"
    printf "${RESET}\n"
}

success_animation() {
    printf "\n${GREEN}"
    printf "  ╭──────────────────────╮\n"
    printf "  │  ${YELLOW}✔ SUCCESS! ${GREEN}         │\n"
    printf "  ╰──────────────────────╯\n"
    printf "${RESET}"
}

progress_dots() {
    printf "${CYAN}Initializing setup "
    for i in $(seq 1 3); do
        printf "."
        sleep 0.3
    done
    printf "${RESET}\n\n"
}

header
progress_dots

# Determine the directory where this script is located
SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

# Check for stdlib directory
if [ ! -d "$SCRIPT_DIR/stdlib" ]; then
    printf "\n${RED}╔════════════════════════════════════════════╗"
    printf "\n${RED}║ ${BLINK}🚫 ERROR: Missing 'stdlib' directory! ${RED}║"
    printf "\n${RED}╚════════════════════════════════════════════╝"
    printf "\n\n${YELLOW}Please ensure:\n"
    printf "1. This script is in the ICVast project root\n"
    printf "2. The 'stdlib' directory exists\n"
    printf "3. Try: ${CYAN}ls -la $SCRIPT_DIR/stdlib${RESET}\n\n"
    exit 1
fi

# Determine shell configuration
if echo "$SHELL" | grep -qi "zsh"; then
    RC_FILE="$HOME/.zshrc"
    SHELL_NAME="ZSH"
elif echo "$SHELL" | grep -qi "bash"; then
    RC_FILE="$HOME/.bashrc"
    SHELL_NAME="Bash"
else
    printf "\n${RED}⚠ Unsupported shell!${RESET}"
    printf "\n${YELLOW}Please manually add this to your shell config:"
    printf "\n${CYAN}export CVAST_STDLIB=\"path/to/stdlib\"${RESET}\n\n"
    exit 1
fi

EXPORT_LINE="export CVAST_STDLIB=\"$SCRIPT_DIR/stdlib\""

# Update RC file
if sed -i.bak '/^export CVAST_STDLIB=/d' "$RC_FILE"; then
    echo "$EXPORT_LINE" >> "$RC_FILE"
    printf "\n${GREEN}↳ Updated ${SHELL_NAME} config: ${CYAN}${RC_FILE}${RESET}"
    printf "\n${YELLOW}▸ New environment variable:"
    printf "\n${CYAN}${EXPORT_LINE}${RESET}"

    success_animation
    printf "\n${GREEN}🎉 Setup complete! ${YELLOW}Run this to finish:\n"
    printf "${CYAN}source ${RC_FILE}${RESET}"
    printf "\n\n${YELLOW}Tip: ${GREEN}Restart your terminal for changes to take effect!${RESET}\n\n"
else
    printf "\n${RED}╔════════════════════════════════════════════╗"
    printf "\n${RED}║ ${BLINK}💥 CRITICAL ERROR: File update failed! ${RED}║"
    printf "\n${RED}╚════════════════════════════════════════════╝"
    printf "\n\n${YELLOW}Possible reasons:"
    printf "\n◼ No write permissions for ${RC_FILE}"
    printf "\n◼ Disk full or filesystem error"
    printf "\n◼ Antivirus interference\n\n"
    exit 1
fi