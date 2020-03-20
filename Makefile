# compiler shit
CC = gcc
LIBS = -lm -lSDL -lSDL_image -lSDL_mixer
INCLUDE = -I. -I./src
# obj shit
OBJS := src/main.o src/bios.o src/keine.o src/mokou.o src/kappamap.o
OBJS += src/kanako.o src/player.o
OBJS += src/sinlut.o src/fade.o

NAME = build/raymo.out
CPPFLAGS += -g

%.o: %.c %.cpp
	$(CC) -c -o $@ $^ $(INCLUDE)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) -g

clean:
	rm $(OBJS)
	rm $(NAME)

