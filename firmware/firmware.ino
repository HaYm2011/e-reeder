#include <Wire.h>
#include <Adafruit_VL53L0X.h>

#define I2C_SDA_PIN       8
#define I2C_SCL_PIN       9

#define SENSOR_LEFT_XSHUT  4
#define SENSOR_RIGHT_XSHUT 5

#define EPD_RX_PIN        18
#define EPD_TX_PIN        17
#define EPD_WAKE_PIN      16
#define EPD_RST_PIN       15

HardwareSerial EPD_Serial(1);

#define ADDR_SENSOR_LEFT   0x30
#define ADDR_SENSOR_RIGHT  0x31

Adafruit_VL53L0X sensorLeft  = Adafruit_VL53L0X();
Adafruit_VL53L0X sensorRight = Adafruit_VL53L0X();

#define FRAME_HEADER      0xA5
#define CMD_HANDSHAKE     0x00
#define CMD_SET_BAUD      0x01
#define CMD_READ_BAUD     0x02
#define CMD_SET_MEMORY    0x07
#define CMD_ENTER_STOP    0x08
#define CMD_UPDATE        0x0A
#define CMD_SET_FONT_EN   0x1E
#define CMD_DISP_STRING   0x30
#define CMD_CLEAR         0x2E

#define FONT_ASCII32      0x01
#define FONT_ASCII48      0x02
#define FONT_ASCII64      0x03

#define GESTURE_TRIGGER_MIN_MM  30
#define GESTURE_TRIGGER_MAX_MM  140
#define GESTURE_COOLDOWN_MS     1200

unsigned long lastPageTurnTime = 0;
int currentPage = 0;

const int TOTAL_PAGES = 4;
const char* bookPages[TOTAL_PAGES] = {
  "Project Gutenberg E-Book\n\nChapter 1\nThe cold silence of deep space was interrupted only by the steady hum of life support systems...",
  "Chapter 1 (Cont.)\n\nHe checked the navigation telemetry. The trajectory remained locked towards the distant orbital gate.",
  "Chapter 2\n\nThe sensors pinged a rhythmic anomaly 200 kilometers off the port bow. Something was tracking their velocity profile.",
  "End of Preview.\n\nSwipe left to return to previous pages or restart the demonstration."
};

void epdSendPacket(uint8_t cmd, const uint8_t *payload, uint16_t length) {
  uint16_t frameLen = 1 + 2 + 1 + length + 4 + 1;
  uint8_t buffer[frameLen];
  
  buffer[0] = FRAME_HEADER;
  buffer[1] = (frameLen >> 8) & 0xFF;
  buffer[2] = frameLen & 0xFF;
  buffer[3] = cmd;
  
  for (uint16_t i = 0; i < length; i++) {
    buffer[4 + i] = payload[i];
  }
  
  uint16_t tailIndex = 4 + length;
  buffer[tailIndex++] = 0xCC;
  buffer[tailIndex++] = 0x33;
  buffer[tailIndex++] = 0xC3;
  buffer[tailIndex++] = 0x3C;
  
  uint8_t parity = 0;
  for (uint16_t i = 0; i < tailIndex; i++) {
    parity ^= buffer[i];
  }
  buffer[tailIndex] = parity;
  
  EPD_Serial.write(buffer, frameLen);
  EPD_Serial.flush();
  delay(20);
}

void epdClearScreen() {
  epdSendPacket(CMD_CLEAR, NULL, 0);
}

void epdRefresh() {
  epdSendPacket(CMD_UPDATE, NULL, 0);
}

void epdSetEnFont(uint8_t font) {
  uint8_t data[1] = { font };
  epdSendPacket(CMD_SET_FONT_EN, data, 1);
}

void epdDisplayString(int16_t x, int16_t y, const char *str) {
  uint16_t strLen = strlen(str);
  uint16_t payloadLen = 4 + strLen + 1;
  uint8_t payload[payloadLen];
  
  payload[0] = (x >> 8) & 0xFF;
  payload[1] = x & 0xFF;
  payload[2] = (y >> 8) & 0xFF;
  payload[3] = y & 0xFF;
  
  memcpy(&payload[4], str, strLen);
  payload[4 + strLen] = 0x00;
  
  epdSendPacket(CMD_DISP_STRING, payload, payloadLen);
}

