
#include <Adafruit_NeoPixel.h>
//#ifndef LED
//#define LED
#define LED_PIN 6
#define LED_COUNT 42

int buttonState = 0;
const uint8_t button1 = 9;
const uint8_t button2 = 3;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

class led {

  // This started as a way for me to learn how to make classes in c/c++. It is now a functioning lamp!
  // I'm currently going back through all of the functions and giving them their own unique classes.

public:
  led() {
    location = 0;
    colorCode = 1;
    brightness = 255;
    rateOfChange = 1;
    brightnessDir = true;
  }
  led(int32_t location, uint8_t colorCode, uint8_t brightness,
      bool brightnessDir) {
    this->location = location;
    this->colorCode = colorCode;
    this->brightness = brightness;
    this->brightnessDir = brightnessDir;
  }

  // Uses a |led| to light up a led in adafruits neopixel strip.
  // Still needs to be used in tandom with |strip.show();|.
  ledUpdate();

  // Tells you the location of a specific |led|.
  int32_t getLocation();

  // Tells you the current brightness of a |led|.
  uint32_t getBrightness();

  // Moves a |led| a set distance from it's current locataion.
  movePos(uint32_t);

  // Sets a location for an |led| regardless of it's current location.
  setLocation(uint32_t);

  // Set the color and brightness. This uses it's own color code where 0 = white
  // etc. Brightness is taken as just a regular 0-255 value.
  setColor(uint8_t, uint8_t);

  // Increases brightness from it's current brightness by any given value.
  increaseBrightness(uint8_t);

  // Decreases brightness from it's current brightness by any given value.
  decreaseBrightness(uint8_t);

  uint8_t getBrightVal();
  uint8_t getColorCode();

  //This sets the |rateOfChange|.
  void setRateOfChange(uint8_t newRate);

  //This gives you the |rateOfChange|.
  uint8_t getRateOfChange(void);

  // This changes the bool value in a |led|, aka the direction of movement of
  // that |led|.
  setBrightnessDir(bool);
  bool getBrightnessDir();

private:
  uint32_t location;
  uint8_t colorCode;
  uint32_t color;
  uint8_t brightness;
  uint8_t rateOfChange;
  bool brightnessDir;
};

//#endif

led::ledUpdate(void) {
  setColor(colorCode, brightness);
  strip.setPixelColor(location, color);
}

int32_t led::getLocation(void) { return location; }

uint32_t led::getBrightness(void) { return brightness; }

led::movePos(uint32_t distanceToMove) {
  location = (location + distanceToMove) % LED_COUNT;
  ledUpdate();
}

led::setLocation(uint32_t newLocation) {
  location = newLocation % LED_COUNT;
  ledUpdate();
}

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
  case 7:
    // orangy red for |flame| function
    color = strip.Color((brightness / 4), brightness, 0);
    break;
  case 8:
    // Soft white for the actual lamp function of the lamp.
    color = strip.Color(brightness / 2, brightness, 0);
    break;
  case 9:
    // Blue ish white for the stars function.
    color = strip.Color(brightness / 2, (brightness / 16) * 7,
                        (brightness / 16) * 7);
  }
}

led::increaseBrightness(uint8_t adjustBy) {
  if (brightness + adjustBy >= 255) {
    brightness = 255;
    ledUpdate();
    return;
  }
  brightness += adjustBy;
  ledUpdate();
}

led::decreaseBrightness(uint8_t adjustBy) {
  if (brightness - adjustBy <= 0) {
    brightness = 0;
    ledUpdate();
    return;
  }
  brightness -= adjustBy;
  ledUpdate();
}

uint8_t led::getColorCode(void) { return colorCode; }

uint8_t led::getBrightVal(void) { return brightness; }

void led::setRateOfChange(uint8_t newRate) { rateOfChange = newRate; }

uint8_t led::getRateOfChange(void) { return rateOfChange; }


led::setBrightnessDir(bool upOrDown) {
  if (upOrDown) {
    brightnessDir = true;
  } else {
    brightnessDir = false;
  }
}

