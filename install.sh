#!/bin/sh

# ASCII art and colors
RED='\033[1;31m'
GREEN='\033[1;32m'
YELLOW='\033[1;33m'
CYAN='\033[1;36m'
BLINK='\033[5m'
RESET='\033[0m'
GXX=${1:-g++}  # Use first argument or default to g++

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
    printf "█      SETUP SCRIPT       █\n"
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

# Get script directory and validate environment
SCRIPT_DIR="$( cd "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
cd "$SCRIPT_DIR" || exit 1

if [ ! -d "stdlib" ] || [ ! -f "main.cpp" ]; then
    printf "\n${RED}╔════════════════════════════════════════════╗"
    printf "\n${RED}║ ${BLINK}🚫 ERROR: Incorrect execution location! ${RED}║"
    printf "\n${RED}╚════════════════════════════════════════════╝"
    printf "\n${YELLOW}Please run this script from the ICVast project root\n"
    printf "directory containing both:\n"
    printf "${CYAN}• stdlib/ ${YELLOW}and${CYAN} • main.cpp${RESET}\n\n"
    exit 1
fi

# Shell configuration
if echo "$SHELL" | grep -qi "zsh"; then
    RC_FILE="$HOME/.zshrc"
    SHELL_NAME="ZSH"
elif echo "$SHELL" | grep -qi "bash"; then
    RC_FILE="$HOME/.bashrc"
    SHELL_NAME="Bash"
else
    printf "\n${RED}⚠ Unsupported shell!${RESET}"
    printf "\n${YELLOW}Manually add to your shell config:"
    printf "\n${CYAN}export CVAST_STDLIB=\"$SCRIPT_DIR/stdlib\"${RESET}\n\n"
    exit 1
fi

# Update shell config
EXPORT_LINE="export CVAST_STDLIB=\"$SCRIPT_DIR/stdlib\""

# Check if export already exists
CURRENT_EXPORT=$(grep '^export CVAST_STDLIB=' "$RC_FILE")
if [ -n "$CURRENT_EXPORT" ]; then
    printf "\n${YELLOW}⚠ Existing CVAST_STDLIB found:${RESET}\n${CYAN}%s${RESET}\n" "$CURRENT_EXPORT"
    printf "${YELLOW}Do you want to update it to:${RESET}\n${CYAN}%s${RESET}? [y/N] " "$EXPORT_LINE"
    read -r UPDATE_CONFIRM
    case "$UPDATE_CONFIRM" in
        [yY][eE][sS]|[yY])
            # Proceed with update
            sed -i.bak '/^export CVAST_STDLIB=/d' "$RC_FILE"
            echo "$EXPORT_LINE" >> "$RC_FILE"
            printf "\n${GREEN}✓ Updated CVAST_STDLIB configuration${RESET}\n"
            ;;
        *)
            printf "\n${YELLOW}↳ Keeping existing CVAST_STDLIB configuration${RESET}\n"
            ;;
    esac
else
    # Add new export if none exists
    echo "$EXPORT_LINE" >> "$RC_FILE"
    printf "\n${GREEN}✓ Added CVAST_STDLIB configuration${RESET}\n"
fi

printf "\n${GREEN}↳ Current ${SHELL_NAME} configuration: ${CYAN}${RC_FILE}${RESET}"
printf "\n${YELLOW}▸ Active environment variable:"
printf "\n${CYAN}%s${RESET}\n" "$EXPORT_LINE"

# Compilation phase
printf "\n\n${GREEN}▛▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▜"
printf "\n${GREEN}▌ Using compiler: %-10s ▐" "$GXX"
printf "\n${GREEN}▙▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▟${RESET}\n"

# Compiler validation
if ! command -v "$GXX" >/dev/null 2>&1; then
    printf "\n${RED}╔════════════════════════════════════════════╗"
    printf "\n${RED}║ ${BLINK}🚫 ERROR: $GXX compiler missing! ${RED}          ║"
    printf "\n${RED}╚════════════════════════════════════════════╝"
    printf "\n${YELLOW}Install with:${RESET}"
    printf "\n${CYAN}• Debian/Ubuntu: sudo apt install ${GXX}\n"
    printf "${CYAN}• macOS: brew install ${GXX}\n"
    printf "${CYAN}• Arch: pacman -S ${GXX}${RESET}\n\n"
    exit 1
fi

# Version check
GPP_MAJOR=$("$GXX" -dumpversion | cut -d. -f1)
if [ "$GPP_MAJOR" -lt 11 ]; then
    printf "\n${RED}╔════════════════════════════════════════════╗"
    printf "\n${RED}║ ${BLINK}🚫 ERROR: Need ${GXX}-11+ (found %-2s) ${RED}         ║" "$GPP_MAJOR"
    printf "\n${RED}╚════════════════════════════════════════════╝"
    printf "\n${YELLOW}Update with:${RESET}"
    printf "\n${CYAN}• Debian/Ubuntu: sudo apt install ${GXX}-11\n"
    printf "${CYAN}• macOS: brew install ${GXX}@11${RESET}\n\n"
    exit 1
fi

# Compilation with visual feedback
printf "${CYAN}\n🌀 Compiling with ${GXX} (C++20 mode)...${RESET}"
spin='-\|/'
i=0
if ! "$GXX" main.cpp -o InterpretedCVast -std=c++20 2> compile.log; then
    printf "\r${RED}✗ Compilation failed!${RESET}\n"
    printf "${YELLOW}Error log:${RESET}\n"
    cat compile.log
    rm compile.log
    exit 1
fi
rm compile.log

# Final instructions
success_animation
printf "\n${GREEN}🎉 Success! ${YELLOW}Run these to start:${RESET}"
printf "\n${CYAN}source ${RC_FILE}  # Load new environment"
printf "\n${CYAN}./InterpretedCVast  # Start interpreter${RESET}"

# Alias creation prompt
printf "\n\n${YELLOW}Create permanent 'cvast' alias?${RESET}"
printf "\n${GREEN}This will add to your ${RC_FILE}:"
printf "\n${CYAN}alias cvast='%s/InterpretedCVast'${RESET}" "$SCRIPT_DIR"
printf "\n${YELLOW}Create alias? [y/N] ${RESET}"
read -r ALIAS_CONFIRM

case "$ALIAS_CONFIRM" in
    [yY][eE][sS]|[yY])
        # Check for existing alias
        if ! grep -q "^alias cvast=" "$RC_FILE"; then
            echo "alias cvast='$SCRIPT_DIR/InterpretedCVast'" >> "$RC_FILE"
            printf "\n${GREEN}✓ Alias created! Use ${CYAN}cvast${GREEN} after reloading shell${RESET}\n"
        else
            printf "\n${YELLOW}⚠ Alias already exists in ${RC_FILE}${RESET}\n"
        fi
        ;;
    *)
        printf "\n${YELLOW}↳ Skipping alias creation${RESET}\n"
        ;;
esac

printf "\n${YELLOW}Tip: ${GREEN}Run ${CYAN}source ${RC_FILE}${GREEN} or restart terminal to apply changes!${RESET}\n\n"