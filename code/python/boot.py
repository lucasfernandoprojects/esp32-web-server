# This file is executed on every boot (including wake-boot from deepsleep)

try:
    import usocket as socket
except:
    import socket
    
from machine import Pin, freq
from network import WLAN, STA_IF
from esp import osdebug
from gc import collect
from time import sleep
from dht import DHT11

def clean_board():
    osdebug(None)
    collect()
    return None

def connect_wifi(ssid, password, cpu_freq):
    freq(cpu_freq)  # Set CPU frequency to 80 MHz
    
    built_in_led = 2
    station = WLAN(STA_IF)
    station.active(True)
    
    # Set Wi-Fi power management
    station.config(pm=WLAN.PM_POWERSAVE)  # Enable Wi-Fi power-saving mode
    
    station.connect(ssid, password)
    
    while not station.isconnected():
        led = Pin(built_in_led, Pin.OUT)  # Define the LED
        led.value(0)  # Turn off the built-in blue LED
        sleep(0.2)
        led.value(1)  # Turn on the built-in blue LED
        sleep(0.2) 
        print('Connecting to wifi...')
        
    print(f'Successfully connected to {ssid}')
    print(station.ifconfig())
    return None

clean_board()
connect_wifi('NET_2GA2CE5E', 'B3A2CE5E', 80000000)
