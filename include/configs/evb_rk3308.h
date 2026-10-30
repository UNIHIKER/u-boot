/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __EVB_RK3308_H
#define __EVB_RK3308_H

#include <configs/rk3308_common.h>

/* CONFIG_SUPPORT_EMMC_RPMB 已由 Kconfig/autoconf 提供，勿在此重复 #define */
/* mmc0=eMMC，环境变量放板载 eMMC */
#define CONFIG_SYS_MMC_ENV_DEV 0


#define ROCKCHIP_DEVICE_SETTINGS \
			"stdout=serial,vidconsole\0" \
			"stderr=serial,vidconsole\0"
#undef CONFIG_CONSOLE_SCROLL_LINES
#define CONFIG_CONSOLE_SCROLL_LINES            10
#	define CONFIG_SYS_MAX_FLASH_BANKS	1	/* max number of memory banks */
#	define CONFIG_SYS_MAX_FLASH_SECT	2	/* max number of sectors on one chip */
#ifndef CONFIG_SPL_BUILD
/* Unihiker：p5=boot，直接加载 boot.scr 并执行 */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"run rkimg_bootdev; " \
	"setenv distro_bootpart 5; " \
	"load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} boot.scr; " \
	"source ${scriptaddr}"
#endif

#endif
