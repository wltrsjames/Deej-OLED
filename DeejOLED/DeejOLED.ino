#include "Animations.h"
#include "OLED_init.h"

#include <Keyboard.h>

const int NUM_SLIDERS = 6;
const char *inputLabels[NUM_SLIDERS] = { "Master", "Discord", "Browser", "Music", "Current App", "Mic" };
// make sure each hotkey combo ends with 0
const int hotKeys[][3] = { { 0 }, { 0 }, { 0 }, { 0 }, { KEY_LEFT_CTRL, KEY_F12, 0 } };

//if starting up
boolean startUp = true;
//if an input has been adjusted
boolean inputAdjusted = false;
//if the ui countdown timer to clear it has restarted
boolean uiTimerRestarted = true;

//counter to work out how long the vol slider has been onscreen
unsigned long previousMillis = 0;
//counter to work out how animation framerate
unsigned long previousAnimationMillis = 0;
//how many milliseconds the vol slider should be onscreen
unsigned long volumeSliderTimeout = 3000;
//rng storage
int randNumber;

//POT info
const int analogInputs[NUM_SLIDERS] = { A0, A1, A6, A3, A7, A8 };

const int NUM_BUTTONS = 5;
const int digitalInputs[NUM_BUTTONS] = { 8, 9, 11, 10, 7 };
const int ledPins[NUM_BUTTONS] = { 4, 5, 2, 3, 6 };

//storage of analog inputs
int analogSliderValues[NUM_SLIDERS];
int filteredSliderValues[NUM_SLIDERS];

//indicator as to which input was last changed
int lastChangedInput;
//sets the amount of cycles until the input is read for analog inputs
const int noiseGate = 2;

