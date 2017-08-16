#include <stdio.h>
#include <string.h>
#include <taihen.h>
#include <psp2kern/kernel/modulemgr.h>

void _start() __attribute__ ((weak, alias ("module_start")));
	
// handle to our hook
static tai_hook_ref_t hook_disable_led;
// this function is in kernel space
SceUID disable_led_other(int bus, int port) {
	if (bus == 0 && (port == 7 | port == 6))
	{
	return 0;	
	}
	//disable_led();
	return TAI_CONTINUE(SceUID, hook_disable_led, bus, port);
	}

static void disable_led()
{
	ksceGpioPortClear(0, 7);
	ksceGpioPortClear(0, 6);
}

int module_start(SceSize argc, const void *args)
{
	disable_led();
	  taiHookFunctionExportForKernel(KERNEL_PID,                  // Kernel process
                                 &hook_disable_led,           // Output a reference
                                 "SceLowio",              // Name of module being hooked
                                 TAI_ANY_LIBRARY,             // If there's multiple libs exporting this
                                 0xD454A584,                  // NID specifying `ksceGpioPortSet`
                                 disable_led_other); // Name of the hook function
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	return SCE_KERNEL_STOP_SUCCESS;
}
