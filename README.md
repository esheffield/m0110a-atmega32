# m0110a-atmega32

## Background
This repo contains the schematic, pcb, and code for a drop-in replacement module for the controller in the Macintosh Plus m0110a keyboard. This controller is often damage or destroyed thanks to Apple's bizarre decision to use a cable that that *looks* like a telephone handset cable and uses the same connectors but is *wired differently*. If a user tried to use a phone handset cable in place of the original, this would feed reversed polarity power to the controller and kill it.

This replacement is based on the ATMega32 microcontroller. This controller was chosen due to the high number of GPIO pins available. The keyboard uses most of these, with a 10 x 8 matrix, four additional lines for Shift, Caps Lock, Command, and Option keys, and two communication lines for a total of 24 required lines. To aid in debugging if necessary in the future, the dedicated serial TxD and RxD lines are reserved and broken out to an optional header on the module.

The module also provides a standard 6-pin ISP connection. Thanks to the layout of keyboard, it's possible to access the ISP and Serial headers by removing the spacebar.

## Limitations
At this time, this module will only work in the m0110a keyboard as included with the Macintosh Plus. I don't know if the 128 / 512 keyboard and numeric keypad use the same controller. I understand that the keypad in particular has to act as a intermediary between the keyboard and Mac. I expect this work could be adapted, but at the least would require updated code if not changes to the hardware itself.

## License

The design and code are licensed under Apache 2.0.

## Attribution and Thanks

The code for communication with the Macintosh is based on the work of Tomasz Rękawek (https://github.com/trekawek), specifically his project adapting a PS2 keyboard to a Mac Plus (https://github.com/trekawek/mac-plus-ps2). Many thanks to Tomasz for his valuable work!

