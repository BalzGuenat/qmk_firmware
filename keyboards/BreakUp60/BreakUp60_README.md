# BreakUp60

A split 60% keyboard by Balz Guenat.

## Features and Specs

The Breakup60 is a QMK-powered 60% keyboard, broken up into two halves.
It can be used as a split keyboard with an interconnect cable or be pushed together and used like a normal keyboard without (!) an interconnect cable.
Oh, and it also has a rotary encoder.

- Crosslink with spring-loaded pins when halves are merged
- 3.5mm audio cable crosslink when split
- Hotswap switch sockets (Kailh)
- QMK Firmware
- Layout Support: ISO, split backspace, stepped caps lock, small right shift
- Rotary encoder support (with small right shift)
- Low-profile, 3D-printed case
- USB-C connector

**Unplug your keyboard from USB before (dis-)connecting the 3.5mm interconnect cable!**
The same precaution is not necessary when pushing the halves together.

Note: The Breakup60 is ***not*** compatible with standard 60% cases or PCBs.

## Firmware and Keymap

### Special Features

#### `together()`

Returns `true` if the two halves of the keyboard are together and `false` if they are apart.
This is done by reading the `SENS` pin on the left half, which is pulled xxxx when the right half is connected via the pogo connector.

#### `apart()`

The opposite of `together()`.

#### `onSplit()`

This method is called when a split is detected, i.e. when the halves are pulled apart.
More precisely, it is called when the `SENS` pin transitions from xxx to xxx.

#### `onMerge()`

This method is called when a merge is detected, i.e. when the halves are pushed together.
More precisely, it is called when the `SENS` pin transitions from xxx to xxx.

#### Rotary Encoder

To program the turning action of the encoder, override `void encoder_update_user(uint8_t, bool)` in your `keymap.c`.
See the [QMK docs](https://docs.qmk.fm/#/feature_encoders?id=callbacks) for the details.

The push action of the encoder works exactly as if a normal switch was installed in its place.
It can be programmed in the normal keymap.

### Flashing

**Warning! It is critical to disconnect the halves before flashing!** Not doing so might corrupt the bootloader.

Fuses:
```bash
# write
avrdude -p atmega32u4 -P /dev/ttyACM0 -c avrisp -u -U lfuse:w:0x5E:m -U hfuse:w:0xD9:m -U efuse:w:0xC3:m
# read
avrdude -p atmega32u4 -P /dev/ttyACM0 -c avrisp -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
# Fuses OK (E:F3, H:99, L:5E)
```

Bootloader:
```bash
avrdude -p atmega32u4 -P /dev/ttyACM0 -c avrisp -U flash:w:util/bootloader_atmega32u4_1.0.0.hex:i
```

Firmware:
```bash
# via DFU bootloader
make BreakUp60:flash
# via ISP (for left side)
make BreakUp60
avrdude -p atmega32u4 -P /dev/ttyACM0 -c avrisp -U flash:w:BreakUp60_default.hex:i
```

After the initial flash of the bootloader and firmware (see Assembly), you only need to flash the left half of the keyboard to update your keymap, macros, etc..
This is done over USB, the same way as with any other QMK keyboard.

## Assembly

1. Solder any remaining components to the PCB.
2. Use an SPI to flash the bootloader and QMK firmware. After the first time, the keyboard can be flashed without a SPI via USB.
3. Install stabilizers.
4. Glue the pogo connectors into the case. The pins go on the left half where they are better protected. The contact pads on the right.
5. Solder the wires for the crosslink connectors to the PCB.
6. Place the PCB into the case and screw it in place.
7. Solder the crosslink wires to the connectors.

## Hardware Description

Each half has its own Atmel ATMega32U4 controller.
In general, the matrices / pin assignments between the halves are very different but some key pins have the same assignment.
The pins for SPI programming, USB and crosslink are broken out to through-hole pads.

### Handedness Detection

Pin `B7` is used to detect handedness.
On the left half, the pin is pulled high; on the right half, it is pulled low.

The `UVBUS` pin is only connected on the left MC and can be used as an alternative to detect handedness.

Another alternative to detect handedness is USB-detect.

### Crosslink

The halves can be linked either with a 3.5mm TRRS audio cable or, when the halves are pushed together, with the pogo connector.
The pinouts are as follows.

**TRRS Jack**

|Plug|MC Pin|
|---|---|
|T|`XXX`|
|R|`XXX`|
|R|`XXX`|
|S|`XXX`|

**Pogo Connector**

Pins are counted from the top, i.e. from the number row downwards.

|Con. Pin|MC Pin|
|---|---|
|1|`XXX`|
|2|`XXX`|
|3|`XXX`|
|4|`XXX`|
|5|n/c|
|6|`SENS`|

The `SENS` pin on the pogo connector is used to detect whether the halves are together or apart.
It is weakly pulled high by the left half and strongly pulled low by the right.
When the left half reads a low value on the pin, it means the halves are pushed together.
See the functions `together()` and `apart()` on how to use this feature.

## Possible Improvements, Enhancements, Next Steps

### Pogo Connector

Instead of connecting the pogo connector by soldering wires to the PCB, a small plug connector and header could be used.

### Debounce Sense Pin

This should improve the experience of switching modes.

### Improve Case

Instead of a 3D-printed case, I could see a case made fully or partially with acrylic or even machined from metal.
Some changes to the contruction might be necessary (or become possible).

It might be worth taking some notes from the [UHK](https://ultimatehackingkeyboard.com/) in terms of construction (and palm rest/tenting).

### USB for Right Half

Not only would this make flashing the right half much easier, it would also make it standalone.
