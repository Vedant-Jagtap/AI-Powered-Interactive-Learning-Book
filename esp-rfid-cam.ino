#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <esp_now.h>
#include "DFRobotDFPlayerMini.h"

// === PIN DEFINITIONS for ESP32 DEV MODULE ===
#define SS_PIN 5        // RFID SDA (SS)
#define RST_PIN 22      // RFID RST

// === WIFI CONFIGURATION (MUST MATCH YOUR CAMERA) ===
const char* ssid = "Redmi 10";
const char* password = "zprt5435";
// ===============================================

HardwareSerial mySerial(2);
DFRobotDFPlayerMini myDFPlayer;
MFRC522 mfrc522(SS_PIN, RST_PIN);

typedef struct struct_message {
  char prediction[32];
} struct_message;
struct_message incomingPrediction;

// Function to play sound based on prediction string
// Function to play sound based on a prediction string
void speakPrediction(String prediction) {
  Serial.print("Speaking prediction from CAM: ");
  Serial.println(prediction);
  prediction.trim();
  
  if (prediction.equalsIgnoreCase("A")) { myDFPlayer.play(3); }
  else if (prediction.equalsIgnoreCase("B")) { myDFPlayer.play(4); }
  else if (prediction.equalsIgnoreCase("C")) { myDFPlayer.play(5); }
  else if (prediction.equalsIgnoreCase("D")) { myDFPlayer.play(6); }
  else if (prediction.equalsIgnoreCase("E")) { myDFPlayer.play(7); }
  else if (prediction.equalsIgnoreCase("F")) { myDFPlayer.play(8); }
  else if (prediction.equalsIgnoreCase("G")) { myDFPlayer.play(9); }
  else if (prediction.equalsIgnoreCase("H")) { myDFPlayer.play(10); }
  else if (prediction.equalsIgnoreCase("I")) { myDFPlayer.play(11); }
  else if (prediction.equalsIgnoreCase("J")) { myDFPlayer.play(12); }
  else if (prediction.equalsIgnoreCase("K")) { myDFPlayer.play(13); }
  else if (prediction.equalsIgnoreCase("L")) { myDFPlayer.play(14); }
  else if (prediction.equalsIgnoreCase("M")) { myDFPlayer.play(15); }
  else if (prediction.equalsIgnoreCase("N")) { myDFPlayer.play(16); }
  else if (prediction.equalsIgnoreCase("O")) { myDFPlayer.play(17); }
  else if (prediction.equalsIgnoreCase("P")) { myDFPlayer.play(18); }
  else if (prediction.equalsIgnoreCase("Q")) { myDFPlayer.play(19); }
  else if (prediction.equalsIgnoreCase("R")) { myDFPlayer.play(20); }
  else if (prediction.equalsIgnoreCase("S")) { myDFPlayer.play(21); }
  else if (prediction.equalsIgnoreCase("T")) { myDFPlayer.play(22); }
  else if (prediction.equalsIgnoreCase("U")) { myDFPlayer.play(23); }
  else if (prediction.equalsIgnoreCase("V")) { myDFPlayer.play(24); }
  else if (prediction.equalsIgnoreCase("W")) { myDFPlayer.play(25); }
  else if (prediction.equalsIgnoreCase("X")) { myDFPlayer.play(26); }
  else if (prediction.equalsIgnoreCase("Y")) { myDFPlayer.play(27); }
  else if (prediction.equalsIgnoreCase("Z")) { myDFPlayer.play(28); }
  
  else if (prediction.equals("0")) { myDFPlayer.play(38); }
  else if (prediction.equals("1")) { myDFPlayer.play(29); }
  else if (prediction.equals("2")) { myDFPlayer.play(30); }
  else if (prediction.equals("3")) { myDFPlayer.play(31); }
  else if (prediction.equals("4")) { myDFPlayer.play(32); }
  else if (prediction.equals("5")) { myDFPlayer.play(33); }
  else if (prediction.equals("6")) { myDFPlayer.play(34); }
  else if (prediction.equals("7")) { myDFPlayer.play(35); }
  else if (prediction.equals("8")) { myDFPlayer.play(36); }
  else if (prediction.equals("9")) { myDFPlayer.play(37); }
  
  else { Serial.println("Prediction not in sound map."); }
}

