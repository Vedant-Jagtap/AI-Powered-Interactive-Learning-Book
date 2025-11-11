#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <esp_now.h>
#include <esp_wifi_types.h> // <-- THIS IS THE FIRST FIX

// =====================================================
// ===         CONFIGURATION - CHANGE THESE          ===
// =====================================================
const char* ssid = "Redmi 10";
const char* password = "zprt5435";
const char* serverIP = "172.24.2.218";
const int serverPort = 5000;

#define BUTTON_PIN 13 // Button on GPIO 13

// --- NEW ---
// Paste the MAC Address of your ESP32 DEV MODULE here
uint8_t speakerMacAddress[] = {0xF4, 0x65, 0x0B, 0x55, 0xE5, 0x98};
// ---------------------------------------------------
// =====================================================

// Data structure to send predictions
typedef struct struct_message {
  char prediction[32];
} struct_message;

struct_message myPrediction;
esp_now_peer_info_t peerInfo;

// Debounce variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
int lastButtonState = HIGH;

//
// --- THIS IS THE SECOND FIX ---
// This is the new, correct function definition for your library
//
void OnDataSent(const wifi_tx_info_t *tx_info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
// --------------------------------

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Camera Config (AI-Thinker)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0; config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5; config.pin_d1 = 18; config.pin_d2 = 19; config.pin_d3 = 21;
  config.pin_d4 = 36; config.pin_d5 = 39; config.pin_d6 = 34; config.pin_d7 = 35;
  config.pin_xclk = 0; config.pin_pclk = 22; config.pin_vsync = 25; config.pin_href = 23;
  config.pin_sccb_sda = 26; config.pin_sccb_scl = 27; config.pin_pwdn = 32; config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;
  config.grab_mode = CAMERA_GRAB_LATEST;
  if (esp_camera_init(&config) != ESP_OK) { Serial.println("Camera init failed"); return; }
  Serial.println("Camera initialized.");

  // Connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register the send callback function
  // This will now match the function definition above
  esp_now_register_send_cb(OnDataSent);
  
  // Register the "Speaker" ESP32 as a peer
  memcpy(peerInfo.peer_addr, speakerMacAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("ESP-NOW peer added. Ready to send predictions.");
}

void captureAndPredict() {
  camera_fb_t *fb = NULL;
  fb = esp_camera_fb_get(); if (fb) { esp_camera_fb_return(fb); } // Flush buffer
  fb = esp_camera_fb_get(); // Get fresh image
  if (!fb) { Serial.println("Capture failed"); return; }

  WiFiClient client;
  if (!client.connect(serverIP, serverPort)) {
    Serial.println("Connection to server failed!");
    esp_camera_fb_return(fb); return;
  }
  
  // Send image to Flask server
  String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
  String head = "--" + boundary + "\r\n";
  head += "Content-Disposition: form-data; name=\"imageFile\"; filename=\"capture.jpg\"\r\n";
  head += "Content-Type: image/jpeg\r\n\r\n";
  String tail = "\r\n--" + boundary + "--\r\n";
  uint32_t totalLen = head.length() + fb->len + tail.length();
  client.println("POST /predict_esp HTTP/1.1");
  client.println("Host: " + String(serverIP));
  client.println("Content-Length: " + String(totalLen));
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println();
  client.print(head); client.write(fb->buf, fb->len); client.print(tail);
  esp_camera_fb_return(fb);
  Serial.println("Image sent. Waiting for prediction...");
  
  long startTime = millis();
  while (!client.available() && millis() - startTime < 30000) { delay(100); }
  if (!client.available()) { Serial.println("Timeout."); client.stop(); return; }

  bool foundBody = false;
  while (client.available()) { if (client.readStringUntil('\n') == "\r") { foundBody = true; break; } }

  if (foundBody) {
    String responseBody = client.readString();
    JsonDocument doc;
    deserializeJson(doc, responseBody);
    
    if (doc.containsKey("prediction")) {
      String prediction = doc["prediction"];
      Serial.println("\n--- PREDICTION ---");
      Serial.println(prediction);
      Serial.println("--------------------");

      // Send this prediction to the Speaker ESP32
      strcpy(myPrediction.prediction, prediction.c_str());
      esp_err_t result = esp_now_send(speakerMacAddress, (uint8_t *) &myPrediction, sizeof(myPrediction));
      if (result == ESP_OK) {
        Serial.println("Prediction sent to Speaker ESP32.");
      } else {
        Serial.println("Error sending prediction via ESP-NOW.");
      }

    } else if (doc.containsKey("error")) {
      Serial.print("Error from server: ");
      Serial.println(doc["error"].as<String>());
    }
  } else { Serial.println("Failed to read response."); }

  client.stop();
  Serial.println("\nReady.");
}

void loop() {
  int reading = digitalRead(BUTTON_PIN);
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != lastButtonState && reading == LOW) {
      Serial.println("Button pressed!");
      captureAndPredict();
    }
  }
  lastButtonState = reading;
}
