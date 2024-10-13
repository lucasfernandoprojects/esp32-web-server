#include <WiFi.h>
#include <WebServer.h>

#define LED_BUILTIN 2  // Built-in LED on ESP32

// Pin definitions for RGB LED
#define RED_PIN 18
#define GREEN_PIN 19
#define BLUE_PIN 21

WebServer server(80);  // HTTP server on port 80

// Wi-Fi credentials
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

// Variables for LED states
bool redState = false;
bool greenState = false;
bool blueState = false;

// Function to connect to Wi-Fi
void connectWiFi() {
  WiFi.begin(ssid, password);
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Blink the built-in LED while connecting
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);  // LED off
    delay(200);
    digitalWrite(LED_BUILTIN, HIGH); // LED on
    delay(200);
    Serial.println("Connecting to Wi-Fi...");
  }
  
  // Wi-Fi connected
  Serial.println("Wi-Fi connected.");
  Serial.println(WiFi.localIP()); // Print the IP address
}

// Function to generate HTML for the web page
String webPage() {
  String page = "<!DOCTYPE html><html lang='en'>";
  page += "<head><meta charset='utf-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  page += "<title>RGB Led Controller</title>";
  page += "<link href='https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/css/bootstrap.min.css' rel='stylesheet'>";
  page += "<style>body, html { height: 100%; margin: 0; padding: 0; background-color: #f0f0f0; display: flex; flex-direction: column; }";
  page += "h1, h2 { text-align: center; margin: 20px 0; } .card { border-radius: 12px; margin-bottom: 20px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1); }";
  page += ".btn { width: 100%; padding: 15px; font-size: 1.2rem; } main { padding-left: 15px; padding-right: 15px; }";
  page += "footer { margin-top: auto; background-color: #343a40; color: white; padding: 20px; text-align: center; }";
  page += ".footer-link { color: #f1c40f; text-decoration: none; border: 2px solid #f1c40f; padding: 5px 10px; border-radius: 5px; }";
  page += ".footer-link:hover { text-decoration: underline; background-color: #f1c40f; color: black; }</style></head>";
  page += "<body><main class='container mt-5'><div class='row'><h1>RGB Led Controller</h1><h2>Control your RGB LED remotely</h2></div>";
  page += "<div class='row justify-content-center'><div class='card col-12 col-md-3 mx-3'><div class='card-body text-center'>";
  page += "<h5 class='card-title text-danger'>Red Light</h5><p class='card-text'>Turn the red light on/off.</p>";
  page += "<a href='/?red' class='btn btn-danger'>" + String(redState ? "ON" : "OFF") + "</a></div></div>";
  page += "<div class='card col-12 col-md-3 mx-3'><div class='card-body text-center'>";
  page += "<h5 class='card-title text-success'>Green Light</h5><p class='card-text'>Turn the green light on/off.</p>";
  page += "<a href='/?green' class='btn btn-success'>" + String(greenState ? "ON" : "OFF") + "</a></div></div>";
  page += "<div class='card col-12 col-md-3 mx-3'><div class='card-body text-center'>";
  page += "<h5 class='card-title text-primary'>Blue Light</h5><p class='card-text'>Turn the blue light on/off.</p>";
  page += "<a href='/?blue' class='btn btn-primary'>" + String(blueState ? "ON" : "OFF") + "</a></div></div></div>";
  page += "</main><footer><p>Created by <a href='https://www.youtube.com/@lucasfernandochannel' target='_blank' class='footer-link'>Lucas Fernando</a></p>";
  page += "<p>Check out the project on <a href='https://github.com/lucasfernandoprojects' target='_blank' class='footer-link'>GitHub</a></p></footer>";
  page += "<script src='https://cdn.jsdelivr.net/npm/bootstrap@5.2.3/dist/js/bootstrap.bundle.min.js'></script>";
  page += "</body></html>";
  return page;
}

// HTTP handler for the root path
void handleRoot() {
  String response = webPage();  // Generate HTML page
  server.send(200, "text/html", response);  // Send response
}

// HTTP handler for LED control
void handleLed() {
  if (server.hasArg("red")) {
    redState = !redState; // Toggle red state
    digitalWrite(RED_PIN, redState ? HIGH : LOW);
  } 
  if (server.hasArg("green")) {
    greenState = !greenState; // Toggle green state
    digitalWrite(GREEN_PIN, greenState ? HIGH : LOW);
  } 
  if (server.hasArg("blue")) {
    blueState = !blueState; // Toggle blue state
    digitalWrite(BLUE_PIN, blueState ? HIGH : LOW);
  }
  server.sendHeader("Location", "/");  // Redirect to main page
  server.send(302, "text/plain", "Redirecting...");
}

void setup() {
  Serial.begin(115200);
  
  // Set LED pins as output
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  // Initialize LED states
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
  
  // Connect to Wi-Fi
  connectWiFi();

  // Set up the web server
  server.on("/", handleRoot);  // Handle root URL
  server.on("/?red", handleLed);
  server.on("/?green", handleLed);
  server.on("/?blue", handleLed);
  server.begin();  // Start the server
  Serial.println("Web server started.");
}

void loop() {
  server.handleClient();  // Handle client requests
}
