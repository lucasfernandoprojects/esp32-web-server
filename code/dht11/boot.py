# ========================================================================================
# Read values from a DHT11 sensor and display them on a web page
# This code sets up the initial server parameters 
# Created on 17 October 2024
# Created by Lucas Fernando (https://www.youtube.com/@lucasfernandochannel)
# You are free to use this code the way you want
# ========================================================================================

import network

def create_access_point():
    # Create access point
    ap = network.WLAN(network.AP_IF)
    ap.active(True)
    
    # Configure IP and subnet
    ap.ifconfig(('192.168.1.1', '255.255.255.0', '192.168.1.1', '255.255.255.0'))
    
    # Set Wi-Fi credentials
    ap.config(essid="ESP32_DHT11", password="esp32project")
    
    while not ap.active():
        pass  # Wait for access point to be active
    
    print('Access Point started with IP:', ap.ifconfig()[0])

# Call the function to start the access point
create_access_point()


