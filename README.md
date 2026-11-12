# uefi-apollo

UEFI platform integration for the Xiaomi Mi 10T (apollo, SM8250), using
Project Silicium and Project Mu.

The platform supplies memory reservations, board peripherals, display
handoff, and the firmware volume. UniXNU supplies the boot application and
DarwinBoot loader. UniXNU-sm8250-common supplies shared XNU platform support.

- [Firmware integration](https://unixnu.com/porting/uefi-firmware)
- [Device repository](https://github.com/Pistachio-Project/UniXNU-apollo)
- [Build instructions](Building.md)
- [Platform status](Status.md)

## Origin

The apollo platform port (`Platforms/Xiaomi/apolloPkg`,
`Resources/Configs/apollo.toml`, `Resources/DTBs/apollo.dtb` and the vendor
binaries) originates from HakiOS (`devices/mi-10t/uefi`). It was made on
Mu-Silicium `e51cd877` and is modelled on `aliothPkg`, which shares the
KonaPkg SoC package and the XBL firmware build.

HakiOS added its own XNU loader to Mu-Silicium's boot manager and had the boot
shim copy an "XNUP" container to 0xF0000000. That hook is not carried here.
UniXNU's boot manager and DarwinBoot replace it, and the payload arrives as the
boot image's ramdisk. The 0xF0000000 "XNU Payload" carve-out is therefore
gone from the memory map. The "Kernel" region at 0xA0000000 (256 MiB) stays
reserved. It is ABL's kernel window from `uefiplat.cfg`, where the boot
image's kernel, device tree and ramdisk are expected to land. This has not yet
been checked on the phone; `UNIXNURAMDISK:` in the log reports the initrd
address.

## Payload firmware

Build through `./unixnu build firmware --uefi ../uefi-apollo` from UniXNU.
The DSC and FDF include `UniXNURamDiskDxe`, DiskIoDxe and FAT support so the
boot-image payload appears as a filesystem. `UniXNURamDiskDxe` is a shared
module in UniXNU's `package/`, and uses the Mu-Silicium FDT handoff: SEC keeps
the device tree address ABL passes in x0 in the entry-argument HOB
(`gSiliciumEntryArgHobGuid`), as in uefi-sheng.
DarwinBoot is built into the firmware volume, and `AdvancedLogDxe` exports the
firmware log from UniXNU's Tools menu.
See [required firmware integration](https://unixnu.com/porting/uefi-firmware#required-integration).

## Vendor binaries

`Binaries` is the Device-Binaries submodule, as in upstream Mu-Silicium.
Upstream has no apollo folder, so the submodule points at
[Pistachio-Project/Device-Binaries](https://github.com/Pistachio-Project/Device-Binaries),
a fork whose recorded commit adds `apollo/` beside the other devices. The
binaries are not tracked in this repository because Mu-Silicium ignores files
copied into the submodule, and the fork keeps Device-Binaries' layout.

The `apollo/` drivers and raw files were extracted from the phone's own XBL,
firmware `BOOT.XF.3.2-00304-SM8250-2` (slot 4), with
[UEFIReader](https://github.com/WOA-Project/UEFIReader). Six drivers
(ButtonsDxe, UsbConfigDxe, TzDxeLA, FeatureEnablerDxe, UsbMsdDxe, DALSYSDxe)
carry alioth's byte patches for the same firmware build. Each patch was
applied as a delta after the original bytes were checked. `Binaries/apollo/README.md`
lists each patch with its reason and author.

## Credits

[Microsoft](https://github.com/microsoft/) for their [Project Mu](https://microsoft.github.io/mu/) which Powers this Project. <br>
[WOA-Project](https://github.com/WOA-Project/) for their [mu_andromeda_platforms](https://github.com/WOA-Project/mu_andromeda_platforms/) which is used as Base for this Project, and for [UEFIReader](https://github.com/WOA-Project/UEFIReader). <br>
[Project Silicium](https://github.com/Project-Silicium/) for [Mu-Silicium](https://github.com/Project-Silicium/Mu-Silicium), its alioth port and [Device-Binaries](https://github.com/Project-Silicium/Device-Binaries). <br>
[Project Aloha](https://github.com/Project-Aloha/) for their [mu_aloha_platforms](https://github.com/Project-Aloha/mu_aloha_platforms/) which is used as Reference for some SoCs. <br>
[Open Surface RT](https://openrt.gitbook.io/open-surfacert/) for their Help on Tegra Related Stuff.

[sonic011gamer](https://github.com/sonic011gamer/) and [halal-beef](https://github.com/halal-beef/) for their Amazing Work on Exynos Related Stuff.

## License

All code except drivers in `GPLDrivers` & `GPLApplications` & `GPLLibrary` directories are licensed under BSD-2-Clause. <br />
GPL Drivers are licensed under `GPLv2` or later license.

## Device porting record

The firmware section of apollo's porting record records handoff inputs,
development findings, evidence and completion criteria. Update
`UniXNU-apollo/porting.toml` when firmware work changes those inputs or
results. Maintainers and agents use the [same workflow](https://unixnu.com/porting/records).

## Local build inputs

Save device paths and boot arguments in `unixnu.local.toml` in the UniXNU
checkout. See the [configuration reference](https://unixnu.com/reference/configuration#local-build-inputs).
Then use `./unixnu kernel`, `./unixnu firmware`, `./unixnu payload`, and
`./unixnu bootimg`. Argument changes require rebuilding the payload and boot
image. See [boot arguments](https://unixnu.com/xnu/boot-arguments#apply-arguments)
and [green-bar diagnosis](https://unixnu.com/guide/debugging#green-bar).
