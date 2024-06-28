# ESP32 Klipper Monitor

This project will eventually become a monitor for any printer (or printers) running Klipper. The goal interface is a simple ESP32 with a screen/interface similar to KlipperScreen's (but scaled down) that initially prompts to be connected to a computer like a USB drive, where the end user can configure settings in a config.json file, things like wifi info, ip of the target printer, Moonraker port, etc. After this initial step, the device will then be able to display all relevant information on its screen in some kind of graphical way.


## Hardware
The primary hardware used for this project will be an ESP32-based development board, eventually switched to a PCB. Also necessary is a ST7789/ST7735 LCD (hopefully multiple supported screens!) and power source, either a USB cable or charging LiPo. Wiring details to come, as I haven't yet connected mine. 
If you're trying to follow along exactly what I do, I'm using an ESP-WROOM-32 USB-C devkit (unbranded) and ST7789 240*280 LCD breakout with rounded corners.

## Planned features list / to do
In no particular order:
|Feature/element|Status|
|:---------------------------------|:----:|
|API connection|:x:|
|USB thumb drive capabilities|:x:|
|LVGL implementation basics|:x:|
|Multiple screens/dynamic UI|:x:|
|Full configuration through json|:x:|
|ePaper support|:x:|
|Web UI/config GUI|:x:|

## License / legal mumbo jumbo
In the off chance you want to replicate this project, everything is of course open source. As of writing, I have not placed an official license on anything here, so I will unofficially ask that if you redistribute/modify/use/sell any part of this code that you credit me.