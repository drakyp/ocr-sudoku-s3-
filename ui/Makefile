CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` `pkg-config --cflags SDL2_image`
LDLIBS = `pkg-config --libs gtk+-3.0` -lm `pkg-config --libs SDL2_image` 

TARGET=ui
OBJS=src/ui.o ../filters/src/filters.o 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

debug: CFLAGS+=-g -fsanitize=address
debug: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJS) *.txt *.png
