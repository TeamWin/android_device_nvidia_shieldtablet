#include <unistd.h>
#include <log.h>
#include <trampoline/devices.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mount.h>
#include <pthread.h>
#include <fcntl.h>
#include <util.h>

#if MR_DEVICE_HOOKS >= 1
#define PID_TOUCH    "/touch.pid"
#define PID_WATCHDOG "/watchdog.pid"

static void write_pid(const char* path, int pid) {
	FILE* f = fopen(path, "w");
	int num = fprintf(f, "%d\n", pid);
	fclose(f);
}

static void kill_pid(const char* path) {
	int pid;
	FILE *f	= fopen(path, "r");
	if (f == NULL)
		return;
	fscanf(f, "%d", &pid);
	fclose(f);
	kill(pid, SIGKILL);
	unlink(path);
}

void mrom_hook_before_fb_close() {
	// Kill the extra threads
	kill_pid(PID_TOUCH);
	kill_pid(PID_WATCHDOG);
}

int mrom_hook_after_android_mounts(__attribute__((unused))const char *busybox_path, __attribute__((unused))const char *base_path, __attribute__((unused))int type) {
	mrom_hook_before_fb_close();
	return 0;
}
#endif

#if MR_DEVICE_HOOKS >= 3
// If gk20a doesn't get firmware, boot to secondary is slow and the internal fails to init graphics.
// The symlink is needed for an automatic followup firmware load.
// There has got to be a simpler way to do this.
static void load_firmware() {
	int fd_loading = -1, c = 0;
	FILE *fd_firm = NULL, *fd_data = NULL;

	// Mount internal /system to /sysinit. Mounting to /system causes problems
	mkdir("/sysint", 0755);
	wait_for_file("/dev/block/platform/sdhci-tegra.3/by-name/APP", 10);
	mount("/dev/block/platform/sdhci-tegra.3/by-name/APP", "/sysint", "ext4", MS_RDONLY, NULL);
	symlink("/sysint/etc", "/etc");

	// Open the device for firmware loading
	wait_for_file("/sys/class/firmware/gk20a!gpmu_ucode.bin/loading", 10);
	fd_loading = open("/sys/class/firmware/gk20a!gpmu_ucode.bin/loading", O_WRONLY);
	write(fd_loading, "1", 1);

	// Copy the firmware into the device
	wait_for_file("/sysint/etc/firmware/gk20a/gpmu_ucode.bin", 10);
	fd_firm = fopen("/sysint/etc/firmware/gk20a/gpmu_ucode.bin", "rb");
	fd_data = fopen("/sys/class/firmware/gk20a!gpmu_ucode.bin/data", "wb");
	while((c=getc(fd_firm))!=EOF)
		fprintf(fd_data,"%c",c);
	fclose(fd_firm);
	fclose(fd_data);

	// Close the device firmware loader
	write(fd_loading, "0", 1);
	close(fd_loading);

	// Here, gk20a loads another firmware blob from /etc/firmware
	// Arbitrarily wait a second for it to finish loading and unmount everything
	sleep(1);
	unlink("/etc");
	umount("/sysint");
	unlink("/sysint");
	ERROR("Finished gk20a firmware load.\n");
}

// Touch thread. Touchscreen driver wrapper needs run separately.
static void ts_thread() {
	wait_for_file("/dev/touch", 10);
	wait_for_file("/realdata/media/0/multirom", 10);
	symlink("/realdata/media/0/multirom/touchscreen", "/sbin/touchscreen");

	ERROR("Starting rm-wrapper...\n");
	char* argv[] = {"/sbin/touchscreen/rm-wrapper", NULL};
	char* envp[] = {"LD_LIBRARY_PATH=/sbin/touchscreen", NULL};
	write_pid(PID_TOUCH, getpid());
	execve(argv[0], &argv[0], envp);
}

// Watchdog thread. If this isn't running, device will reset after a period of time.
static void wd_thread() {
	int fd = -1;

	while ((fd = open("/dev/watchdog", O_RDWR)) == -1)
		sleep(1);

	ERROR("Opened /dev/watchdog, preventing auto-reboot.\n");

	while (1) {
		write(fd, "", 1);
		sleep(5);
	}
}

void tramp_hook_before_device_init() {
	FILE *fd_cmdline = NULL;
	char *line = NULL;
	size_t len = 0, read = 0;
	int second_boot = 0;

	// If second boot, skip.
	wait_for_file("/proc/cmdline", 10);
	fd_cmdline = fopen("/proc/cmdline", "r");
	if (fd_cmdline >= 0) {
	  while ((read = getline(&line, &len, fd_cmdline)) != -1) {
	    if (strstr(line, "mrom_kexecd=1")) {
	      second_boot++;
	      break;
	    }
	  }
	  fclose(fd_cmdline);
	  if (line)
	    free(line);
	  if (second_boot)
	    return;
	}

	signal(SIGCHLD, SIG_IGN);

	// Spawn touch thread
	if (fork() == 0) {
		ts_thread();
		_exit(0);
	}

	// Load firmware thread
	if (fork() == 0) {
		load_firmware();
		_exit(0);
	}

	// Spawn watchdog thread
	if (fork() == 0) {
		write_pid(PID_WATCHDOG, getpid());
		wd_thread();
		_exit(0);
	}
}
#endif
