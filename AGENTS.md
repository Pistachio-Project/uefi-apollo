# Agent instructions

## Responsibility

Maintain apollo UEFI integration, memory reservations, platform initialization,
firmware packaging, and hardware handoff.

## Required reading

1. Read README.md, Building.md, and Binaries/apollo/README.md.
2. Read [firmware integration](https://unixnu.com/porting/uefi-firmware),
   [log capture](https://unixnu.com/porting/log-capture#firmware-setup), and
   the [device repository](https://github.com/Pistachio-Project/UniXNU-apollo).
3. Read [porting](https://unixnu.com/porting/), the
   [repository specification](https://unixnu.com/porting/device-repository),
   [component catalog](https://unixnu.com/reference/components), and
   [device-tree specification](https://unixnu.com/porting/device-tree).
4. Read the selected component README, build interface, and implementation.
5. Read [payload](https://unixnu.com/concepts/payload),
   [building](https://unixnu.com/building), and
   [bring-up](https://unixnu.com/porting/bring-up) before image assembly.
6. Read the [migration guide](https://unixnu.com/porting/migration) for migration.

## Implementation

Inspect existing SoC support and catalog components before adding code.
apollo shares KonaPkg and the XBL firmware build with alioth; compare
`Platforms/Xiaomi/aliothPkg` before changing the platform.
Keep common SM8250 operations in shared support and apollo peripheral policy
in this repository or UniXNU-apollo, according to the execution stage.
Preserve protocol GUIDs, runtime identifiers, and firmware filenames.
Vendor binaries belong in the Device-Binaries fork (`Binaries/apollo`), not in
this repository; record the new submodule commit after changing them.
Maintain the documentation site in UniXNU and link its relevant pages here.

## Verification

Build with the selected UniXNU device and OS profiles using Building.md.
Run existing firmware checks. Report build commands, revisions, required
inputs, and results. Record hardware boot and peripheral observations with
the device and firmware configuration.

## Device porting record

The firmware section of apollo's porting record records handoff inputs,
development findings, evidence and completion criteria. Update
`UniXNU-apollo/porting.toml` when firmware work changes those inputs or
results. Maintainers and agents use the [same workflow](https://unixnu.com/porting/records).
