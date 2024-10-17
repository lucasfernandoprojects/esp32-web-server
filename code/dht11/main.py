# ========================================================================================
# Read values from a DHT11 sensor and display them on a web page
# This server works asynchronously
# Created on 17 October 2024
# Created by Lucas Fernando (https://www.youtube.com/@lucasfernandochannel)
# You are free to use this code the way you want
# ========================================================================================

import network
import socket
import time
import dht
from machine import Pin

# DHT11 setup
dht_pin = Pin(25)
dht_sensor = dht.DHT11(dht_pin)

# HTML content for the web server
html = """<!DOCTYPE html>
<html>
<head>
<meta charset='UTF-8'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>ESP32 Web Server</title>
<style>
body {
    background-color: #f8f9fa;
    display: flex;
    align-items: center;
    justify-content: center;
    height: 100vh;
    padding: 0 15px;
    margin: 0;
    font-family: Arial, sans-serif;
}
h1 {
    color: #343a40;
}
.container {
    text-align: center;
}
.temperature {
    color: #dc3545;
    font-size: 3rem;
}
.humidity {
    color: #17a2b8;
    font-size: 3rem;
}
p {
    font-size: 1.25rem;
    color: #6c757d;
}
</style>
<script>
function updateData() {
  fetch('/sensor').then(response => response.json()).then(data => {
    document.getElementById('temp').innerHTML = data.temp_c + '°C | ' + data.temp_f + '°F';
    document.getElementById('hum').innerHTML = data.hum + '%';
  }).catch(error => console.log('Error fetching sensor data:', error));
}
setInterval(updateData, 5000);
</script>
</head>
<body onload="updateData()">
<div class="container">
<h1>DHT11 Sensor Data</h1>
<h2 class="temperature" id="temp">Loading...</h2>
<h2 class="humidity" id="hum">Loading...</h2>
<p>This project code is available on <a href='https://github.com/lucasfernandoprojects/esp32-web-server/tree/main' target='_blank'>GitHub</a></p>
</div>
</body>
</html>"""

def celsius_to_fahrenheit(celsius):
    return (celsius * 9.0 / 5.0) + 32.0

def read_sensor_data():
    try:
        dht_sensor.measure()
        temp_c = dht_sensor.temperature()
        temp_f = celsius_to_fahrenheit(temp_c)
        hum = dht_sensor.humidity()
        print(f"Temperature: {temp_c}°C, Humidity: {hum}%, Fahrenheit: {temp_f}°F")  # Debug info
        return {'temp_c': temp_c, 'temp_f': "{:.1f}".format(temp_f), 'hum': hum}
    except Exception as e:
        print("Error reading sensor data:", e)
        return {'temp_c': "--", 'temp_f': "--", 'hum': "--"}

def start_server():
    addr = socket.getaddrinfo('192.168.1.1', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(1)
    print('Server running on http://192.168.1.1')

    while True:
        cl, addr = s.accept()
        print('Client connected from', addr)
        request = cl.recv(1024)
        request_str = str(request)

        if '/sensor' in request_str:
            sensor_data = read_sensor_data()
            response = ('HTTP/1.1 200 OK\r\n' +
                        'Content-Type: application/json\r\n' +
                        'Connection: close\r\n\r\n' +
                        f'{{"temp_c": "{sensor_data["temp_c"]}", ' +
                        f'"temp_f": "{sensor_data["temp_f"]}", ' +
                        f'"hum": "{sensor_data["hum"]}"}}')
        else:
            response = ('HTTP/1.1 200 OK\r\n' +
                        'Content-Type: text/html\r\n' +
                        'Connection: close\r\n\r\n' + html)

        cl.sendall(response)
        cl.close()

# Main function
def main():
    # Configure Access Point
    ap = network.WLAN(network.AP_IF)
    ap.active(True)
    ap.ifconfig(('192.168.1.1', '255.255.255.0', '192.168.1.1', '192.168.1.1'))
    ap.config(essid='ESP32_DHT11', password='esp32project')

    while not ap.active():
        time.sleep(1)

    print('Access Point started with IP:', ap.ifconfig()[0])

    # Start the web server
    start_server()

# Execute main function
main()


