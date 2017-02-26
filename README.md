# Synerift

## CAN communication for Raspberry Pi

Pinout reference:
```
Regulator PD with CAN module.
RPi Pin    RPi Label     CAN Module
02---------5V------------VCC
06---------GND-----------GND
19---------GPIO10--------MOSI (SI)
21---------GPIO9---------MISO (SO)
22---------GPIO25--------INT
23---------GPIO11--------SCK
24---------GPIO8---------CS
```

### RPi configuration

Edit `/boot/config.txt` and add the following lines, setting the oscillator parameter to that of the oscillator present on the module.
```
dtparam=spi=on
dtoverlay=mcp2515-can0-overlay,oscillator=8000000,interrupt=25 
dtoverlay=spi-bcm2835-overlay
```
Reboot.

## Compiling Can-utils

You may need to install some prerequisites:
```shell
sudo apt-get install git autoconf libtool
```

Run the following commands to compile can-utils:
```shell
git clone https://github.com/linux-can/can-utils.git
cd can-utils
./autogen.sh
./configure
make
sudo make install
```
