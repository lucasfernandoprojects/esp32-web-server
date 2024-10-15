#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "ESP32_DHT11";
const char* password = "esp32project";

// DHT11
#define DHTPIN 25  // Pin where the DHT11 is connected
#define DHTTYPE DHT11  // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// Create an instance of the web server on port 80
WebServer server(80);

// IP Address configuration
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);  // Gateway is the same as the local IP in soft AP mode
IPAddress subnet(255, 255, 255, 0);  // Subnet mask

String readDHTTemperature() {
  float t = dht.readTemperature();
  if (isnan(t)) {
    return "--";
  }
  return String(t, 1);  // One decimal place
}

String readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    return "--";
  }
  return String((int)h);  // No decimal places
}

float celsiusToFahrenheit(float celsius) {
  return (celsius * 9.0 / 5.0) + 32.0;
}

void handleRoot() {
  String temp = readDHTTemperature();
  String hum = readDHTHumidity();
  float celsius = temp.toFloat();
  float fahrenheit = celsiusToFahrenheit(celsius);

  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta http-equiv='refresh' content='5'>";
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
  
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>DHT11 Sensor Data</h1>";
  html += "<div class='my-4'>";
  html += "<h2 class='temperature'>" + temp + "°C | " + String(fahrenheit, 1) + "°F</h2>"; // One decimal for Celsius/Fahrenheit
  html += "<h2 class='humidity'>" + hum + "%</h2>";
  html += "</div>";
  html += "<p>Page refreshes every 5 seconds.</p>";
  html += "</div>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

void setup() {
  // Serial communication for debugging
  Serial.begin(115200);
  
  // Initialize the DHT11 sensor
  dht.begin();

  // Set up the soft AP with specific IP configuration
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("AP Config Failed!");
  }
  
  // Setting ESP32 as Soft Access Point
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // Define the route for the web page
  server.on("/", handleRoot);

  // Start server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}
