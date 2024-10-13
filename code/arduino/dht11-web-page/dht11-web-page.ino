#include <WiFi.h>
#include <DHT.h>
#include <WebServer.h>  // Built-in web server library for ESP32

#define DHTPIN 32  // Pin where the DHT11 is connected
#define DHTTYPE DHT11
#define LED_BUILTIN 2  // Built-in LED on ESP32

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);  // HTTP server on port 80

// Wi-Fi credentials
const char* ssid = "WIFI_NAME";
const char* password = "WIFI_PASSWORD";

// Variables for temperature and humidity
float temperature = 0;
float humidity = 0;

// Wi-Fi connection function
void connectWiFi() {
  WiFi.begin(ssid, password);
  
  // Set Wi-Fi power-saving mode
  WiFi.setSleep(true);

  // Blink the built-in LED while connecting
  pinMode(LED_BUILTIN, OUTPUT);
  
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
  digitalWrite(LED_BUILTIN, HIGH);  // Turn LED off once connected
}

// Function to read DHT11 data (with retry mechanism)
void readDHT11() {
  for (int i = 0; i < 3; i++) {  // Retry up to 3 times
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    
    if (!isnan(temperature) && !isnan(humidity)) {
      Serial.println("DHT11 data read successfully.");
      return;
    } else {
      Serial.println("DHT11 reading failed. Retrying...");
      delay(2000);
    }
  }
  // If failed after retries, set values to NaN
  temperature = NAN;
  humidity = NAN;
}

// Function to generate HTML for the web page with Bootstrap
String webPage() {
  String page;
  
  // Add Bootstrap CSS
  String bootstrap = "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">";

  if (isnan(temperature) || isnan(humidity)) {
    // Error page if sensor data is not available
    page = "<html><head><meta http-equiv='refresh' content='5'><meta charset='UTF-8'>" + bootstrap;
    page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    page += "<title>ESP32 Web Server</title></head>";
    page += "<body class='bg-danger text-white d-flex align-items-center justify-content-center' style='height: 100vh;'>";
    page += "<div class='text-center'>";
    page += "<h1 class='display-4'>Error: Sensor Data Unavailable</h1>";
    page += "<p class='lead'>Failed to read from DHT11 sensor. Retrying...</p>";
    page += "<p>The page will refresh in 5 seconds.</p></div></body></html>";
  } else {
    // Main page with sensor data
    page = "<html><head><meta http-equiv='refresh' content='5'><meta charset='UTF-8'>" + bootstrap;
    page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    page += "<title>ESP32 Web Server</title></head>";
    page += "<body class='bg-light d-flex align-items-center justify-content-center' style='height: 100vh;'>";
    page += "<div class='text-center'>";
    page += "<h1 class='display-4'>DHT11 Sensor Data</h1>";
    page += "<h2 class='text-primary' style='font-size: 3rem;'>" + String(temperature) + "°C</h2>";
    page += "<h2 class='text-info' style='font-size: 3rem;'>" + String(humidity) + "%</h2>";
    page += "<p class='lead'>Page refreshes every 5 seconds.</p></div></body></html>";
  }
  
  return page;
}

// HTTP handler for the root path
void handleRoot() {
  readDHT11();  // Update sensor data
  String response = webPage();  // Generate HTML page
  server.send(200, "text/html", response);  // Send response
}

// Main Arduino setup function
void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectWiFi();

  // Initialize DHT11 sensor
  dht.begin();

  // Set up the web server
  server.on("/", handleRoot);  // Handle root URL
  server.begin();  // Start the server
  Serial.println("Web server started.");
}

// Main Arduino loop function
void loop() {
  server.handleClient();  // Handle client requests
}
