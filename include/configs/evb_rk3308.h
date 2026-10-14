/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __EVB_RK3308_H
#define __EVB_RK3308_H

#include <configs/rk3308_common.h>

#define CONFIG_SYS_MMC_ENV_DEV 0

#define ROCKCHIP_DEVICE_SETTINGS \
			"stdout=serial,vidconsole\0" \
			"stderr=serial,vidconsole\0"
#undef CONFIG_CONSOLE_SCROLL_LINES
#define CONFIG_CONSOLE_SCROLL_LINES            10

#ifndef CONFIG_SPL_BUILD
#undef CONFIG_BOOTCOMMAND
#if defined(CONFIG_ROCKCHIP_FIT_IMAGE) && !defined(CONFIG_ANDROID_BOOT_IMAGE)
# ifdef CONFIG_RK3308_UNIHIKER
/* Unihiker: FAT boot partition (mmc 0:4) with boot.scr + Image + DTB */
#  define CONFIG_BOOTCOMMAND "load mmc 0:4 0x00500000 boot.scr; source 0x00500000"
# else
#  define CONFIG_BOOTCOMMAND "boot_fit;"
# endif
#else
#define CONFIG_BOOTCOMMAND RKIMG_BOOTCOMMAND
#endif
#endif

#endif
