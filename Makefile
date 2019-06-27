COMPILER = gcc
CFLAGS   = --Wall
INCLUDE  = -I./core
TARGET   = ./mao86
SRCROOT  = ./
OBJROOT  = ./obj
SRCDIRS  = $(shell find $(SRCROOT) -type d)
SOURCES := $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))
OBJECTS  = $(addprefix $(OBJROOT)/, $(SOURCES:.c=.o)) 
OBJDIRS  = $(addprefix $(OBJROOT)/, $(SRCDIRS)) 
DEPENDS  = $(OBJECTS:.o=.d)
$(TARGET): $(OBJECTS) $(LIBS)
	$(COMPILER) -o ./$@ $^ $(LDFLAGS)

$(OBJROOT)/%.o: $(SRCROOT)/%.c
	@if [ ! -e `dirname $@` ]; then mkdir -p `dirname $@`; fi
	$(COMPILER) $(CXXFLAGS) $(INCLUDE) -o $@ -c $<

-include $(DEPENDS)

