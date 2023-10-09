#!/bin/bash

_lcd_set () {
    local cur=${COMP_WORDS[COMP_CWORD]}
    if [ $COMP_CWORD -eq 1 ]; then
        local link_path=$(readlink -f /usr/bin/rpi-lcd)
        local dir_path=$(dirname $link_path)
        local files=$(ls $dir_path/tft)
        COMPREPLY=($(compgen -W "$files" -- $cur))
    elif [ $COMP_CWORD -eq 2 ]; then
        local functions=$(grep -oP '^[^_]\w+\s*\(\)' /usr/bin/rpi-lcd | sed 's/()//')
        COMPREPLY=($(compgen -W "$functions" -- $cur))
    elif [ $COMP_CWORD -eq 3 ]; then
        local function_name=${COMP_WORDS[2]}
        local variable_name="${function_name}_switch"
        local variable_value=$(grep -oP "(?<=^$variable_name=\").*?(?=\")" /usr/bin/rpi-lcd)
        COMPREPLY=($(compgen -W "$variable_value" -- $cur))
    fi
}
complete -F _lcd_set rpi-lcd
