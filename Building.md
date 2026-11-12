# Building apollo firmware

Clone the firmware and device repositories beside UniXNU:

```sh
git clone https://github.com/Pistachio-Project/uefi-apollo.git
git clone https://github.com/Pistachio-Project/UniXNU-apollo.git
cd UniXNU
./unixnu defconfig apollo_defconfig -D ../UniXNU-apollo
./unixnu build package firmware --uefi ../uefi-apollo
```

The shared resolver selects the SM8250 and OS profiles and component sources.
The device profile defines the firmware DSC, defines, patches, submodules and
output path. Host requirements and input preparation are documented in
[firmware integration](https://unixnu.com/porting/uefi-firmware).

The `Binaries` submodule is the Device-Binaries fork that carries `apollo/`.
The firmware build initializes the submodules the board lists; to fetch them
yourself, run `git submodule update --init --depth 1` in this checkout.

The firmware embeds `UniXNU`, `UniXNUDxe`, `DarwinBoot` and the shared
`UniXNURamDiskDxe`, with DiskIoDxe and FAT support, and the `AdvancedLogDxe`
log exporter. Keep each INF in the DSC and FDF. The RAM disk driver publishes
the bootloader's initrd so UniXNU can read `EFI/UniXNU/unixnu.toml` and list
the payload entries.

Provide the selected device's payload inputs and the firmware FDT before packaging:

```sh
./unixnu build payload --unique-chip-id <hex> --trust-cache <file> --data-ramdisk <seed>
./unixnu bootimg --uefi ../uefi-apollo --dtb ../uefi-apollo/Resources/DTBs/apollo.dtb
```

`Resources/DTBs/apollo.dtb` is the device tree of the phone's own Android
(`/sys/firmware/fdt`). The firmware volume is 0x300000 bytes at 0x9FC00000
(`Resources/Configs/apollo.toml`), XBL's UEFI window in the phone's
`uefiplatLA.cfg`.

## Standalone build

`build_uefi.py` still builds the platform outside UniXNU. `DeviceBuild.py`
looks for UniXNU's `package/` and DarwinBoot beside this checkout; set
`UNIXNU_ROOT` and `DARWINBOOT_ROOT` when they are elsewhere:

```sh
python3 -m venv venv && venv/bin/pip install -r pip-requirements.txt
source venv/bin/activate
UNIXNU_ROOT=../UniXNU DARWINBOOT_ROOT=../DarwinBoot python3 build_uefi.py -d apollo -r DEBUG
```

The output is `Build/apolloPkg/<TARGET>_CLANGPDB/FV/SILICIUM_UEFI.fd`.
`build_uefi.py` applies the Mu_Basecore patches in `Resources/MuPatches`
except `Gic-Spurious.patch`, which only the UniXNU board configuration applies.

The kernel, ADT, and payload steps are documented in
[building](https://unixnu.com/building). Firmware verification results belong
with the selected device and configuration.
