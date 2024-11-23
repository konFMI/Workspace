#!/bin/bash

# setup_git_aliases.sh

# Set Git aliases
git config --global alias.g "!git"
git config --global alias.a "add"
git config --global alias.ss "status"
git config --global alias.co "checkout"
git config --global alias.b "branch"
git config --global alias.l "log"
git config --global alias.ll "log --oneline --graph --decorate"
git config --global alias.c "commit"
git config --global alias.ca "commit --amend"
git config --global alias.sh "show HEAD"
git config --global alias.d "diff"
git config --global alias.ds "diff --staged"
git config --global alias.dc "diff --cached"
git config --global alias.r "reset"
git config --global alias.rh "reset --hard"
git config --global alias.rh1 "reset --hard HEAD~1"

# Determine which shell configuration file to modify
SHELL_PROFILE=""
if [ -n "$BASH_VERSION" ]; then
    SHELL_PROFILE="$HOME/.bashrc"
elif [ -n "$ZSH_VERSION" ]; then
    SHELL_PROFILE="$HOME/.zshrc"
else
    echo "Unsupported shell. Please add the alias manually."
    exit 1
fi

# Add the 'g' alias to the shell profile
ALIAS_SCRIPT="alias g='git'"

# Check if the alias is already in the shell profile
if ! grep -q "$ALIAS_SCRIPT" "$SHELL_PROFILE"; then
    echo "$ALIAS_SCRIPT" >> "$SHELL_PROFILE"
    echo "'g' alias added to $SHELL_PROFILE"
else
    echo "'g' alias already exists in $SHELL_PROFILE"
fi

# Apply changes immediately to the current session
source "$SHELL_PROFILE"

echo "Git aliases and shell 'g' alias have been set!"