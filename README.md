# Sunrise alarm clock   
### ESP8266 (ESP-01) based


## Configurations
The ESP8266 runs a web server which handles all of the web UI requests (UI in "index_html.cpp" file).

- Set up your ESP-01 network config (fixed IP) , SSID+pass and timezone.

- Using any device such as PC, tablet, phone connected to the local network (i.e. your home router)
open a web browser and put the following address:
**192.168.0.15**
(NOTE: this might differ between **your chosen network config** and mine).

- The user interface explains itself. Select which days and times the light will turn on. Set the max brightness and the decay time (the higher, the slower the lamp will turn on till reaching the desired brightness).

## Memory
All the configurations are stored in static memory, so you will not lose your preferences (alarm schedules, brightness, etc) on power off. 

## Time
The time is obtained from NTP servers at power on, and every 1 hour. If internet connection is lost, the time may suffer some deviation as MCU internal RTC (real time clock) is not very accurate.

## Hardware
 * files upload pending.
