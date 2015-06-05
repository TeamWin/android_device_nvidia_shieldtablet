#include <stdlib.h>

const char *mr_init_devices[] = {
	"/sys/class/graphics/fb0",

	// Internal SD
	"/sys/block/mmcblk0",
	"/sys/devices/platform/sdhci-tegra.3",
	"/sys/devices/platform/sdhci-tegra.3/mmc_host/mmc0",
	"/sys/devices/platform/sdhci-tegra.3/mmc_host/mmc0/mmc0:0001",
	"/sys/devices/platform/sdhci-tegra.3/mmc_host/mmc0/mmc0:0001/block/mmcblk0*",

	// External SD
	"/sys/block/mmcblk1",
	"/sys/devices/platform/sdhci-tegra.2",
	"/sys/devices/platform/sdhci-tegra.2/mmc_host/mmc1",
	"/sys/devices/platform/sdhci-tegra.2/mmc_host/mmc1/mmc1:1234",
	"/sys/devices/platform/sdhci-tegra.2/mmc_host/mmc1/mmc1:1234/block/mmcblk1*",

	// SD Block Devices
	"/sys/bus/mmc",
	"/sys/bus/mmc/drivers/mmcblk",
	"/sys/bus/sdio/drivers/bcmsdh_sdmmc",
	"/sys/module/mmc_core",
	"/sys/module/mmcblk",
	"/sys/module/sdhci",

	// Touchscreen
	"/sys/devices/virtual/input*",
	"/sys/devices/virtual/misc/uinput",
	"/sys/bus/spi",
	"/sys/bus/spi/drivers/rm_ts_spidev",
	"/sys/devices/platform/spi-tegra114.0*",
	"/sys/bus/platform/drivers/spi-tegra114",
	"/sys/devices/virtual/misc/touch",
	"/sys/bus/hid",
	"/sys/bus/hid/drivers/hid-multitouch",

	// Adb
	"/sys/class/tty/ptmx",
	"/sys/devices/virtual/android_usb/android0/f_adb",
	"/sys/bus/usb",

	// Android hardware buttons
	"/sys/devices/platform/gpio-keys.4",
	"/sys/devices/platform/gpio-keys.4*",
	"/sys/bus/platform/drivers/gpio-keys",

	// Watchdog, reboots after 2 minutes without this and code in mr_hooks
	"/sys/devices/platform/60005100.watchdog",
	"/sys/devices/virtual/misc/watchdog",
	"/sys/devices/virtual/watchdog*",

	NULL
};
