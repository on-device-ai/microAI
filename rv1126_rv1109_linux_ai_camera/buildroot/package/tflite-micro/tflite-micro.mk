################################################################################
#
# tflite-micro
#
################################################################################

TFLITE_MICRO_SITE = $(TOPDIR)/../external/tflite-micro
TFLITE_MICRO_SITE_METHOD = local

TFLITE_MICRO_INSTALL_STAGING = YES

TFLITE_MICRO_MAKE_OPTS = TARGET_TOOLCHAIN_ROOT="$(TOPDIR)/../prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/"
TFLITE_MICRO_MAKE_OPTS += TARGET_TOOLCHAIN_PREFIX="arm-linux-gnueabihf-"

define TFLITE_MICRO_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) $(TFLITE_MICRO_MAKE_OPTS) hello_world
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D) $(TFLITE_MICRO_MAKE_OPTS) lib
endef

define TFLITE_MICRO_INSTALL_STAGING_CMDS
	$(INSTALL) -D -m 0755 $(@D)/libtensorflow-microlite.a $(STAGING_DIR)/usr/lib/libtensorflow-microlite.a
endef

define TFLITE_MICRO_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/hello_world $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
