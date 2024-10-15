import network

# Network credentials
ssid = "ESP32_DHT11"
password = "esp32project"

def connect_wifi():
    # Initialize the Access Point (AP) mode
    ap = network.WLAN(network.AP_IF)
    ap.active(True)
    ap.ifconfig(('192.168.1.1', '255.255.255.0', '192.168.1.1', '8.8.8.8'))
    ap.config(essid=ssid, password=password)
    
    while not ap.active():
        pass
    
    print('Network config:', ap.ifconfig())

# Call the function to connect to Wi-Fi when the ESP32 boots
connect_wifi()

