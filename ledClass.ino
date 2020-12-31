#include <Adafruit_NeoPixel.h>
#ifndef LED
#define LED
#define LED_PIN 6
#define LED_COUNT 20

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

class led {

public:
  led() {
    location = 0;
    colorCode = 1;
    brightness = 255;
    brightnessDir = true;
  }
  led(int32_t location, uint8_t colorCode, uint8_t brightness,
      bool brightnessDir) {
    this->location = location;
    this->colorCode = colorCode;
    this->brightness = brightness;
    this->brightnessDir = brightnessDir;
  }

  ledUpdate();
  int32_t getLocation();
  uint32_t getBrightness();
  movePos(uint32_t);
  setLocation(int32_t);
  setColor(uint8_t, uint8_t);
  uint8_t getBrightVal();
  uint8_t getColorCode();
  setBrightnessDir(bool);
  bool getBrightnessDir();

private:
  int32_t location;
  uint8_t colorCode;
  uint32_t color;
  uint8_t brightness;
  bool brightnessDir;
};

#endif

// sets an led in adafruits strip
led::ledUpdate(void) {
  setColor(colorCode, brightness);
  strip.setPixelColor(location, color);
}

// tells you the location of a specific led
int32_t led::getLocation(void) { return location; }

// tells you the current brightness of an led
uint32_t led::getBrightness(void) { return brightness; }

// this adds the distance you want the led to move along the strip to the leds
// location
led::movePos(uint32_t distanceToMove) {
  if ((location + distanceToMove) > LED_COUNT) {
    location = (location + distanceToMove) - LED_COUNT;
    return;
  }
  location += distanceToMove;
  ledUpdate();
}

// this moves the led up or down a strip depending on if the distance to move is
// positive or negative
led::setLocation(int32_t newLocation) {
  if (newLocation < 0) {
    newLocation = 0;
    return;
  }
  location = newLocation % LED_COUNT;
  ledUpdate();
}

// changes the color of an led. Must be an ada fruit color declared with
// |strip.setColor()|
led::setColor(uint8_t clrcd, uint8_t brtlvl) {

  colorCode = clrcd;
  brightness = brtlvl;

  switch (clrcd) {
  case 0:
    // white
    color = strip.Color(brightness, brightness, brightness);
    break;
  case 1:
    // red
    color = strip.Color(0, brightness, 0);
    break;
  case 2:
    // green
    color = strip.Color(brightness, 0, 0);
    break;
  case 3:
    // blue
    color = strip.Color(0, 0, brightness);
    break;
  case 4:
    // yellow
    color = strip.Color((brightness / 2), (brightness / 2), 0);
    break;
  case 5:
    // teal
    color = strip.Color((brightness / 2), 0, (brightness / 2));
    break;
  case 6:
    // purple
    color = strip.Color(0, (brightness / 2), (brightness / 2));
    break;
  }
}

uint8_t led::getColorCode(void) { return colorCode; }

uint8_t led::getBrightVal(void) { return brightness; }

led::setBrightnessDir(bool upOrDown) {
  if (upOrDown) {
    brightnessDir = true;
  } else {
    brightnessDir = false;
  }
}

bool led::getBrightnessDir(void) {
  if (brightnessDir) {
    return true;
  } else {
    return false;
  }
}

//========================================================================

// swaps two uint32_t's
void swap(uint32_t &swap1, uint32_t &swap2) {
  uint32_t tempSwap = swap1;
  swap1 = swap2;
  swap2 = tempSwap;
}

// fills an array with numbers in numerical order from 0 to |pixelNum|
void fillArr(uint32_t pixelNum, uint32_t *arrToFill) {
  if (!arrToFill) {
    return;
  }
  for (int i = 0; i < pixelNum; ++i) {
    arrToFill[i] = i;
  }
}

// scrambles an array
void randomArrMaker(uint32_t pixelNum, uint32_t *randomArr) {
  if (!randomArr) {
    return;
  }
  fillArr(pixelNum, randomArr);
  for (int i = 0; i < pixelNum; ++i) {
    int randomIndex = random(pixelNum);
    swap(randomArr[i], randomArr[randomIndex]);
  }
}

