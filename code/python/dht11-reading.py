# DHT11 Web Page with Footer
# You can find more information about this project on https://github.com/lucasfernandoprojects
# Created by Lucas Fernando - check my Youtube channel at https://www.youtube.com/@lucasfernandochannel

from time import sleep
from dht import DHT11
from machine import Pin
import socket

def read_dht11_sensor(sensor):
    # Retry mechanism to handle occasional timeouts
    for _ in range(3):  # Retry up to 3 times
        try:
            sensor.measure()
            temperature = sensor.temperature()
            humidity = sensor.humidity()
            return temperature, humidity
        except OSError as e:
            print("DHT11 reading failed, retrying...")
            sleep(2)  # Wait before retrying
    return None, None  # Return None if failed after retries


def web_page(temp, hum):
    if temp is None or hum is None:
        # Error page with a formatted message
        page = """
            <!DOCTYPE html>
            <html>
                <head>
                    <meta charset='UTF-8'>
                    <meta http-equiv='refresh' content='5'>
                    <meta name='viewport' content='width=device-width, initial-scale=1'>
                    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
                    <title>ESP32 Web Server</title>
                </head>
                <body class="bg-danger text-white d-flex align-items-center justify-content-center" style="height: 100vh; padding-left: 15px; padding-right: 15px;">
                    <div class="text-center">
                        <h1 class="display-4">Error: Sensor Data Unavailable</h1>
                        <p class="lead">Failed to read from DHT11 sensor. Retrying...</p>
                        <p>The page will refresh in 5 seconds.</p>
                    </div>
                </body>
            </html>
            """
    else:
        # Main page with sensor data displayed in the center with nice formatting
        page = f"""
            <!DOCTYPE html>
            <html>
                <head>
                    <meta charset='UTF-8'>
                    <meta http-equiv='refresh' content='5'>
                    <meta name='viewport' content='width=device-width, initial-scale=1'>
                    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC" crossorigin="anonymous">
                    <title>ESP32 Web Server</title>
                </head>
                <body class="bg-light d-flex align-items-center justify-content-center" style="height: 100vh; padding-left: 15px; padding-right: 15px;">
                    <div class="text-center">
                        <h1 class="display-4">DHT11 Sensor Data</h1>
                        <div class="my-4">
                            <h2 class="text-primary" style="font-size: 3rem;">
                                <i class="bi bi-thermometer-half"></i> {temp}°C
                            </h2>
                            <h2 class="text-info" style="font-size: 3rem;">
                                <i class="bi bi-droplet-half"></i> {hum}%
                            </h2>
                        </div>
                        <p class="lead">Page refreshes every 5 seconds.</p>
                    </div>
                </body>
            </html>
            """
    return page


sensor = DHT11(Pin(32))
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(('', 80))
server.listen(5)

while True:
    conn, addr = server.accept()
    print(f"The server got a connection from {str(addr)}")
    request = conn.recv(1024)
    
    temp, hum = read_dht11_sensor(sensor)  # Handle potential timeouts
    response = web_page(temp, hum)
    sleep(2)  # Delay to respect the DHT11 timing
    
    # Send HTTP response
    conn.send("HTTP/1.1 200 OK\r\n")
    conn.send("Content-Type: text/html\r\n")
    conn.send("Connection: close\r\n\r\n")
    conn.sendall(response)
    conn.close()
