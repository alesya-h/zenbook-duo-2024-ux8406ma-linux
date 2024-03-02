#!/bin/bash
set -ex
package_name=duo-services

startdir=$(pwd)
echo "Cleaning any old install"
# clean install
# remove code dir
rm -rf /usr/local/$package_name
# remove services
rm -rf /etc/systemd/system/duo-brightness-sync.service
# remove links
rm -rf /usr/local/bin/gnome-monitor-config
rm -rf /usr/local/bin/duo

# make folder in /usr/local/
mkdir /usr/local/$package_name
echo "Copying Files"

# copy duo script
cp ../duo /usr/local/$package_name/duo
chmod +x /usr/local/$package_name/duo

echo "Build and Installing https://github.com/jadahl/gnome-monitor-config"
# get gnome-monitor-config
rm -rf /tmp/$package_name
mkdir /tmp/$package_name
cd /tmp/$package_name
git clone https://github.com/jadahl/gnome-monitor-config
cd gnome-monitor-config
meson build
cd build
meson compile
cp src/gnome-monitor-config /usr/local/$package_name/gnome-monitor-config
chmod +x /usr/local/$package_name/gnome-monitor-config
rm -rf /tmp/$package_name

echo "Linking Executables"

# make linkes to /usr/local/bin
ln -s /usr/local/$package_name/gnome-monitor-config /usr/local/bin/gnome-monitor-config
ln -s /usr/local/$package_name/duo /usr/local/bin/duo

# copy unit file into place
echo "Setting up Services"
cd $startdir
cp duo-brightness-sync.service /etc/systemd/system/duo-brightness-sync.service

# start service
systemctl daemon-reload
systemctl enable --now duo-brightness-sync
