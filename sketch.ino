#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

// Temperature potentiometer
#define TEMP_PIN 34

// Airflow potentiometer
#define AIRFLOW_PIN 35

// OLED
#define SDA_PIN 21
#define SCL_PIN 22

// Status LEDs
#define GREEN_LED 25
#define YELLOW_LED 33
#define RED_LED 32

// Buzzer
#define BUZZER_PIN 23

// Servo - Furnace Damper
#define SERVO_PIN 18

// Start / Stop button
#define BUTTON_PIN 27

// =====================================================
// OLED
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// =====================================================
// SYSTEM VARIABLES
// =====================================================

bool furnaceON = false;

bool lastButtonState = HIGH;
unsigned long lastButtonTime = 0;

const unsigned long debounceDelay = 250;

// =====================================================
// SERVO SETTINGS
// =====================================================

const int servoFreq = 50;
const int servoResolution = 16;

// =====================================================
// SERVO CONTROL
// =====================================================

void setDamperAngle(int angle)
{
  int minDuty = 1638;   // approximately 0.5 ms
  int maxDuty = 8192;   // approximately 2.5 ms

  int duty = map(
    angle,
    0,
    180,
    minDuty,
    maxDuty
  );

  ledcWrite(SERVO_PIN, duty);
}

// =====================================================
// LED CONTROL
// =====================================================

void allLEDOff()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

// =====================================================
// OLED DISPLAY
// =====================================================

void showDisplay(
  int temperature,
  int airflow,
  int damper,
  String status
)
{
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println("INDUSTRIAL FURNACE");

  display.println("----------------");

  display.print("TEMP : ");
  display.print(temperature);
  display.println(" C");

  display.print("AIR  : ");
  display.print(airflow);
  display.println(" %");

  display.print("DAMP : ");
  display.print(damper);
  display.println(" DEG");

  display.print("MODE : ");

  if (furnaceON)
    display.println("ON");
  else
    display.println("OFF");

  display.print("STAT : ");
  display.println(status);

  display.display();
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  // ADC inputs
  pinMode(TEMP_PIN, INPUT);
  pinMode(AIRFLOW_PIN, INPUT);

  // LEDs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Pushbutton
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Servo
  ledcAttach(
    SERVO_PIN,
    servoFreq,
    servoResolution
  );

  setDamperAngle(0);

  // OLED
  Wire.begin(SDA_PIN, SCL_PIN);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C))
  {
    Serial.println("OLED ERROR");
  }
  else
  {
    Serial.println("OLED OK");
  }

  // Initial LED state
  allLEDOff();

  digitalWrite(GREEN_LED, HIGH);

  // Startup OLED
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 10);
  display.println("INDUSTRIAL FURNACE");

  display.setCursor(0, 28);
  display.println("MONITORING SYSTEM");

  display.setCursor(0, 45);
  display.println("PRESS BUTTON");

  display.display();

  delay(2000);
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop()
{
  // ===================================================
  // BUTTON CONTROL
  // ===================================================

  bool buttonState = digitalRead(BUTTON_PIN);

  if (
    buttonState == LOW &&
    lastButtonState == HIGH &&
    millis() - lastButtonTime > debounceDelay
  )
  {
    furnaceON = !furnaceON;

    lastButtonTime = millis();

    Serial.print("FURNACE: ");

    if (furnaceON)
      Serial.println("ON");
    else
      Serial.println("OFF");
  }

  lastButtonState = buttonState;

  // ===================================================
  // READ POTENTIOMETERS
  // ===================================================

  int tempADC = analogRead(TEMP_PIN);
  int airflowADC = analogRead(AIRFLOW_PIN);

  // Convert ADC to engineering values

  int temperature = map(
    tempADC,
    0,
    4095,
    20,
    100
  );

  int airflow = map(
    airflowADC,
    0,
    4095,
    0,
    100
  );

  // ===================================================
  // FURNACE OFF
  // ===================================================

  if (!furnaceON)
  {
    allLEDOff();

    digitalWrite(GREEN_LED, HIGH);

    noTone(BUZZER_PIN);

    setDamperAngle(0);

    showDisplay(
      temperature,
      airflow,
      0,
      "OFF"
    );

    Serial.print("TEMP: ");
    Serial.print(temperature);

    Serial.print(" C | AIRFLOW: ");
    Serial.print(airflow);

    Serial.println(" % | FURNACE OFF");

    delay(500);

    return;
  }

  // ===================================================
  // CRITICAL CONDITION
  // ===================================================

  if (temperature > 80)
  {
    allLEDOff();

    digitalWrite(RED_LED, HIGH);

    // Continuous overheating alarm
    tone(BUZZER_PIN, 2000);

    // Fully open damper
    setDamperAngle(180);

    showDisplay(
      temperature,
      airflow,
      180,
      "CRITICAL"
    );

    Serial.println("!!! CRITICAL: OVERHEATING !!!");
  }

  // ===================================================
  // WARNING CONDITION
  // ===================================================

  else if (
    temperature >= 60 ||
    airflow < 40
  )
  {
    allLEDOff();

    digitalWrite(YELLOW_LED, HIGH);

    // Short warning beep
    tone(BUZZER_PIN, 1200, 150);

    // Increase damper opening
    setDamperAngle(120);

    showDisplay(
      temperature,
      airflow,
      120,
      "WARNING"
    );

    Serial.println("WARNING: FURNACE CONDITION");
  }

  // ===================================================
  // NORMAL CONDITION
  // ===================================================

  else
  {
    allLEDOff();

    digitalWrite(GREEN_LED, HIGH);

    noTone(BUZZER_PIN);

    // Normal damper position
    setDamperAngle(60);

    showDisplay(
      temperature,
      airflow,
      60,
      "NORMAL"
    );

    Serial.println("FURNACE STATUS: NORMAL");
  }

  delay(500);
}
