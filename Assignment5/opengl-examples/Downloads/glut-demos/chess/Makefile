#!smake
#
# Copyright (c) Mark J. Kilgard, 1996, 1997.
#

TARGETS = chess

LDFLAGS = -L/usr/X11R6/lib -lglut -lGLU -lGL -lXext -lX11 -lm

SRCS = chess.c main.c pathplan.c animate.c texture.c
OBJS =  $(SRCS:.c=.o)

default : $(TARGETS)

chess : $(OBJS) $(GLUT)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
