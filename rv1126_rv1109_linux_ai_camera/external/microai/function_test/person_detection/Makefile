CC              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)gcc'
LD              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ld'

RM = rm -f

SRCS := \
microai_person_detection.c person_detection_model.c $(MICROAI_LIBRARY) $(MICROAI_IMAGE_PROVIDER_LIBRARY)

OBJS := \
$(patsubst %.c,%.o,$(SRCS))

CCFLAGS += -std=gnu99 -I../../microai `pkg-config --cflags opencv` 

LDFLAGS += -Wl,--fatal-warnings -Wl,--gc-sections -lm -lstdc++ --sysroot=$(STAGING_DIR) -lv4l2 `pkg-config --libs opencv`

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@
	
microai_person_detection : $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $(OBJS) $(LDFLAGS)

all: microai_person_detection

clean:
	-$(RM) $(OBJS)
	-$(RM) microai_person_detection
