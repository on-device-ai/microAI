################################################################################
#
# microai
#
################################################################################

MICROAI_SITE = $(TOPDIR)/../external/microai
MICROAI_SITE_METHOD = local
MICROAI_DEPENDENCIES = tflite-micro libv4l opencv3

MICROAI_INSTALL_STAGING = YES

MICROAI_MAKE_OPTS = TARGET_TOOLCHAIN_ROOT="$(TOPDIR)/../prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/"
MICROAI_MAKE_OPTS += TARGET_TOOLCHAIN_PREFIX="arm-linux-gnueabihf-"
MICROAI_MAKE_OPTS += TFLITE_MICRO_PATH="../tflite-micro"

define MICROAI_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) $(MICROAI_MAKE_OPTS)
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/image_provider $(MICROAI_MAKE_OPTS)
endef

define MICROAI_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libmicroai.a $(STAGING_DIR)/usr/lib/libmicroai.a
	$(INSTALL) -D -m 0755 $(@D)/image_provider/libmicroai-image-provider.a $(STAGING_DIR)/usr/lib/libmicroai-image-provider.a
endef

$(eval $(generic-package))