void renderCurrentPage() {
  Serial.printf("Rendering Page %d of %d...\n", currentPage + 1, TOTAL_PAGES);
  
  epdClearScreen();
  epdSetEnFont(FONT_ASCII32);
  
  char headerBuf[32];
  snprintf(headerBuf, sizeof(headerBuf), "--- OpenBook S3 | Page %d/%d ---", currentPage + 1, TOTAL_PAGES);
  epdDisplayString(40, 30, headerBuf);
  
  char tempBuffer[512];
  strncpy(tempBuffer, bookPages[currentPage], sizeof(tempBuffer));
  
  char *line = strtok(tempBuffer, "\n");
  int yOffset = 100;
  
  while (line != NULL && yOffset < 540) {
    epdDisplayString(40, yOffset, line);
    yOffset += 45;
    line = strtok(NULL, "\n");
  }
  
  epdRefresh();
}

void setupToFSensors() {
  pinMode(SENSOR_LEFT_XSHUT, OUTPUT);
  pinMode(SENSOR_RIGHT_XSHUT, OUTPUT);
  
  digitalWrite(SENSOR_LEFT_XSHUT, LOW);
  digitalWrite(SENSOR_RIGHT_XSHUT, LOW);
  delay(20);
  
  digitalWrite(SENSOR_LEFT_XSHUT, HIGH);
  delay(20);
  if (!sensorLeft.begin(ADDR_SENSOR_LEFT, false, &Wire)) {
    Serial.println("Error: Failed to init Left VL53L0X!");
  } else {
    Serial.println("Left VL53L0X active at 0x30");
  }
  
  digitalWrite(SENSOR_RIGHT_XSHUT, HIGH);
  delay(20);
  if (!sensorRight.begin(ADDR_SENSOR_RIGHT, false, &Wire)) {
    Serial.println("Error: Failed to init Right VL53L0X!");
  } else {
    Serial.println("Right VL53L0X active at 0x31");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP32-S3 E-Reader firmware...");

  pinMode(EPD_WAKE_PIN, OUTPUT);
  pinMode(EPD_RST_PIN, OUTPUT);
  digitalWrite(EPD_WAKE_PIN, HIGH);
  
  digitalWrite(EPD_RST_PIN, LOW);
  delay(50);
  digitalWrite(EPD_RST_PIN, HIGH);
  delay(200);

  EPD_Serial.begin(115200, SERIAL_8N1, EPD_RX_PIN, EPD_TX_PIN);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN, 400000);
  setupToFSensors();

  renderCurrentPage();
}

void loop() {
  VL53L0X_RangingMeasurementData_t measureLeft;
  VL53L0X_RangingMeasurementData_t measureRight;

  sensorLeft.rangingTest(&measureLeft, false);
  sensorRight.rangingTest(&measureRight, false);

  uint16_t distLeft = (measureLeft.RangeStatus != 4) ? measureLeft.RangeMilliMeter : 9999;
  uint16_t distRight = (measureRight.RangeStatus != 4) ? measureRight.RangeMilliMeter : 9999;

  unsigned long currentMillis = millis();

  if (currentMillis - lastPageTurnTime > GESTURE_COOLDOWN_MS) {
    if (distRight >= GESTURE_TRIGGER_MIN_MM && distRight <= GESTURE_TRIGGER_MAX_MM) {
      if (currentPage < TOTAL_PAGES - 1) {
        Serial.printf("Right gesture detected (%d mm). Next Page.\n", distRight);
        currentPage++;
        lastPageTurnTime = currentMillis;
        renderCurrentPage();
      }
    }
    else if (distLeft >= GESTURE_TRIGGER_MIN_MM && distLeft <= GESTURE_TRIGGER_MAX_MM) {
      if (currentPage > 0) {
        Serial.printf("Left gesture detected (%d mm). Previous Page.\n", distLeft);
        currentPage--;
        lastPageTurnTime = currentMillis;
        renderCurrentPage();
      }
    }
  }

  delay(60);
}
