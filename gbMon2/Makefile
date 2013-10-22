APP			= gbMon2
CC			= gcc
CFLAGS		= -c
LDFLAGS		= -lwiringPi -lpthread
#SHELL		= bash

OBJDIR  = obj

SRCS    := $(wildcard *.c)
OBJS    := $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

all: buildrepo $(APP) clean

$(APP) :  $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@


$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf  $(OBJS)
	rm -rf $(OBJDIR)

distclean: clean
	rm -rf $(APP)

	
buildrepo:
	@$(call make-repo)

# Create obj directory structure
define make-repo
	mkdir -p $(OBJDIR)
endef
