# Estigia

Estigia is a SmartWatch firmware for Lilygo TWatch 2020 (V3, change it on config.h).
It is in development state, but updates are coming fast.

# Apps
- KillAll: TV Kill (IR signal): Just enter the app and aim any TV, wait a few seconds and... Bye :))

# Future updates:
- Honey: EvilAP for phising purposes.

# ATTENTION:
For those having problems while flashing in the V3 watch: There is something extange happening with the default library configuration. The backlight and the IR Led are having conflict.  The V3 theTFT BackLight is in theory in pin 15 (while on the other watch versions it is setted to 12). Thats what is causing somehow mess. For solving the problem, Ive just edited the file Arduino\libraries\TTGO_TWatch_Library-master\src\board\twatch2020_v3.h, and modified the line

from

    #define TWATCH_TFT_BL (GPIO_NUM_15)

to

    #define TWATCH_TFT_BL (GPIO_NUM_12)
