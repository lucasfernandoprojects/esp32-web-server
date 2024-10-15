#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "NET_2GA2CE5E";       // Replace with your Wi-Fi SSID
const char* password = "B3A2CE5E"; // Replace with your Wi-Fi password

// Pin definitions for RGB LED
#define RED_PIN 32
#define GREEN_PIN 33
#define BLUE_PIN 26

// Create an instance of the web server on port 80
WebServer server(80);

// Variables to store the state of the LEDs
bool redState = false;
bool greenState = false;
bool blueState = false;

// Function to handle the root URL
void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='en'>";
  html += "<head>";
  html += "<meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css' rel='stylesheet' integrity='sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC' crossorigin='anonymous'>";
  html += "<title>RGB LED Control</title>";
  html += "<script src='https://code.jquery.com/jquery-3.6.0.min.js'></script>";
  html += "<script>";
  html += "$(document).ready(function() {";
  html += "  $('.btn').click(function(e) {";
  html += "    e.preventDefault();"; // Prevent default button action
  html += "    var url = $(this).attr('href');"; // Get the URL from the button's href";
  html += "    $.get(url);"; // Send GET request without processing the response
  html += "  });";
  html += "});";
  html += "</script>";
  html += "<style>";
  html += "body { display: flex; align-items: center; justify-content: center; height: 100vh; background-color: #f8f9fa; }";
  html += ".container { text-align: center; }";
  html += ".btn { width: 150px; font-size: 1.5rem; margin: 10px; }";
  html += "</style>";
  html += "</head>";
  html += "<body>";
  html += "<div class='container'>";
  html += "<h1>RGB LED Control</h1>";
  html += "<button class='btn btn-danger' href='/red/on'>Red ON</button>";
  html += "<button class='btn btn-secondary' href='/red/off'>Red OFF</button>";
  html += "<button class='btn btn-success' href='/green/on'>Green ON</button>";
  html += "<button class='btn btn-secondary' href='/green/off'>Green OFF</button>";
  html += "<button class='btn btn-info' href='/blue/on'>Blue ON</button>";
  html += "<button class='btn btn-secondary' href='/blue/off'>Blue OFF</button>";
  html += "</div>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

// Function to update the RGB LED state
void updateLEDs() {
  digitalWrite(RED_PIN, redState ? HIGH : LOW);
  digitalWrite(GREEN_PIN, greenState ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blueState ? HIGH : LOW);
}

// Functions to handle color requests
void handleRedOn() {
  redState = true;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void handleRedOff() {
  redState = false;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void handleGreenOn() {
  greenState = true;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void handleGreenOff() {
  greenState = false;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void handleBlueOn() {
  blueState = true;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void handleBlueOff() {
  blueState = false;
  updateLEDs();
  server.send(200, "text/plain", ""); // Send empty response
}

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Set RGB pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Define the routes
  server.on("/", handleRoot);
  server.on("/red/on", handleRedOn);
  server.on("/red/off", handleRedOff);
  server.on("/green/on", handleGreenOn);
  server.on("/green/off", handleGreenOff);
  server.on("/blue/on", handleBlueOn);
  server.on("/blue/off", handleBlueOff);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
}
