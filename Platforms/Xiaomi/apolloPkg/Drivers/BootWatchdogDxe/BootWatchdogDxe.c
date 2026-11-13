/** @file
  Arms the SM8250 APSS watchdog when boot services exit.

  The only system this firmware starts is XNU through DarwinBoot, on a phone without a serial port. A kernel that
  stops before it can reset would leave the phone on a frozen screen until someone holds the power key. With the
  watchdog armed at ExitBootServices the SoC resets by itself BOOT_WATCHDOG_SECONDS later unless the kernel feeds
  the watchdog (XNU's boot log does, see UniXNU-apollo patches/xnu-12377/apollo-bootlog.patch, boot-arg bootlog-wdt).

  Registers: kona.dtsi "qcom,msm-watchdog" at 0x17c10000, written as Linux's qcom-wdt does; the counter runs at
  32765 Hz (HakiOS xnuloader/loader.c found the watchdog off when ABL starts this firmware).

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UefiBootServicesTableLib.h>

#define APSS_WDT_BASE         0x17C10000ULL
#define APSS_WDT_RST          0x04
#define APSS_WDT_EN           0x08
#define APSS_WDT_BARK_TIME    0x10
#define APSS_WDT_BITE_TIME    0x14
#define APSS_WDT_HZ           32765U
#define BOOT_WATCHDOG_SECONDS 30U

STATIC
VOID
EFIAPI
ArmBootWatchdog (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_EN, 0);
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_RST, 1);
  // The bark is an interrupt nobody takes here; it is set past the bite.
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_BARK_TIME, (BOOT_WATCHDOG_SECONDS + 5) * APSS_WDT_HZ);
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_BITE_TIME, BOOT_WATCHDOG_SECONDS * APSS_WDT_HZ);
  MmioWrite32 (APSS_WDT_BASE + APSS_WDT_EN, 1);
}

EFI_STATUS
EFIAPI
BootWatchdogEntry (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_EVENT  Event;

  return gBS->CreateEventEx (
                EVT_NOTIFY_SIGNAL,
                TPL_NOTIFY,
                ArmBootWatchdog,
                NULL,
                &gEfiEventExitBootServicesGuid,
                &Event
                );
}
