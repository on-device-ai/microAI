################################################################################
#
# microai_functest
#
################################################################################

MICROAI_FUNCTEST_SITE = $(TOPDIR)/../external/microai/function_test
MICROAI_FUNCTEST_SITE_METHOD = local
MICROAI_FUNCTEST_DEPENDENCIES = microai

MICROAI_FUNCTEST_MAKE_OPTS = TARGET_TOOLCHAIN_ROOT="$(TOPDIR)/../prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/"
MICROAI_FUNCTEST_MAKE_OPTS += TARGET_TOOLCHAIN_PREFIX="arm-linux-gnueabihf-"
MICROAI_FUNCTEST_MAKE_OPTS += MICROAI_LIBRARY="$(STAGING_DIR)/usr/lib/libmicroai.a"
MICROAI_FUNCTEST_MAKE_OPTS += MICROAI_IMAGE_PROVIDER_LIBRARY="$(STAGING_DIR)/usr/lib/libmicroai-image-provider.a"

define MICROAI_FUNCTEST_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/hello_world $(MICROAI_FUNCTEST_MAKE_OPTS)
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/image_provider $(MICROAI_FUNCTEST_MAKE_OPTS)
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)/person_detection $(MICROAI_FUNCTEST_MAKE_OPTS)
endef

define MICROAI_FUNCTEST_INSTALL_TARGET_CMDS
	$(INSTALL) -D -m 0755 $(@D)/hello_world/microai_hello_world $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/image_provider/microai_image_provider $(TARGET_DIR)/usr/bin
	$(INSTALL) -D -m 0755 $(@D)/person_detection/microai_person_detection $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
