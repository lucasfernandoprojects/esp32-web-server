// ========================================================================================
// Read values from a DHT11 sensor and display them on a web page
// This server works asynchronously
// Created on 17 October 2024
// Created by Lucas Fernando (https://www.youtube.com/@lucasfernandochannel)
// You are free to use this code the way you want
// ========================================================================================

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "ESP32_DHT11";
const char* password = "esp32project";

// DHT11
#define DHTPIN 25  // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Create an instance of the Async web server on port 80
AsyncWebServer server(80);

// IP Address configuration
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);  // Gateway is the same as the local IP in soft AP mode
IPAddress subnet(255, 255, 255, 0);  // Subnet mask

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read temperature");
    return "--";
  }
  return String(t, 1);  // One decimal place
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read humidity");
    return "--";
  }
  return String((int)h);  // No decimal places
}

float celsiusToFahrenheit(float celsius) {
  return (celsius * 9.0 / 5.0) + 32.0;
}

String createWebPage() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<title>ESP32 Web Server</title>";
  
  // Custom CSS
  html += "<style>";
  html += "body { background-color: #f8f9fa; display: flex; align-items: center; justify-content: center; height: 100vh; padding: 0 15px; margin: 0; font-family: Arial, sans-serif; }";
  html += "h1 { color: #343a40; }";
  html += ".container { text-align: center; }";
  html += ".temperature { color: #dc3545; font-size: 3rem; }";  // Bootstrap danger color
  html += ".humidity { color: #17a2b8; font-size: 3rem; }";  // Bootstrap info color
  html += "p { font-size: 1.25rem; color: #6c757d; }"; // Bootstrap muted color
  html += "</style>";
  
  // Add JavaScript for AJAX
  html += "<script>";
  html += "function updateSensorData() {";
  html += "  fetch('/sensor').then(response => response.json()).then(data => {";
  html += "    document.getElementById('temperature').innerHTML = data.temperature + '°C | ' + data.fahrenheit + '°F';";
  html += "    document.getElementById('humidity').innerHTML = data.humidity + '%';";
  html += "  }).catch(error => console.log('Error fetching sensor data:', error));"; // Catch any fetch errors
  html += "}";
  html += "setInterval(updateSensorData, 5000);";  // Update every 5 seconds
  html += "</script>";
  
  html += "</head>";
  html += "<body onload='updateSensorData()'>";
  html += "<div class='container'>";
  html += "<h1>DHT11 Sensor Data</h1>";
  html += "<div class='my-4'>";
  html += "<h2 class='temperature' id='temperature'>Loading...</h2>"; // Placeholder while loading
  html += "<h2 class='humidity' id='humidity'>Loading...</h2>";
  html += "</div>";
  html += "<p>This project code is available on <a href='https://github.com/lucasfernandoprojects/esp32-web-server/tree/main' target='_blank'>GitHub</a></p>";
  html += "</div>";
  html += "</body>";
  html += "</html>";

  return html;
}

void setup() {
  // Serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the DHT11 sensor
  dht.begin();
  Serial.println("DHT11 sensor initialized");

  // Set up the soft AP with specific IP configuration
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("AP Config Failed!");
  } else {
    Serial.println("AP Config Success");
  }

  // Setting ESP32 as Soft Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Define the route for the web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Serving web page...");
    request->send(200, "text/html", createWebPage());
  });

  // Define the route to get sensor data
  server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request){
    String temp = readDHTTemperature();
    String hum = readDHTHumidity();
    float celsius = temp.toFloat();
    float fahrenheit = celsiusToFahrenheit(celsius);
    
    String jsonResponse = "{\"temperature\": \"" + temp + "\", ";
    jsonResponse += "\"fahrenheit\": \"" + String(fahrenheit, 1) + "\", ";
    jsonResponse += "\"humidity\": \"" + hum + "\"}";
    
    Serial.println("Serving sensor data: " + jsonResponse);
    request->send(200, "application/json", jsonResponse);
  });

  // Start the server
  server.begin();
  Serial.println("Async HTTP server started");
}

void loop() {
  // No need to handle client manually in asynchronous mode
}
