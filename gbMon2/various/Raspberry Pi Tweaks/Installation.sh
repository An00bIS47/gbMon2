#!/bin/sh

#  Installation.sh
#  gbMon2
#
#  Created by michael on 17.01.14.
#  Copyright (c) 2014 michael. All rights reserved.

#
# lighttpd
#
# install and create directory /var/wwwlive with the proper permissions
#
sudo apt-get install lighttpd
sudo mkdir /var/www/live
sudo chown www-data:www-data /var/www/live
sudo chmod -R 775 /var/www/live


#
# create dir for sources in ~/sources
#
mkdir ~/sources


#
# wiringPi
#
cd ~/sources
git clone git://git.drogon.net/wiringPi
cd wiringPi
sudo ./build

#
# rrdtool
#
sudo apt-get install rrdtool
sudo apt-get install librrd-dev


#
# ramdisks
#
# edit /etc/fstab and add the following lines
# !! without the leading # !!
#
#tmpfs /tmp				tmpfs	defaults,noatime,mode=1777	0       0
#tmpfs /var/log			tmpfs	defaults,noatime,mode=0755	0       0
#tmpfs /var/lock		tmpfs	defaults,noatime,mode=0755	0		0
#tmpfs /var/www/live	tmpfs	defaults,noatime,mode=0755	0		0
#tmpfs /home/pi/.gbmon/ramdisk	tmpfs	defaults,noatime,mode=0700	0		0

#
# copy scripts to create directories needed on ramdisk
# these scripts will be run on every reboot and shutdown
# the persist folder of gbmon is located at
# ~/.gbmon/persist
#
sudo cp prepare-ramdisk /etc/init.d/
sudo cp persist-ramdisk /etc/init.d/
sudo chmod +x /etc/init.d/prepare-ramdisk
sudo chmod +x /etc/init.d/persist-ramdisk
sudo update-rc.d prepare-ramdisk defaults 01 99
sudo update-rc.d persist-ramdisk defaults 02 98

#
# gbMon2
#
# create directories requiered for gbMon2
mkdir -p ~/.gbmon/persist/db
mkdir -p ~/.gbmon/persist/pics
mkdir -p ~/.gbmon/persist/graphs

# download repository and build it
# to update use the following command:
#
# cd ~/gbMon2
# git pull origin
#
# and then build with:
# make all
#
cd ~
git clone https://github.com/An00bIS47/gbMon2.git
cd gbMon2/gbMon2
make all
cp ~/gbMon2/gbMon2/default.conf ~/.gbmon/ramdisk/gbmon2.conf

#
#
# Verify that:
# The modules are in this order in your /etc/modules
#
# w1-therm
# w1-gpio
# i2c-dev
# i2c-bcm2708
# spi-bcm2708
# snd-bcm2835
#
# and that the /etc/modprobe.d/raspi-blacklist.conf:
# #blacklist spi-bcm2708
# blacklist i2c-bcm2708

