/**
 * WEIO Web Of Things Platform
 *
 * Copyright (C) 2013 Drasko DRASKOVIC and Uros PETREVSKI
 * All rights reserved
 *
 *              ##      ## ######## ####  #######  
 *              ##  ##  ## ##        ##  ##     ## 
 *              ##  ##  ## ##        ##  ##     ## 
 *              ##  ##  ## ######    ##  ##     ## 
 *              ##  ##  ## ##        ##  ##     ## 
 *              ##  ##  ## ##        ##  ##     ## 
 *               ###  ###  ######## ####  #######
 *
 *                   Web Of Things Platform 
 *
 * This file is part of WEIO
 * WEIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WEIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors : 
 * Drasko DRASKOVIC <drasko.draskovic@gmail.com>
 * Uros PETREVSKI <uros@nodesign.net>
 */

#include <asm/mach-ath79/ath79.h>
#include <asm/mach-ath79/ar71xx_regs.h>
#include "common.h"
#include "dev-eth.h"
#include "dev-gpio-buttons.h"
#include "dev-leds-gpio.h"
#include "dev-m25p80.h"
#include "dev-spi.h"
#include "dev-usb.h"
#include "dev-wmac.h"
#include "machtypes.h"
#include "linux/i2c-gpio.h"
#include "linux/platform_device.h"

#define WEIO_GPIO_LED_STA		1
#define WEIO_GPIO_LED_AP		16
//#define WEIO_GPIO_LED_ETH0	13

#define WEIO_GPIO_BTN_AP	    20
#define WEIO_GPIO_BTN_RESET		23

#define WEIO_KEYS_POLL_INTERVAL		20	/* msecs */
#define WEIO_KEYS_DEBOUNCE_INTERVAL	(3 * WEIO_KEYS_POLL_INTERVAL)

#define WEIO_MAC0_OFFSET			0x0000
#define WEIO_MAC1_OFFSET			0x0006
#define WEIO_CALDATA_OFFSET		    0x1000
#define WEIO_WMAC_MAC_OFFSET		0x1002

static struct gpio_led weio_leds_gpio[] __initdata = {
	{
		.name		= "weio:green:sta",
		.gpio		= WEIO_GPIO_LED_STA,
		.active_low	= 1,
        .default_state = LEDS_GPIO_DEFSTATE_ON,
	}, {
		.name		= "weio:green:ap",
		.gpio		= WEIO_GPIO_LED_AP,
		.active_low	= 1,
        .default_state = LEDS_GPIO_DEFSTATE_ON,
	}
};

static struct gpio_keys_button weio_gpio_keys[] __initdata = {
	{
		.desc		= "ap button",
		.type		= EV_KEY,
		.code		= BTN_0,
		.debounce_interval = WEIO_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WEIO_GPIO_BTN_AP,
		.active_low	= 1,
	},
	{
		.desc		= "soft-reset button",
		.type		= EV_KEY,
		.code		= BTN_1,
		.debounce_interval = WEIO_KEYS_DEBOUNCE_INTERVAL,
		.gpio		= WEIO_GPIO_BTN_RESET,
		.active_low	= 1,
	}
};

static struct i2c_gpio_platform_data weio_i2c_gpio_data = {
	.sda_pin        = 18,
	.scl_pin        = 19,
};

static struct platform_device weio_i2c_gpio = {
	.name           = "i2c-gpio",
	.id             = 0,
	.dev     = {
		.platform_data  = &weio_i2c_gpio_data,
	},
};

static struct platform_device *weio_devices[] __initdata = {
        &weio_i2c_gpio
};

static void __init weio_common_setup(void)
{
	u8 *art = (u8 *) KSEG1ADDR(0x1fff0000);

	ath79_register_m25p80(NULL);
	ath79_register_wmac(art + WEIO_CALDATA_OFFSET,
			    art + WEIO_WMAC_MAC_OFFSET);

    /** WeIO does not use ETH - only WiFi by default */
#if 0
	ath79_setup_ar933x_phy4_switch(true, true);

	ath79_init_mac(ath79_eth0_data.mac_addr, art + WEIO_MAC0_OFFSET, 0);
	ath79_init_mac(ath79_eth1_data.mac_addr, art + WEIO_MAC1_OFFSET, 0);

	ath79_register_mdio(0, 0x0);

	/* LAN ports */
	ath79_register_eth(1);

	/* WAN port */
	ath79_register_eth(0);
#endif /** commented out ETH */
}

static void __init weio_setup(void)
{
	weio_common_setup();

    
	ath79_gpio_function_disable(AR933X_GPIO_FUNC_ETH_SWITCH_LED0_EN |
				AR933X_GPIO_FUNC_ETH_SWITCH_LED1_EN |
				AR933X_GPIO_FUNC_ETH_SWITCH_LED2_EN |
				AR933X_GPIO_FUNC_ETH_SWITCH_LED3_EN |
                AR933X_GPIO_FUNC_ETH_SWITCH_LED4_EN);

    platform_add_devices(weio_devices, ARRAY_SIZE(weio_devices));

	ath79_register_leds_gpio(-1, ARRAY_SIZE(weio_leds_gpio),
				 weio_leds_gpio);

	ath79_register_gpio_keys_polled(-1, WEIO_KEYS_POLL_INTERVAL,
					ARRAY_SIZE(weio_gpio_keys),
					weio_gpio_keys);
	ath79_register_usb();
}

MIPS_MACHINE(ATH79_MACH_WEIO, "WEIO", "WeIO board from Drasko DRASKOVIC and Uros PETREVSKI",
		weio_setup);