// Gives you the direction of a |led|.
bool led::getBrightnessDir(void) {
  if (brightnessDir) {
    return true;
  } else {
    return false;
  }
}

//==============================================================buttonCheck

#define BUTTON_PRESSED HIGH //LOW

// checks to see the current state of all the possible button presses
void buttonCheck(bool &on, uint8_t &brightnessStand) {
  if (digitalRead(button1) == BUTTON_PRESSED && digitalRead(button2) == BUTTON_PRESSED) {
    delay(500);
    if (digitalRead(button1) == BUTTON_PRESSED && digitalRead(button2) == BUTTON_PRESSED) {
      on = false;
    }
  }

  if (digitalRead(button1) == BUTTON_PRESSED) {
    delay(50);
    brightnessStand += 5;
    if (brightnessStand >= 250) {
      brightnessStand = 250;
    }
  }
  if (digitalRead(button2) == BUTTON_PRESSED) {
    delay(50);
    brightnessStand -= 5;
    if (brightnessStand < 20) {
      brightnessStand = 20;
    }
  }
  strip.setBrightness(brightnessStand);
  strip.show();
}

//==============================================================randomArrMaker

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

//==============================================================LightALL

// Lights up the whole strip any single color you want.
// Assuming you are using a color code in the led class.
void lightAll(uint8_t colorCode) {
  led ledToLight;
  ledToLight.setColor(colorCode, 255);
  ledToLight.ledUpdate();
  for (int i = 0; i < LED_COUNT; ++i) {
    ledToLight.setLocation(i);
    strip.show();
  }
}

// does the same thing but with a color code
void lightAll(uint8_t g, uint8_t r, uint8_t b) {
  for (int i = 0; i < LED_COUNT; ++i) {
    strip.setPixelColor(i, g, r, b);
    strip.show();
  }
}

// THIS ONE TAKES BOOL FOOL also a color code.
// use this for a light feature as it will stay on until a button is pressed
void lightAll(bool &on, uint8_t &brightness, uint8_t colorCode) {
  led ledToLight;
  ledToLight.setColor(colorCode, 255);
  ledToLight.ledUpdate();
  while (on) {
    for (int i = 0; i < LED_COUNT; ++i) {
      ledToLight.setLocation(i);
      strip.show();
    }
    buttonCheck(on, brightness);
  }
}

//==============================================================randomPopulate