// Lights up the whole strip any single color you want
void lightAll(uint32_t color) {
  for (int i = 0; i < 20; ++i) {
    strip.setPixelColor(i, color);
    strip.show();
  }
}

// populates the strip by lighting random pixels up in one
// leaves the pixels on, so you need to use |strip.clear()| tin tandem with this
// function requires the speed at which you want this function to happen
// requires the number of pixels in a strip
void randomPopulate(uint32_t pixelNum, uint32_t color, int DELAY) {
  uint32_t randomArr[pixelNum];
  randomArrMaker(pixelNum, randomArr);
  int flickSpeed = 20;
  for (int i = 0; i < pixelNum; ++i) {
    strip.setPixelColor(randomArr[i], color);
    delay(DELAY);
    strip.show();
  }
  lightAll(color);
  delay(DELAY);
  strip.show();
}

// turns off pixels one by one in a random manner
// requires the speed at which you want to this process to happen
// requires the number of pixels in a strip
void randomVacate(uint32_t pixelNum, int DELAY) {
  uint32_t randomArr[pixelNum];
  randomArrMaker(pixelNum, randomArr);
  int flickSpeed = 20;
  for (int i = 0; i < pixelNum; ++i) {
    strip.setPixelColor(randomArr[i], 0, 0, 0);
    delay(200);
    strip.show();
  }
  strip.clear();
  delay(DELAY);
}

// slowly turns on and off leds at a random location and color
// takes a bool to turn on the loop
// |ledArrSize| dictates how many leds to flicker
// |flickRate| is the rate at which they flicker
void randColorFlicker(bool on, uint32_t ledArrSize, uint8_t flickRate) {

  // check to make sure that you are not overloading the pixel strip
  // if you do it will just light one led as an indication you are feeding it to
  // big a number
  if (ledArrSize >= LED_COUNT) {
    ledArrSize = 1;
  }

  // create an array of leds and an array of random locations
  led ledArr[ledArrSize];
  uint32_t randomArr[LED_COUNT];
  randomArrMaker(LED_COUNT, randomArr);

  // go through the led array and randomly choose color/location/direction
  for (int i = 0; i < ledArrSize; ++i) {
    ledArr[i].setLocation(randomArr[i]);
    ledArr[i].setColor(random(8), random(255));
    if ((i % 2) == 1) {
      ledArr[i].setBrightnessDir(true);
    } else {
      ledArr[i].setBrightnessDir(false);
    }
  }

  // cycle through the led array and flicker the leds
  while (on) {
    for (int i = 0; i < ledArrSize; ++i) {
      int color = ledArr[i].getColorCode();
      int brightness = ledArr[i].getBrightVal();
      // check direction to see if the led needs to increase in brightness
      if (ledArr[i].getBrightnessDir()) {
        ledArr[i].setColor(color, (brightness + 1));
        if (ledArr[i].getBrightVal() >= 255) {
          ledArr[i].setBrightnessDir(false);
        }
        ledArr[i].ledUpdate();
      }
      // check direction to see if the led needs to decrease in brightness
      if (!ledArr[i].getBrightnessDir()) {
        ledArr[i].setColor(color, (brightness - 1));
        if (ledArr[i].getBrightVal() <= 0) {
          ledArr[i].setBrightnessDir(true);
          // check to make sure the new location isn't occupied by another led
          int newRandLoc = random(LED_COUNT);
          int y = 0;
          while (y < ledArrSize) {
            if (newRandLoc == ledArr[y].getLocation()) {
              newRandLoc = random(LED_COUNT);
              y = 0;
              continue;
            }
            y++;
          }
          ledArr[i].setColor(random(8), brightness);
          ledArr[i].setLocation(newRandLoc);
        }
        ledArr[i].ledUpdate();
      }
    }
    strip.show();
    delay(flickRate);
    strip.clear();
  }
}