bool buttonState[NUM_BUTTONS] = { 0, 0, 0, 0, 0 };
bool ledState[NUM_BUTTONS] = { 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A4));

  Keyboard.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // init buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(digitalInputs[i], INPUT_PULLUP);
  }

  // init leds
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], HIGH);
  }

  //init pots
  for (int i = 0; i < NUM_SLIDERS - 2; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  drawAnimation(chickenAnimation, chickenAnimationLength, true, 10, 1, false, (char *)"Deej OLED");

  // turn off all lights
  for (int i = 0; i < NUM_SLIDERS - 2; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  randNumber = random(0, 8);

  switch (randNumber) {
    case 1:
      drawAnimation(spaceAnimation, spaceAnimationLength, true, 10, 10, true, "");
      break;
    case 2:
      drawAnimation(penguinAnimation, penguinAnimationLength, true, 40, 20, true, "");
      break;
    case 3:
      drawAnimation(chickenAnimation, chickenAnimationLength, true, 20, 5, true, "");
      break;
    case 4:
      drawAnimation(climbAnimation, climbAnimationLength, true, 40, 20, true, "");
      break;
    case 5:
      drawAnimation(diveAnimation, diveAnimationLength, true, 30, 20, true, "");
      break;
    case 6:
      drawAnimation(spiral1Animation, spiral1AnimationLength, true, 40, 20, true, "");
      break;
    case 7:
      drawAnimation(spiral2Animation, spiral2AnimationLength, true, 40, 20, true, "");
      break;
  }
}

void drawAnimation(const unsigned char animation[][1024], int totalFrames, bool invertColours, unsigned long frameTime, int totalLoops, boolean adjustSliderValues, String message) {
  display.invertDisplay(invertColours);
  for (int loop = 0; loop < totalLoops; loop++) {
    for (int frame = 0; frame < totalFrames; frame++) {
      readButtons();

      //update and send the slider values in the render loop for optomisation
      if (adjustSliderValues) {
        updateSliderValues();
        sendSliderValues();
      }

      //set frameRate
      unsigned long currentMillis = millis();
      if (currentMillis - previousAnimationMillis > frameTime) {
        previousAnimationMillis = currentMillis;
        display.clearDisplay();

        if (inputAdjusted) {
          display.drawBitmap(0, 0, animation[frame], SCREEN_WIDTH, SCREEN_HEIGHT / 2, 1);
          drawVolume();
        } else if (message.length() > 1) {
          display.drawBitmap(0, 0, animation[frame], SCREEN_WIDTH, SCREEN_HEIGHT / 4 * 3, 1);
          drawMessage(message);
        } else {
          display.drawBitmap(0, 0, animation[frame], SCREEN_WIDTH, SCREEN_HEIGHT, 1);
        }

        display.display();
      } else {
        //if no frame is drawn, redraw it
        frame--;
      }
    }
  }
}

void drawVolume() {
  display.fillRect(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0xFF);
  display.drawRect(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0x00);
  display.setTextSize(1);                       // Normal 1:1 pixel scale
  display.setTextColor(0x00);                   // Draw white text
  display.setCursor(3, SCREEN_HEIGHT / 2 + 2);  // Start at top-left corner
  display.println(F(inputLabels[lastChangedInput]));
  display.drawRect(4, SCREEN_HEIGHT / 2 + 15, SCREEN_WIDTH - 8, SCREEN_HEIGHT / 5, 0x00);
  display.fillRect(4, SCREEN_HEIGHT / 2 + 15, (map(filteredSliderValues[lastChangedInput], 0, 100, 0, SCREEN_WIDTH - 8)), SCREEN_HEIGHT / 5, 0x00);
}

void drawMessage(String message) {
  display.fillRect(0, SCREEN_HEIGHT / 4 * 3, SCREEN_WIDTH, SCREEN_HEIGHT / 4, 0xFF);
  //  display.drawRect(0, SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2, 0x00);
  display.setTextSize(1);                           // Normal 1:1 pixel scale
  display.setTextColor(0x00);                       // Draw white text
  display.setCursor(3, SCREEN_HEIGHT / 4 * 3 + 5);  // Start at top-left corner
  display.println(message);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    int lastValue = filteredSliderValues[i];
    int filteredValue;

    if (i != 0 && ledState[i - 1]) {
      filteredSliderValues[i] = 0;
      filteredValue = 0;
      analogSliderValues[i] = 0;
    } else {
      int currentValue = map(analogRead(analogInputs[i]), 1023, 0, 0, 1023);
      analogSliderValues[i] = currentValue;
      filteredValue = map(currentValue, 0, 1023, 0, 100);
    }

    int diff = lastValue - filteredValue;

    //if the filtered value has changed and has changed by the noise gate
    if (lastValue != filteredValue && (diff >= noiseGate || diff <= -noiseGate)) {
      if (!startUp) {
        inputAdjusted = true;
      }
      filteredSliderValues[i] = filteredValue;
      lastChangedInput = i;
      uiTimerRestarted = true;
    } else {
      uiTimer();
    }
  }
  startUp = false;
}

void uiTimer() {
  unsigned long currentMillis = millis();
  if (uiTimerRestarted) {
    uiTimerRestarted = false;
    previousMillis = millis();
  }

  if (currentMillis - previousMillis > volumeSliderTimeout) {
    inputAdjusted = false;
    uiTimerRestarted = true;
  }
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }

  Serial.println(builtString);
}

void readButtons() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    bool lastButtonState = buttonState[i];
    buttonState[i] = !digitalRead(digitalInputs[i]);

    if (!lastButtonState && buttonState[i]) {
      ledState[i] = !ledState[i];

      const int* hotKeyCombo = hotKeys[i];
      const int hotKeyComboLength = sizeof (hotKeyCombo[0]);

      for (int j = 0; j < hotKeyComboLength; j++) {
        if (hotKeyCombo[j] == 0) {
          delay(100);
          Keyboard.releaseAll();
          break;
        };
        Keyboard.press(hotKeyCombo[j]);
      }
    }

    if (ledState[i]) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}
