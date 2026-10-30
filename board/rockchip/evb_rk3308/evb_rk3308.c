/*
 * (C) Copyright 2018 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <adc.h>
#ifndef CONFIG_SPL_BUILD
#include <dm.h>
#include <asm/gpio.h>
#include <linux/delay.h>
#endif

DECLARE_GLOBAL_DATA_PTR;

#define KEY_DOWN_MIN_VAL        0
#define KEY_DOWN_MAX_VAL        30

/*
 * Two board variants whith adc channel 3 is for board id
 * v10: 1024, v11: 512
 * v10: adc channel 0 for dnl key
 * v11: adc channel 1 for dnl key
 */
int rockchip_dnl_key_pressed(void)
{
	unsigned int key_val, id_val;
	int key_ch;

	if (adc_channel_single_shot("saradc", 3, &id_val)) {
		printf("%s read board id failed\n", __func__);
		return false;
	}

	if (abs(id_val - 1024) <= 30)
		key_ch = 0;
	else
		key_ch = 1;

	if (adc_channel_single_shot("saradc", key_ch, &key_val)) {
		printf("%s read adc key val failed\n", __func__);
		return false;
	}

	if ((key_val >= KEY_DOWN_MIN_VAL) && (key_val <= KEY_DOWN_MAX_VAL))
		return true;
	else
		return false;
}

#ifndef CONFIG_SPL_BUILD
/*
 * 上电时长按 A（GPIO0_C1，上拉，按下为低）满 10 秒才设置 recover=1。
 * 未按下不延时。boot.cmd 在导入 uEnv.txt 之前保存该值，为 1 则加载 recovery 分区。
 */
#define KEY_A_HOLD_MS		10000
#define KEY_A_POLL_MS		100
static int unihiker_key_a_probe(struct udevice *dev)
{
	return 0;
}

static const struct udevice_id unihiker_key_a_ids[] = {
	{ .compatible = "dfrobot,unihiker-key-a" },
	{ }
};

U_BOOT_DRIVER(unihiker_key_a) = {
	.name = "unihiker_key_a",
	.id = UCLASS_NOP,
	.of_match = unihiker_key_a_ids,
	.probe = unihiker_key_a_probe,
};

int rk_board_late_init(void)
{
	struct udevice *dev;
	struct gpio_desc key;
	int ret, i;

	ret = uclass_get_device_by_driver(UCLASS_NOP,
					  DM_GET_DRIVER(unihiker_key_a), &dev);
	if (ret) {
		printf("A key: device not found (%d)\n", ret);
		return 0;
	}

	ret = gpio_request_by_name(dev, "gpios", 0, &key, GPIOD_IS_IN);
	if (ret) {
		printf("A key: gpio request failed (%d)\n", ret);
		return 0;
	}

	/* 等上拉稳定后再采样，避免刚切到 GPIO 时读到中间电平 */
	udelay(2000);
	if (dm_gpio_get_value(&key) <= 0) {
		env_set("recover", "0");
		return 0;
	}

	printf("Hold A for %ds to enter recovery\n", KEY_A_HOLD_MS / 1000);
	for (i = 0; i < KEY_A_HOLD_MS / KEY_A_POLL_MS; i++) {
		mdelay(KEY_A_POLL_MS);
		if (dm_gpio_get_value(&key) <= 0) {
			printf("A key released, normal boot\n");
			env_set("recover", "0");
			return 0;
		}
		if (((i + 1) % (1000 / KEY_A_POLL_MS)) == 0)
			printf("A key held %ds\n", (i + 1) / (1000 / KEY_A_POLL_MS));
	}

	printf("A key held %ds, enter recovery\n", KEY_A_HOLD_MS / 1000);
	env_set("recover", "1");
	return 0;
}
#endif
