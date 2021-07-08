CC              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)gcc'
LD              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ld'

RM = rm -f

SRCS := \
microai_hello_world.c hello_world_model.c $(MICROAI_LIBRARY)

OBJS := \
$(patsubst %.c,%.o,$(SRCS))

CCFLAGS += -std=gnu99 -I../../microai

LDFLAGS += -Wl,--fatal-warnings -Wl,--gc-sections -lm -lstdc++

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
	
microai_hello_world : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS) $(LDFLAGS)

all: microai_hello_world

clean:
	-$(RM) $(OBJS)
	-$(RM) microai_hello_world
