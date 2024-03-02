#!/bin/bash

# install bin(s) and systemd service
sudo ./install-needs-sudo.sh

# remove airplane mode toggle from keyboard event
gsettings set org.gnome.settings-daemon.plugins.media-keys rfkill-static "['XF86UWB', 'XF86RFKill']"

# add autostarts
gsettings set org.gnome.settings-daemon.plugins.media-keys custom-keybindings "['/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom0/']"
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom0/ name 'Duo Set Displays'
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom0/ command 'duo set-displays'
gsettings set org.gnome.settings-daemon.plugins.media-keys.custom-keybinding:/org/gnome/settings-daemon/plugins/media-keys/custom-keybindings/custom0/ binding 'WLAN'
