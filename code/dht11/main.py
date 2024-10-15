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
<meta http-equiv='refresh' content='5'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>ESP32 Web Server</title>
<style>
body {{ background-color: #f8f9fa; display: flex; align-items: center; justify-content: center; height: 100vh; padding: 0 15px; margin: 0; font-family: Arial, sans-serif; }}
h1 {{ color: #343a40; }}
.container {{ text-align: center; }}
.temperature {{ color: #dc3545; font-size: 3rem; }}
.humidity {{ color: #17a2b8; font-size: 3rem; }}
p {{ font-size: 1.25rem; color: #6c757d; }}
</style>
</head>
<body>
<div class='container'>
<h1>DHT11 Sensor Data</h1>
<div class='my-4'>
<h2 class='temperature'>{temp_c}°C | {temp_f}°F</h2>
<h2 class='humidity'>{hum}%</h2>
</div>
<p>Page refreshes every 5 seconds.</p>
</div>
</body>
</html>"""

def celsius_to_fahrenheit(celsius):
    return (celsius * 9.0 / 5.0) + 32.0

def web_page():
    try:
        dht_sensor.measure()
        temp_c = dht_sensor.temperature()
        temp_f = celsius_to_fahrenheit(temp_c)
        hum = dht_sensor.humidity()
        return html.format(temp_c=temp_c, temp_f="{:.1f}".format(temp_f), hum=hum)
    except Exception as e:
        print("Error reading sensor data:", e)
        return html.format(temp_c="--", temp_f="--", hum="--")

def serve():
    addr = socket.getaddrinfo('192.168.1.1', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(5)
    print('Listening on', addr)
    
    while True:
        cl, addr = s.accept()
        print('Client connected from', addr)
        request = cl.recv(1024)
        print('Request:', request)
        response = web_page()
        cl.send('HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n')
        cl.sendall(response)
        cl.close()

# Call the function to start the web server
serve()
