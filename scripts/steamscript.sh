
#DWM STEAM MONITORS SCRIPT
#Dependecies: xdotool, Dwm self restart patch (https://dwm.suckless.org/patches/selfrestart/), notify-send and some package that show the notification
#ONLY WORK IF YOU USING XORG
#If know i better way to write this script create a pull request.

#Description & Intructions
#Certain Steam games need to disable primary display to work in fullscreen without mess up fulscreen resolution
#This script ask to self restart dwm after close the game because because reativing the primary display make them mirror

#Use xrandr to find the display name (exemple output on the patch if you clone my repo or on this link: https://github.com/MrDuartePT/dwm/blob/main/scripts/xrand.exemple.steamscript.sh.png  
#Change eDP1 bellow to you screen name and and this to launch options: 'xrandr --output eDP1 --off; MANGOHUD=1; %command%; /patch/to/the/script', change eDPI and patch/to/the/script

xrandr --output eDP1 --auto

notify-send 'DWM will selfrestart, to restore the previous monitor configuration!!!'
while true; do
    read -p "You want to procced, 'N' will need to self restart yourself (Y/N): " yn
    case $yn in
        [Yy]* ) xdotool key Super_L+Shift+Q; break;;
        [Nn]* ) exit;;
        * ) echo "Plase Enter 'Y' or 'N'";;
    esac
done