// populates the strip by lighting random pixels up in one
// leaves the pixels on, so you need to use |strip.clear()| tin tandem with this
// function requires the speed at which you want this function to happen
// requires the number of pixels in a strip
void randomPopulate(uint32_t pixelNum, uint32_t color, int DELAY) {
  uint32_t randomArr[pixelNum];
  randomArrMaker(pixelNum, randomArr);
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

void randomPopNVac(bool on, uint8_t &brightness, uint32_t pixelNum, int DELAY) {
  uint32_t red = strip.Color(0, 255, 0);
  while (on) {
    //    buttonCheck(on, brightness);
    randomPopulate(pixelNum, red, DELAY);
    //   buttonCheck(on, brightness);
    randomVacate(pixelNum, DELAY);
  }
}

//==============================================================fadeOnOff

void fadeOnOff(uint8_t colorCode, uint8_t fadeRate, uint8_t brightnessMin, uint8_t brightnessMax) {

  if (brightnessMax > 255) {
    brightnessMax = 255;
  }

  uint8_t brightness = brightnessMin;
  led ledToFade(0, colorCode, brightness, true);

  while (brightness < brightnessMax) {
    for (int i = 0; i < LED_COUNT; ++i) {
      ledToFade.setLocation(i);
      ledToFade.setColor(colorCode, brightness);
    }
    strip.show();
    delay(fadeRate);
    brightness += 5;
    strip.show();
  }
  while (brightness > brightnessMin) {
    for (int i = 0; i < LED_COUNT; ++i) {
      ledToFade.setLocation(i);
      ledToFade.setColor(colorCode, brightness);
    }
    strip.show();
    delay(fadeRate);
    brightness -= 5;
    strip.show();
  }
}

void startUp(uint32_t colorCode) {

  fadeOnOff(colorCode, 5, 50, 90);
  fadeOnOff(colorCode, 8, 70, 100);
  delay(30);
  fadeOnOff(colorCode, 6, 80, 120);
  delay(50);
  fadeOnOff(colorCode, 4, 100, 200);
  delay(300);
  fadeOnOff(colorCode, 8, 70, 100);
  lightAll(colorCode);
  delay(colorCode);
  fadeOnOff(colorCode, 3, 100, 255);
  lightAll(colorCode);
}

//==============================================================randColorFlicker

// slowly turns on and off leds at a random location and color
// takes a bool to turn on the loop
// |numOfDots| dictates how many leds to flicker
class randDot {
  
  public:
    // cycles a |randDot| from off to on then back to off.
    // Each time it reaches a 0 value, it will change its color. 
    // The location will stay the same unless specified otherwise.
    void flicker(void) {
      if (fadeDir) {
        increaseBrightness();
      } else {
        decreaseBrightness();
      }
      strip.setPixelColor(
        location,
        red * brightness / 255,
        green * brightness / 255,
        blue * brightness / 255
      );
      if (brightness > 250) {
        fadeDir = false;
      }
      if (brightness == 0) {
        fadeDir = true;
        readyToMove = true;
        fadeRate = random(5);
        changeColor();
      }
    }

    // This sets the location of the |randDrop|.
    void setLocation(uint32_t newLocation) {
      location = newLocation;
      readyToMove = false;
    }

    // This tells you the location of a |randDot|.
    uint32_t getLocation(void) {
      return location;
    }

    // Tells you if the |randDot| is at a 0 value and ready to change location.
    bool isReadyToMove(void) {
      return readyToMove;
    }
  

  private:

    // Finds a new color for a |randDot|.
    void changeColor(void) {
      uint32_t tempColor = random(16777216);
      red = tempColor >> 16;
      green = tempColor >> 8;
      blue = tempColor >> 0;
    }

    // Increases the brightness of a |randDot|.
    void increaseBrightness(void) {
      brightness += fadeRate;
      if(brightness > 255) {
        brightness = 255;
      }
    }

    // Decreases the brightness of a |randDot|.
    void decreaseBrightness(void) {
      brightness -= fadeRate;
      if(brightness < 3) {
        brightness = 0;
      }
    }
  
    bool readyToMove = false;
    bool fadeDir = true;
    uint8_t red = random(255);
    uint8_t green = random(255);
    uint8_t blue = random(255);
    uint8_t brightness = 0;
    uint8_t fadeRate = random(5);
    uint32_t location = 0;
  
};


uint32_t findNewLocation(uint32_t* randDotLocations, uint32_t numOfDots) {
    uint32_t newRandLoc = random(LED_COUNT);
    uint32_t y = 0;
    while (y < numOfDots) {
      if (newRandLoc == randDotLocations[y]) {
      newRandLoc = random(LED_COUNT);
      y = 0;
      continue;
    }
    y++;
  }
  return newRandLoc;
}

void flickerRandDots(bool &on, uint8_t &brightnessStand, uint32_t numOfDots) {

  if (numOfDots >= LED_COUNT) {
    numOfDots = LED_COUNT - 1;
  }
  
  uint32_t count = 0;
  uint8_t timer = 10;
  uint32_t randomArr[LED_COUNT];
  uint32_t randDotLocations[numOfDots];
  randomArrMaker(LED_COUNT, randomArr);
  randDot randDotArr[numOfDots];

  for(int i = 0; i < numOfDots; ++i) {
    randDotArr[i].setLocation(randomArr[i]);
    randDotLocations[i] = randDotArr[i].getLocation();
  }

  while(on) {
    if(count % timer == 0) {
      strip.clear();
      for(int i = 0; i < numOfDots; ++i) {
        randDotArr[i].flicker();
        if(randDotArr[i].isReadyToMove()) {
          randDotArr[i].setLocation(findNewLocation(randDotLocations, numOfDots));  
          randDotLocations[i] = randDotArr[i].getLocation();
        }
      }
    }
    strip.show();
    delay(1);
    count++;
    buttonCheck(on, brightnessStand);
  }
}


//==============================================================Flame

class FlameDot {

  public:

    setLocation(uint32_t setLocation) {
      location = setLocation;
      bounceUp();
    }

    void flicker(void) {
      if (tickerDir) {
        increaseBrightness();
        bounceDown();
      } else {
        decreaseBrightness();
        bounceUp();
      }
      displayDot();
    }

  private:
    // Changes the dot from decreasing in brightness to increasing.
    // It first checks to make sure the dot has reached it's low limit and is moving downward. 
    // It will then either asign it a high value or a low value.
    void bounceUp(void) {
      if(ticker <= lowLimit) {
        tickerDir = true;
        lowLimit = random(20, 35);
        //This is how many times we are going to divde the red value by. This is not the 8bit RGB value.
        orangeVal = random(4, 8);
        uint8_t hotOrNot = random(100);
        if (hotOrNot < 20) {
          highLimit = random(150, 250);
          tickerRate = random(3, 5);
        } else {
          highLimit = random(40, 60);
          tickerRate = random(1, 2);
        }
      }
    }

    // Just changes the tickerDir to false, thus causing the dot to decrease in brightness.
    void bounceDown(void) {
      if (ticker >= highLimit) {
        tickerDir = false;
      }
    }

    void increaseBrightness(void) {
      ticker += tickerRate;
    }

    void decreaseBrightness(void) {
      ticker -= tickerRate;
    }

    void displayDot(void) {
      strip.setPixelColor(location, ticker / orangeVal, ticker, 0);
    }
    
    bool tickerDir = true;
    uint8_t location = 0;
    uint8_t ticker = 0;
    uint8_t tickerRate = 0;
    uint8_t highLimit = 0;
    uint8_t lowLimit = 0;
    uint8_t orangeVal = 0;
};


void Flame(bool &on, uint8_t &brightnessStand) {
  uint32_t count = 0;
  uint32_t rate = 7;

  FlameDot flameArray[LED_COUNT];
  for(int i = 0; i < LED_COUNT; ++i) {
    flameArray[i].setLocation(i);
  }

  while(on) {
    if(count % rate == 0) {
      for(int i = 0; i < LED_COUNT; ++i) {
        flameArray[i].flicker();
      }
    }
    strip.show();
    delay(1);
    count++;
    buttonCheck(on, brightnessStand);
  }
}

//==============================================================blob
uint8_t globalTrackVals[LED_COUNT];

void cleanBlobTrack() {
  for (int i = 0; i < LED_COUNT; ++i) {
    globalTrackVals[i] = 0;
  }
}

void displayBlobs(uint32_t tail, uint32_t head) {
  uint32_t index = 4;
  uint32_t red = strip.Color(0, 255, 0);
  strip.fill(red, 0, 4);
  while (index < LED_COUNT) {
    uint32_t start = index * 255; // start insures that the index lines up with the tail and head.
    for (int i = start; i < start + 255; ++i) {
      if (i >= tail && head >= i) {
        globalTrackVals[index] += 1;
        if (globalTrackVals[index] >= 255) {
          globalTrackVals[index] = 255;
          break;
        }
      }
    }
    index += 1;
  }
}


class Blob {
  
  public:

  uint32_t getHead() {
    return head;
  }

  uint32_t getTail() {
    return tail;
  }

  void moveBlob() {
    // This gives me 0 at 0 and 20 when the head is at the top.
    // This means a blob needs to be going faster then 20 in order to make it to the top.
    size_t gravity = (head * head) / 1200000;
    if (dir) {
      moveUpTrack(blobSpeed - gravity);
    } else {
      moveDownTrack(blobSpeed - gravity);
    }
    displayBlob();
  }

  void displayBlob() {
    for (int i = 0; i < LED_COUNT; ++i) {
      strip.setPixelColor(i, 0, globalTrackVals[i], 0);
    }
  }


  void moveDownTrack(int32_t distance) {
    head -= distance;
    tail -= distance;
    if (tail <= 0) {
      resetBlob();
      setBlobVals();
    }
  }

  void moveUpTrack(int32_t distance) {
    if (distance <= 1) {
      dir = false;
      return;
    }
    head += distance;
    tail += distance;
    if (head >= trackSize) {
      resetBlob();
    }
  }

  
  private: 

  void resetBlob() {
    if (dir) {
      head = trackSize;
      tail = head - 255;
      dir = false;
    } else {
      head = 255;
      tail = 0;
      dir = true;
    }
  }
  
  setBlobVals() {
    blobSpeed = random(20, 90);
    head = random(1, 4) * 255;
  }
  static const int32_t trackSize = LED_COUNT * 255;
  int32_t head = random(1, 4) * 255;
  int32_t tail = 0;
  int32_t blobSpeed = random(20, 90); 
  bool dir = true;
};

void lavaLamp(bool &on, uint8_t &brightnessStand) {
  uint32_t blobArrSize = 4;
  Blob blobArr[blobArrSize];
  while(on) {
    cleanBlobTrack();
    for (int i = 0; i < blobArrSize; ++i) {
      blobArr[i].moveBlob();
      displayBlobs(blobArr[i].getTail(), blobArr[i].getHead());
    }
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
  }
}
//==============================================================randColorFade

void randColorFade(bool &on, uint8_t &brightnessStand) {
  int32_t cycle = 0;
  int32_t count = 40;
  int8_t incRed = random(1, 10);
  int8_t incGreen = random(1, 10);
  int8_t incBlue = random(1, 10);
  int8_t switchCase = 0;
  led red(0, 0, random(255), true);
  led green(0, 0, random(255), false);
  led blue(0, 0, random(255), true);

  while (on) {
    if (cycle % count == 0) {
      if (red.getBrightnessDir()) {
        red.increaseBrightness(4);
        if (red.getBrightVal() == 255) {
          red.setBrightnessDir(false);
          incRed = random(1, 10);
        }
      } else {
        red.decreaseBrightness(4);
        if (red.getBrightVal() == 0) {
          red.setBrightnessDir(true);
          incRed = random(1, 10);
        }
      }

      if (green.getBrightnessDir()) {
        green.increaseBrightness(incGreen);
        if (green.getBrightVal() == 255) {
          green.setBrightnessDir(false);
          incGreen = random(1, 10);
        }
      } else {
        green.decreaseBrightness(incGreen);
        if (green.getBrightVal() == 0) {
          green.setBrightnessDir(true);
          incGreen = random(1, 10);
        }
      }

      if (blue.getBrightnessDir()) {
        blue.increaseBrightness(incBlue);
        if (blue.getBrightVal() == 255) {
          blue.setBrightnessDir(false);
          incBlue = random(1, 10);
        }
      } else {
        blue.decreaseBrightness(incBlue);
        if (blue.getBrightVal() == 0) {
          blue.setBrightnessDir(true);
          incBlue = random(1, 10);
        }
      }
      lightAll(red.getBrightVal(), green.getBrightVal(), blue.getBrightVal());
    }
    buttonCheck(on, brightnessStand);
    delay(1);
    cycle++;
  }
}

//==============================================================stars



/*==============================================================stars
 * 
 * This used the StarDot class to do basically exatly the same thing as the flame function in white.
 * This time instead of all the dots being lit, each time the function is called it picks a random pattern and sticks to it.
 */

class StarDot {

  public:
    
    setLocation(uint32_t setLocation) {
      location = setLocation;
      bounceUp();
    }

    void flicker(void) {
      // This moves each led up and down in it's brightness values.
      if (tickerDir) {
        increaseBrightness();
        bounceDown();
      } else {
        decreaseBrightness();
        bounceUp();
      }
      displayDot();
    }
    

  private:
    void bounceUp(void) {
      if(ticker <= lowLimit) {
        tickerDir = true;
        lowLimit = random(20, 30);
        uint8_t yesTwinkle = random(50);
        if (yesTwinkle <= 1) { // This creates a 2 in 50 chance of a StarDot getting brighter then the others.
          highLimit = random(150, 250); // It gets a new higher high limit if that is the case.
          tickerRate = random(1, 2);
        } else {
          highLimit = random(30, 60); // Other wise it gets a lower hight limit
          tickerRate = random(1, 2);
        }
      }
    }

    void bounceDown(void) {
      if (ticker >= highLimit) {
        tickerDir = false;
      }
    }

    void increaseBrightness(void) {
      ticker += tickerRate;
    }

    void decreaseBrightness(void) {
      ticker -= tickerRate;
    }

    void displayDot(void) {
      strip.setPixelColor(location, ticker * 7 / 16, (ticker * 7 /16),
                        ticker / 2);
    }
    
    bool tickerDir;
    uint8_t ticker = 0;
    uint8_t tickerRate = 0;
    uint8_t lowLimit = 0;
    uint8_t highLimit = 0;
    uint32_t location = 0;
};

void stars(bool &on, uint8_t &brightnessStand) {
  uint32_t count = 0;
  uint8_t rate = 22;
  uint32_t numOfStars = LED_COUNT * 3 / 5;
  StarDot starArr[numOfStars];
  uint32_t randomArr[LED_COUNT];
  randomArrMaker(LED_COUNT, randomArr);

  for (int i = 0; i < numOfStars; ++i) {
    starArr[i].setLocation(randomArr[i]);
  }

  while(on) {
    if(count % rate == 0) {
      for(int i = 0; i < LED_COUNT; ++i) {
        starArr[i].flicker();
      }
    } 
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
    count++;
  }
}

//==============================================================Heat Dissapate

class HeatDrop {

  public:

    void dissipate(uint8_t valA, uint8_t valB, bool bounce) {
      uint8_t meanTemp = (valA + valB) / 2;
      if (!heatingUp) {
        if (meanTemp > heatVal && heatVal < 250) {
          heatVal += pow(meanTemp - heatVal, (1/2));
        } else if (meanTemp < heatVal && heatVal > 1) {
          heatVal -= pow(heatVal - meanTemp, (1/2));
        }
      } else {
        heatVal += 1;
      }
      if (heatVal < 1) {
        heatVal = 1;
      }
      if (heatVal > 250) {
        heatVal = 250;
        heatingUp = false;
      }
      displayDot(bounce);
    }

    uint8_t getHeatVal(void) {
      return heatVal;
    }

    void setLocation(uint32_t newLocation) {
      location = newLocation;
    }

    void heatDotUp(void) {
      heatingUp = true;
    }

  private:
    bool heatingUp = false;
    uint8_t heatVal = 1;
    uint32_t location;
    void displayDot(bool bounce) {
      if (bounce) {
        strip.setPixelColor(location, 0, heatVal, 250 - heatVal);
      } else {
      strip.setPixelColor(location, 250 - heatVal, heatVal, 0);
      }
    }
};


void heatDiss(bool &on, uint8_t &brightnessStand) {
  uint32_t cycle = 1;
  uint8_t count = 8;
  uint32_t newDotCount = 500;
  uint8_t valA = 0, valB = 0;
  bool bounce = false;
  
  HeatDrop heatArr[LED_COUNT];
  for(int i = 0; i < LED_COUNT; ++i) {
    heatArr[i].setLocation(i);
  }

  while(on) {
    if (cycle % count == 0) {
      for(int i = 0; i < LED_COUNT; ++i) {
        if (i > 0) {
          valA = heatArr[i - 1].getHeatVal();
        } else {
          valA = 100;
        }
        if(i < LED_COUNT) {
          valB = heatArr[i + 1].getHeatVal();
        } else {
          valB = 100;
        }
        heatArr[i].dissipate(valA, valB, bounce);
      }
    }
    if (cycle % newDotCount == 0) {
      uint32_t hotDot = random(LED_COUNT);
      heatArr[hotDot].heatDotUp();
      newDotCount = random(3000, 4000);
    }
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
    cycle++;
  }
}


void heatBounce(bool &on, uint8_t &brightnessStand) {
  uint32_t cycle = 1;
  uint8_t count = 4;
  uint32_t moveOne = 150;
  uint8_t valA = 0, valB = 0;
  uint32_t hotDot = 0;
  bool dir = true;
  bool bounce = true;
  
  HeatDrop heatArr[LED_COUNT];
  for(int i = 0; i < LED_COUNT; ++i) {
    heatArr[i].setLocation(i);
  }

  while(on) {
    if (cycle % count == 0) {
      for(int i = 0; i < LED_COUNT; ++i) {
        if (i > 0) {
          valA = heatArr[i - 1].getHeatVal();
        } else {
          valA = 100;
        }
        if(i < LED_COUNT) {
          valB = heatArr[i + 1].getHeatVal();
        } else {
          valB = 100;
        }
        heatArr[i].dissipate(valA, valB, bounce);
      }
    }
    if (cycle % moveOne == 0) {
      if (dir) {
         hotDot++;
         if (hotDot == LED_COUNT) {
          dir = false;
         }
       } else {
         hotDot--;
         if (hotDot == 0) {
           dir = true;
         }
       }
       heatArr[hotDot].heatDotUp();
    }
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
    cycle++;
  }
}


//==============================================================Bouncy Ball



class Ball {
  
  public:

  void moveBall() {
    timer++;
    if (dir) {;
      moveUpTrack(calcRateUp());
    } else {
      moveDownTrack(calcRateDown());
    }
    displayBall();
  }
  
  private: 

  size_t calcRateUp() {
    return 2*-timer + velocity;
  }

  size_t calcRateDown() {
    return timer + velocity;
  }

  void moveDownTrack(int32_t distance) {
    head -= distance;
    tail -= distance;
    if (tail <= 0) {
      resetBall(255);
    }
  }

  void moveUpTrack(int32_t distance) {
    if (distance <= 1) {
      resetBall(head);
      return;
    }
    head += distance;
    tail += distance;
    if (head >= trackSize) {
      resetBall(trackSize);
    }
  }

  void resetBall(uint32_t headLocation) {
    dir = !dir;
    head = headLocation;
    tail = head - 255;
    timer = 0;
    if (headLocation == LED_COUNT || headLocation == 255) {
      bounce();
    }
    if (velocity == 0) {
      velocity = random(100, 500);
      bouncy = random(40,90);
      strip.setPixelColor(0, 255, 0, 255);
      strip.show();
      delay(1000);
    }
  }

  void bounce() {
    if (dir) {
      velocity -= bouncy;
    } else {
      velocity = 2*-timer + velocity;
      //velocity -= bouncy;
    }
    if (velocity <= 0) {
      velocity = 0;
    }
  }

  void displayBall() {
    for (int i = 0; i < LED_COUNT; ++i) {
      trackVals[i] = 0; // reset the track values so it doesn't just light the whole strip
    }
    uint32_t index = 0;
    while (index < LED_COUNT) {
      uint32_t start = index * 255; // start insures that the index lines up with the tail and head.
      for (int i = start; i < start + 255; ++i) {
        if (i >= tail && head >= i) {
          trackVals[index] += 1;
          if (trackVals[index] >= 255) {
            trackVals[index] = 255;
            break;
          }
        }
      }
      index += 1;
    }
    for (int i = 0; i < LED_COUNT; ++i) {
      strip.setPixelColor(i, trackVals[i], 0, trackVals[i]);
    }
  }
  
  static const int32_t trackSize = LED_COUNT * 255;
  uint8_t trackVals[LED_COUNT];
  int32_t head = 255;
  int32_t tail = 0;
  int32_t velocity= random(100, 500);
  int8_t bouncy = random(40, 90);
  size_t timer = 0; 
  bool dir = true;
};

void bouncyBall(bool &on, uint8_t &brightnessStand) {
  Ball bouncyBoy;
  while(on) {
    bouncyBoy.moveBall();
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
  }
}
