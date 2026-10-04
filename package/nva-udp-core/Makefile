include $(TOPDIR)/rules.mk

PKG_NAME:=nva-udp-core
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/nva-udp-core
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Nva UDP core for mipsel_24kc routers
endef

define Package/nva-udp-core/description
  Lightweight UDP local parser core for OpenWrt/ImmortalWrt routers.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/nva_udp_core.c $(PKG_BUILD_DIR)/
endef

define Build/Compile
	$(TARGET_CC) $(TARGET_CFLAGS) $(TARGET_LDFLAGS) -DNVA_VERSION=\"$(PKG_VERSION)\" -o $(PKG_BUILD_DIR)/nva-udp-core $(PKG_BUILD_DIR)/nva_udp_core.c
endef

define Package/nva-udp-core/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/nva-udp-core $(1)/usr/bin/nva-udp-core
endef

$(eval $(call BuildPackage,nva-udp-core))