// Function to play sound based on RFID tag ID
void playRfidAudio(String tagID) {
  Serial.print("Playing audio for RFID tag: ");
  Serial.println(tagID);

  if (tagID == "1D0B6B000B1080") { myDFPlayer.play(1); }
  else if (tagID == "1D0C6B000B1080") { myDFPlayer.play(2); }
  else if (tagID == "1D0D6B000B1080") { myDFPlayer.play(3); }
  else if (tagID == "1D0E6B000B1080") { myDFPlayer.play(4); }
  else if (tagID == "1D0F6B000B1080") { myDFPlayer.play(5); }
  else if (tagID == "1D156B000B1080") { myDFPlayer.play(6); }
  else if (tagID == "1D166B000B1080") { myDFPlayer.play(7); }
  else if (tagID == "1D176B000B1080") { myDFPlayer.play(8); }
  else if (tagID == "1D186B000B1080") { myDFPlayer.play(9); }
  else if (tagID == "1D196B000B1080") { myDFPlayer.play(10); }
  else if (tagID == "1DA46B000B1080") { myDFPlayer.play(11); }
  else if (tagID == "1DA56B000B1080") { myDFPlayer.play(12); }
  else if (tagID == "1D096B000B1080") { myDFPlayer.play(13); }
  else if (tagID == "1D0A6B000B1080") { myDFPlayer.play(14); }
  else if (tagID == "1D996B000B1080") { myDFPlayer.play(15); }
  else if (tagID == "1D9A6B000B1080") { myDFPlayer.play(16); }
  else if (tagID == "1D9B6B000B1080") { myDFPlayer.play(17); }
  else if (tagID == "1D9C6B000B1080") { myDFPlayer.play(18); }
  else if (tagID == "1D9D6B000B1080") { myDFPlayer.play(19); }
  else if (tagID == "1D9E6B000B1080") { myDFPlayer.play(20); }
  else if (tagID == "1D9F6B000B1080") { myDFPlayer.play(21); }
  else if (tagID == "1DA06B000B1080") { myDFPlayer.play(22); }
  else if (tagID == "1DA16B000B1080") { myDFPlayer.play(23); }
  else if (tagID == "1DA26B000B1080") { myDFPlayer.play(24); }
  else if (tagID == "1D946B000B1080") { myDFPlayer.play(25); }
  else if (tagID == "1D956B000B1080") { myDFPlayer.play(26); }
  else if (tagID == "1D966B000B1080") { myDFPlayer.play(27); }
  else if (tagID == "1D976B000B1080") { myDFPlayer.play(28); }
  else if (tagID == "1D986B000B1080") { myDFPlayer.play(29); }
  else if (tagID == "1D106B000B1080") { myDFPlayer.play(30); }
  else if (tagID == "1D116B000B1080") { myDFPlayer.play(31); }
  else if (tagID == "1D126B000B1080") { myDFPlayer.play(32); }
  else if (tagID == "1D136B000B1080") { myDFPlayer.play(33); }
  else if (tagID == "1D146B000B1080") { myDFPlayer.play(34); }
  else if (tagID == "1D1F6B000B1080") { myDFPlayer.play(35); }
  else if (tagID == "1D206B000B1080") { myDFPlayer.play(36); }
  else if (tagID == "1D216B000B1080") { myDFPlayer.play(37); }
  else if (tagID == "1D226B000B1080") { myDFPlayer.play(38); }
  else if (tagID == "1D1A6B000B1080") { myDFPlayer.play(39); }
  else if (tagID == "1D236B000B1080") { myDFPlayer.play(40); }
  else { Serial.println("Tag not recognized"); }

  Serial.print("Now playing track: ");
  Serial.println(myDFPlayer.readCurrentFileNumber());
}

// Callback function when ESP-NOW data is received
void OnDataRecv(const esp_now_recv_info * info, const uint8_t *incomingData, int len) {
  memcpy(&incomingPrediction, incomingData, sizeof(incomingPrediction));
  Serial.println("Message received from CAM.");
  speakPrediction(incomingPrediction.prediction);
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  mySerial.begin(9600, SERIAL_8N1, 16, 17); // RX, TX
  Serial.println("Initializing DFPlayer...");
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer not found.");
    while (true);
  }
  Serial.println("DFPlayer online.");
  myDFPlayer.volume(29);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi to sync channel...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected! Channel is synced.");

  Serial.println("ESP32 Dev Module Ready.");
  Serial.print("My MAC Address is: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Ready to scan RFID or receive prediction...");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String tagID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) tagID += "0";
      tagID += String(mfrc522.uid.uidByte[i], HEX);
    }
    tagID.toUpperCase();
    playRfidAudio(tagID);
    mfrc522.PICC_HaltA();
    delay(1000);
  }
}
