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
CFLAGS		= -g -Wall -O3
LDFLAGS		= -lwiringPi -lpthread -lrt
#SHELL		= bash

OBJDIR		= obj

SRCS		:= $(wildcard *.c)

OBJS		= debug.o helpers.o server.o settings.o main.o lcddisplay.o lcd.o dht22.o fonts/fnt_dejavu.o fonts/fnt_dejavu_9.o fonts/fnt_dejavu_9_b.o fonts/fnt_silkscreen_8.o fonts/fnt_spaceLex_8.o fonts/fnt_spaceLex_12.o fonts/fnt_spaceLex_8b.o fonts/fnt_spaceLex_5.o


#OBJS		:= $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))
#OBJS		:= fonts/fnt_dejavu.o fonts/fnt_dejavu_9.o fonts/fnt_dejavu_9_b.o fonts/fnt_silkscreen_8.o fonts/fnt_spaceLex_8.o

help:
			@ echo "The following make targets are available"
			@ echo " help	- print this message"
			@ echo " all	- make everything"
			@ echo " clean	- remove any temporary files"
			@ echo " "
			@ echo " + Make choice for tools here too!!!"
			@ echo " + EyeCandy with Echo like this help !!"
all:		$(TARGET) clean

$(TARGET):  $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@


$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf  $(OBJS)
	rm -rf $(OBJDIR)

distclean: clean
	rm -rf $(TARGET)

	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
endef
