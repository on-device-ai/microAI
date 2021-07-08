CXX             := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)g++'
AR              := '$(TARGET_TOOLCHAIN_ROOT)$(TARGET_TOOLCHAIN_PREFIX)ar' 

RM = rm -f

ARFLAGS := -csr

SRCS := \
libmicroai-image-provider.cc

OBJS := \
$(patsubst %.cc,%.o,$(SRCS))

IMAGE_PROVIDER_LIB = libmicroai-image-provider.a

CXXFLAGS = `pkg-config --cflags opencv` -I..

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(IMAGE_PROVIDER_LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(IMAGE_PROVIDER_LIB) $(OBJS)

all: $(IMAGE_PROVIDER_LIB)

clean:
	-$(RM) $(OBJS)
	-$(RM) ${IMAGE_PROVIDER_LIB}
