#include <WiFi.h>
#include <WebSocketsServer.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ===== OLED =====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_MOSI 23
#define OLED_CLK  18
#define OLED_DC   2
#define OLED_CS   5
#define OLED_RST  4

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK,
                         OLED_DC, OLED_RST, OLED_CS);

// ===== WIFI =====
const char* ssid = "ECG_MONITOR";
const char* password = "12345678";

// ===== WEBSOCKET =====
WebSocketsServer webSocket = WebSocketsServer(81);

// ===== ECG =====
int ecgPin = 34;
int threshold = 2000;

unsigned long lastBeat = 0;
int BPM = 0;
int signalValue = 0;
int prevSignal = 0;

void setup() {
  // ===== SERIAL (FIXED) =====
  Serial.begin(115200);
  delay(1000);   // 🔥 IMPORTANT
  Serial.println("System Starting...");

  // ===== WIFI FIRST =====
  WiFi.softAP(ssid, password);
  delay(500);

  Serial.print("ESP IP: ");
  Serial.println(WiFi.softAPIP());

  // ===== OLED INIT =====
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("OLED FAIL");
    while (1);
  }

  // Test display
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("READY");
  display.display();
  delay(2000);

  // ===== WEBSOCKET =====
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("System Ready");
}

void loop() {
  webSocket.loop();

  // ===== ECG READ =====
  signalValue = analogRead(ecgPin);

  // ===== BPM DETECTION =====
  if (signalValue > threshold && prevSignal <= threshold) {
    unsigned long currentTime = millis();

    if (currentTime - lastBeat > 400) {
      BPM = 60000 / (currentTime - lastBeat);
      lastBeat = currentTime;
    }
  }
  prevSignal = signalValue;

  // ===== SERIAL DEBUG =====
  Serial.print("Signal: ");
  Serial.print(signalValue);
  Serial.print(" | BPM: ");
  Serial.println(BPM);

  // ===== OLED DISPLAY =====
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("BPM:");
  display.print(BPM);

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Sig:");
  display.print(signalValue);

  display.display();

  // ===== SEND DATA =====
  String json = "{";
  json += "\"ecg\":" + String(signalValue) + ",";
  json += "\"bpm\":" + String(BPM);
  json += "}";

  webSocket.broadcastTXT(json);

  delay(50);
}

// ===== WEBSOCKET EVENT =====
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("Dashboard Connected");
  }
}
