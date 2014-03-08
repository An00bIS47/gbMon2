#
#  Makefile
#  gbmon2
#
#  Created by michael on 27.10.13.
#  Copyright (c) 2013 michael. All rights reserved.
# 
#  Muss an RCSwitch angepasst werden --> g++ compilieren
#  --> main + rcswitch + rcswitchwrapper mit g++ evtl fan lib?
#
TARGET		= gbMon2
CC			= gcc
CFLAGS		= -g -Wall -O3 -std=gnu99
LDFLAGS		= -lwiringPi -lpthread -lrt -lrrd -lavahi-client -lavahi-common
CFLAGS += `MagickWand-config --cflags --cppflags`
LDFLAGS += `MagickWand-config --ldflags --libs`
#SHELL		= bash

OBJDIR		= obj

SRCS		:= $(wildcard *.c)

OBJS		= debug.o helpers.o udpServer.o settings.o main.o lcddisplay.o lcd.o dht22.o rcswitchwrapper.o rcswitch.o rc434.o data.o ringbuffer.o rrdtool.o ds18b20.o piCam.o install.o avahi.o wand.o ioBridge.o screenTemperature.o fonts/fnt_dejavu.o fonts/fnt_dejavu_9.o fonts/fnt_dejavu_9_b.o fonts/fnt_silkscreen_8.o fonts/fnt_spaceLex_8.o fonts/fnt_spaceLex_12.o fonts/fnt_spaceLex_8b.o fonts/fnt_spaceLex_5.o


#OBJS		:= $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
#OBJS		:= fonts/fnt_dejavu.o fonts/fnt_dejavu_9.o fonts/fnt_dejavu_9_b.o fonts/fnt_silkscreen_8.o fonts/fnt_spaceLex_8.o

default:	update clean info $(TARGET) endinfo

all:		update clean info $(TARGET) endinfo

help:
			@ echo "The following make targets are available"
			@ echo " all	- make everything"
			@ echo " help	- print this message"
			@ echo " clean	- remove any temporary files"
			@ echo " update	- update gbMon2 from git"
			@ echo " "
			@ echo " + Make choice for tools here too!!!"
			@ echo " + EyeCandy with Echo like this help !!"

$(TARGET):  $(OBJS)
	g++ $(OBJS) $(LDFLAGS) -o $@

rcswitch.o : rcswitch.cpp
	g++ -c rcswitch.cpp -o rcswitch.o $(LDFLAGS)

rcswitchwrapper.o : rcswitchwrapper.cpp
	g++ -c rcswitchwrapper.cpp -o rcswitchwrapper.o $(LDFLAGS)
	
$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	
info:
	@echo "Building gbMon2"
	@echo "=========================================="

endinfo:
	@echo ""
	@echo ""
	@echo "Done building gbMon2!"
	@echo "Type sudo ./gbMon2 to start"
	@echo "=========================================="

clean:
	@echo "Cleaning gbMon2"
	@echo "=========================================="
	rm -rf  $(OBJS)
	rm -rf $(TARGET)
#	rm -rf $(OBJDIR)
	@echo ""
	@echo ""

distclean: clean
	@echo "Removing old gbMon2"
	@echo "=========================================="
	rm -rf $(TARGET)
	@echo ""
	@echo ""
	
update:
	@echo "Updating gbMon2"
	@echo "=========================================="
	git pull origin
	@echo ""
	@echo ""

buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
endef
