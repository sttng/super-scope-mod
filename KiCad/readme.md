# Arduino Nano Shield KiCad 9 files

An Arduino Nano shield version of the Super Scope Mod. It is supposed to go below an Arduino Nano Shield. 
The orientation is marked with the USB (top) marker. This is where the Arduino Nanos USB port is supposed to be (on top), with the shield below.

![PCB](https://raw.githubusercontent.com/sttng/super-scope-mod/refs/heads/main/KiCad/SimulateReceiver.png)

# Device

![Prototype-1](https://raw.githubusercontent.com/sttng/super-scope-mod/refs/heads/main/KiCad/Nano-Scope-Mod-03.jpg) 

![Prototype-2](https://raw.githubusercontent.com/sttng/super-scope-mod/refs/heads/main/KiCad/Nano-Scope-Mod-02.jpg) 

![Prototype-3](https://raw.githubusercontent.com/sttng/super-scope-mod/refs/heads/main/KiCad/Nano-Scope-Mod-01.jpg) 

## BoM

| Designator  | Footprint                                          | Quantity | Description   |
|-------------|----------------------------------------------------|----------|---------------|
| J1, J2      | PinSocket_1x15_P2.54mm_Vertical                    | 2        |               |
| J4          | Molex_PicoBlade_53048-0610_1x06_P1.25mm_Horizontal | 1        |               |
| C1,C2,C3,C3 | C_0805_2012Metric_Pad1.18x1.45mm_HandSolder        | 4        | 0.1µF         |
| R1          | R_0805_2012Metric_Pad1.20x1.40mm_HandSolder        | 1        | 680k          |
| IC1         | DIP-16_W7.62mm                                     | 1        | MC14021BCP    |
| U1          | JEITA_SOIC-8_3.9x4.9mm_P1.27mm                     | 1        | LM1881M       |



