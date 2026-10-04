# Nva router udp core

面向 OpenWrt/ImmortalWrt `ramips/mt7621` (`mipsel_24kc`) 的轻量 UDP 本地解析框架。

当前仓库包含：

- `src/nva_udp_core.c`：Linux UDP 命令行程序骨架
- `package/nva-udp-core/`：OpenWrt package Makefile
- `scripts/build-openwrt-mipsel24kc.ps1`：交叉编译入口脚本

目标设备示例：Xiaomi Redmi Router AC2100 / MT7621 / ImmortalWrt 24.10.x。
