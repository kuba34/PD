# Synerift

### CAN communication for Raspberry Pi

**Pinout reference:**
```
RPi Pin    RPi Label     CAN Module
02---------5V------------VCC
06---------GND-----------GND
19---------GPIO10--------MOSI (SI)
21---------GPIO9---------MISO (SO)
22---------GPIO25--------INT
23---------GPIO11--------SCK
24---------GPIO8---------CS
```

## RPi configuration

Edit `/boot/config.txt` and add the following lines, setting the oscillator parameter to that of the oscillator present on the module.
```
dtparam=spi=on
dtoverlay=mcp2515-can0-overlay,oscillator=8000000,interrupt=25 
dtoverlay=spi-bcm2835-overlay
```
Reboot.

## Bring Up the Interface

You can bring up the interface with:

```shell
sudo ip link set can0 up type can bitrate 50000
```

You'll need to select the proper bitrate for the network you're connecting to. Common bitrates are: `10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000`.

If you need to change a setting, first bring down the interface:
```shell
sudo ip link set can0 down
```
You can see a list of available settings by appending help to the command:
```shell
pi@raspberrypi:~ $ ip link set can0 up type can bitrate 50000 help
Usage: ip link set DEVICE type can
        [ bitrate BITRATE [ sample-point SAMPLE-POINT] ] |
        [ tq TQ prop-seg PROP_SEG phase-seg1 PHASE-SEG1
          phase-seg2 PHASE-SEG2 [ sjw SJW ] ]

        [ dbitrate BITRATE [ dsample-point SAMPLE-POINT] ] |
        [ dtq TQ dprop-seg PROP_SEG dphase-seg1 PHASE-SEG1
          dphase-seg2 PHASE-SEG2 [ dsjw SJW ] ]

        [ loopback { on | off } ]
        [ listen-only { on | off } ]
        [ triple-sampling { on | off } ]
        [ one-shot { on | off } ]
        [ berr-reporting { on | off } ]
        [ fd { on | off } ]

        [ restart-ms TIME-MS ]
        [ restart ]

        Where: BITRATE  := { 1..1000000 }
                  SAMPLE-POINT  := { 0.000..0.999 }
                  TQ            := { NUMBER }
                  PROP-SEG      := { 1..8 }
                  PHASE-SEG1    := { 1..8 }
                  PHASE-SEG2    := { 1..8 }
                  SJW           := { 1..4 }
                  RESTART-MS    := { 0 | NUMBER }
```
The loopback setting enables loopback mode on the MCP2515, which can be useful for verifying that the RPi is communicating with the CAN module without having to connect to an actual CAN bus. All messages transmitted in loopback mode will be echoed back over the interface.

Listen-only can be useful if you're interested in sniffing CAN traffic, and want to lessen the risk of messing something up by ensuring the device doesn't send any packets.
