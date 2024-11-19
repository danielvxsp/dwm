#!/bin/sh
wal -R -n && xwallpaper --zoom /home/ninog/Pictures/backgrounds/500735ldsdl.jpg
slstatus &
picom --config ~/.config/picom/picom.conf &

# Startup scripts
~/scripts/updatenoti.sh &
