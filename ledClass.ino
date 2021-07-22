#include <Adafruit_NeoPixel.h>
//#ifndef LED
//#define LED
#define LED_PIN 6
#define LED_COUNT 20

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
  for (int i = 0; i < 20; ++i) {
    ledToLight.setLocation(i);
    strip.show();
  }
}

// does the same thing but with a color code
void lightAll(uint8_t g, uint8_t r, uint8_t b) {
  for (int i = 0; i < 20; ++i) {
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
    for (int i = 0; i < 20; ++i) {
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
      red = random(255);
      green - random(255);
      blue = random(255);
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

#ifndef BLOB
#define BLOB

void lavaBackGround(int32_t top) {
  for (int i = 0; i < LED_COUNT - 1; ++i) {
    if (i < top) {
      strip.setPixelColor(i, 0, 150, 0);
    } else {
      strip.setPixelColor(i, 0, 20, 0);
    }
  }
}

bool atTop(int32_t head) {
  if (head == LED_COUNT) {
    return true;
  } else {
    return false;
  }
}

bool atBottom(int32_t head) {
  if (head == -4) {
    return true;
  } else {
    return false;
  }
}

uint8_t adjustedCount(uint32_t count, uint32_t head) {
  // counter is your frame rate, it's value changes each cycle. 0 - infi
  // count fixed value based on the blob. Needs to be between 2 and 10. 2 being
  // fastest and 10 being stopped. head ranges from 0 - 19 (LED_COUNT) Updates
  // more frequent at zero and less frequent at 19.
  return minOf(13, count + (head * head) / 45);
}

uint32_t minOf(uint32_t x, uint32_t y) { return x < y ? x : y; }

class blob {

public:
  blob() {
    counter = 0;
    brightness = 20;
    head = 3;
    count = 1;
    state = 0;
  }
  // Creates a blob out of the following data members.
  // The |dist| is an internal data member that counts up everytime the blob is
  // moved. The |head| is the top of blob, and dictates where the rest of the
  // blob is located. The |count| acts as a delay defore moving a blob. The
  // |state| is the direction the blob is moving in.
  blob(int32_t head, uint32_t count, bool state){};

  // Moves a blob across the strip based on preset data members.
  // Left alone this will just bounce a blob across the strip.
  moveBlob(int32_t);

  // Lets you set the location of the top of the blob;
  setHead(int32_t);

  // Gives you the location of the top of the blob.
  int32_t getHead();

  // This sets the delay for each blobs movement.
  setCount(uint32_t);

  // Tells you the current |count| of a blob.
  uint32_t getCount();

  // Sets the direction of a blob.
  setState(int8_t);

  // Tells you the current direction of a blob.
  int8_t getState();

private:
  uint32_t counter;
  uint32_t brightness;
  int32_t head;
  uint32_t count;
  int8_t state;
};

#endif

blob::moveBlob(int32_t bottom) {
  uint8_t baseBrightness = 20;
  if (counter % adjustedCount(count, head) == 0) {
    brightness += 2; // Increases the brightness everytime counter matches
                     // evenly with the count.
  }
  counter++;        // Increase the counter every time through the function.
  if (state == 0) { // If the blob is going up.
    if (head - 3 > bottom) {
      strip.setPixelColor(head - 3, 0, 120 - brightness, 0);
    }
    if (head - 2 > bottom) {
      strip.setPixelColor(head - 2, 0, 200 - brightness, 0);
    }
    if (head - 1 > bottom) {
      strip.setPixelColor(head - 1, 0, 100 + brightness, 0);
    }
    if (head > bottom) {
      strip.setPixelColor(head, 0, brightness, 0);
    }
    strip.show();
    if (brightness >= 100) { // If the brightness is at 100 the blob is ready to
                             // be moved along the strip.
      brightness = baseBrightness;
             // Resets brightness for the whole thing to start over again.
      head++; // Moves the blob up the strip one led.
    }
    if (atTop(head) || adjustedCount(count, head) == 13) { // Turns around the blob if its at the top.
      state = 1;
      head -= 4;
    }
  }

  if (state == 1) {
    if (head > bottom) {
      strip.setPixelColor(head, 0, 0, 0);
    }
    if (head + 1 > bottom) {
      strip.setPixelColor(head + 1, 0, 100, 0);
    }
    if (head + 2 > bottom) {
      strip.setPixelColor(head + 2, 0, 200, 0);
    }
    if (head + 3 > bottom) {
      strip.setPixelColor(head + 3, 0, 100, 0);
    }
    strip.show();
    if (brightness >= 200) {
      brightness = baseBrightness;
      state = 2;
    }
  }

  if (state == 2) { // If the blob is going down.
    if (head > bottom) {
      strip.setPixelColor(head, 0, brightness, 0);
    }
    if (head + 1 > bottom) {
      strip.setPixelColor(head + 1, 0, 100 + brightness, 0);
    }
    if (head + 2 > bottom) {
      strip.setPixelColor(head + 2, 0, 200 - brightness, 0);
    }
    if (head + 3 > bottom) {
      strip.setPixelColor(head + 3, 0, 120 - brightness, 0);
    }
    strip.show();
    if (brightness >= 100) {
      brightness = baseBrightness;
      head--; // Moves the blob down one led.
    }
    if (atBottom(head)) { // Turns the blob around if its at the bottom.
      state = 0;
      head = 0;
    }
  }
}

blob::setHead(int32_t newHead) {
  if (!state && newHead > LED_COUNT - 4) {
    newHead = LED_COUNT - 4;
    return;
  }
  if (state && newHead < 3) {
    newHead = 3;
    return;
  }
  head = newHead;
}

int32_t blob::getHead(void) { return head; }

blob::setCount(uint32_t newCount) { count = newCount; }

uint32_t blob::getCount() { return count; }

blob::setState(int8_t newState) { state = newState; }

int8_t blob::getState() { return state; }

void lavaLamp(bool &on, uint8_t &brightnessStand) {
  int8_t randNumBottom = 6;
  int8_t randNumbTop = 12;
  int32_t topOfGlob = 3;
  blob blob1;
  blob blob2;
  blob blob3;
  blob1.setCount(random(randNumBottom, randNumbTop));
  blob2.setCount(random(randNumBottom, randNumbTop));
  blob3.setCount(random(randNumBottom, randNumbTop));
  blob2.setHead(random(LED_COUNT));
  blob3.setHead(random(LED_COUNT));

  while (on) {
    if (atTop(blob1.getHead()) || atBottom(blob1.getHead())) {
      blob1.setCount(random(randNumBottom, randNumbTop));
    }
    if (atTop(blob2.getHead()) || atBottom(blob2.getHead())) {
      blob2.setCount(random(randNumBottom, randNumbTop));
    }
    if (atTop(blob3.getHead()) || atBottom(blob3.getHead())) {
      blob3.setCount(random(randNumBottom, randNumbTop));
    }
    lavaBackGround(topOfGlob);
    blob1.moveBlob(topOfGlob);
    blob2.moveBlob(topOfGlob);
    blob3.moveBlob(topOfGlob);
    buttonCheck(on, brightnessStand);
    strip.clear();
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

void cometTail(led cometHead, int32_t cometSize) {
  led ledCometTail[cometSize];
  uint8_t prevBright = 255;
  int32_t prevLoc = cometHead.getLocation();
  for (int i = 0; i < cometSize; ++i) {
    ledCometTail[i].setColor(0, prevBright / 2);
    ledCometTail[i].setLocation(prevLoc);
    if (cometHead.getBrightnessDir()) {
      prevLoc--;
      ledCometTail[i].setLocation(prevLoc);
    } else {
      prevLoc++;
      ledCometTail[i].setLocation(prevLoc);
    }
    prevBright = prevBright / 2;
  }
}

void stars(bool &on, uint8_t &brightnessStand) {
  uint32_t cycle = 0;
  int32_t count = 35;
  uint8_t highPeakTop = 200;
  uint8_t highPeakBottom = 100;
  uint8_t lowPeakTop = 90;
  uint8_t lowPeakBottom = 80;
  uint8_t valleyTop = 50;
  uint8_t valleyBottom = 40;
  uint8_t twinkleStar1 = 1;
  uint8_t twinkleStar2 = 2;
  int8_t ledArrSize = (LED_COUNT / 2);
  uint32_t cometCount = 255;
  int32_t cometClock = 0;
  int32_t cometHeadLoc = random(8, 13);
  int8_t cometDuration = 6;
  led cometHead(cometHeadLoc, 5, 155, random(2));

  led ledArr[ledArrSize];
  uint32_t randomArr[LED_COUNT];
  randomArrMaker(LED_COUNT, randomArr);

  for (int i = 0; i < ledArrSize; ++i) {
    ledArr[i].setColor(0, random(valleyBottom, lowPeakTop));
    ledArr[i].setLocation(randomArr[i]);
    ledArr[i].setBrightnessDir(random(2));
  }

  while (on) {
    if (cycle % count == 1) {
      strip.clear();
      for (int i = 0; i < ledArrSize; ++i) {
        if (ledArr[i].getBrightnessDir()) {
          if (i == twinkleStar1) {
            ledArr[i].increaseBrightness(8);
              if (ledArr[i].getBrightness() >= random(highPeakBottom, highPeakTop)) {
                ledArr[i].setBrightnessDir(false);
              }
            } else if (i == twinkleStar2) {
            ledArr[i].increaseBrightness(10);
              if (ledArr[i].getBrightness() >= random(highPeakBottom, highPeakTop)) {
                ledArr[i].setBrightnessDir(false);
              }
            } else {
            ledArr[i].increaseBrightness(1);
            if (ledArr[i].getBrightness() >= random(lowPeakBottom, lowPeakTop)) {
              ledArr[i].setBrightnessDir(false);
            }
          }
        } else {
          if (i == twinkleStar1) {
            ledArr[i].decreaseBrightness(8);
            if (ledArr[i].getBrightness() <= random(valleyBottom, valleyTop)) {
              ledArr[i].setBrightnessDir(true);
              twinkleStar1 = random(ledArrSize);
            }
          } else if (i == twinkleStar2) {
            ledArr[i].decreaseBrightness(10);
            if (ledArr[i].getBrightness() <= random(valleyBottom, valleyTop)) {
              ledArr[i].setBrightnessDir(true);
              twinkleStar2 = random(ledArrSize);
            } 
          } else {
            ledArr[i].decreaseBrightness(1);
            if (ledArr[i].getBrightness() <= random(valleyBottom, valleyTop)) {
              ledArr[i].setBrightnessDir(true);
            }
          }
        }
      }
      
      if (cycle % cometCount == 1) {
        cometClock++;
        cometHead.setLocation(cometHeadLoc);
        cometTail(cometHead, cometDuration);
        cometCount = count;
        if (cometHead.getBrightnessDir()) {
          cometHeadLoc++;
        } else {
          cometHeadLoc--;
        }
        if (cometClock == cometDuration) {
          cometCount = random(800, 1300);
          cometClock = 0;
          cometDuration = random(4, 8);
          cometHead.setBrightnessDir(random(2));
          if (cometHead.getBrightnessDir()) {
            cometHeadLoc = random(2, 8);
          } else {
            cometHeadLoc = random(LED_COUNT - 8, LED_COUNT - 2);
          }
        }
      }
    }
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
    cycle++;
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
  // This sends the bouncy ball on it's way. Up down and respawn. 
  void go(void) {
    t++;
    disBall();
  }
  
 private:
  int32_t t = 0; //time 
  uint8_t br = 0; //brightness
  int32_t r = random(1, 10); //rate
  int32_t bf = random(1, 90); //bounce factor
  int32_t l = 0; //location
  bool up = true;

  void ballCheck(void) {
    if (r <= 0 && l != 0) {
      up = false;
    }
    if (up && br > 250) {
      l++;
      br = 0;
    } else {
      l--;
      br = 0;
    }
    if (up && l == LED_COUNT) {
      bounce();
      t = 0;
      up = false;
    } else if (up && l == 0) {
      bounce();
      t = 0; 
      up = true;
      if (r <= 0) {
        delay(500);
        r = random(1, 10);
        bf = random(1, 90);
      }
    }
  }

  void bounce(void) {
     // tb is temporary brightness.
     uint8_t tb = r - (r * bf) / 10;
     r = (r * bf) / 100;
     strip.setPixelColor(l, tb, tb, tb); 
  }

  void disBall(void) {
    ballCheck();
    strip.setPixelColor(l, br, br, br);
    if (up && l > 0) {
      br += -.0016 * t + r;
      strip.setPixelColor(l - 1, 250 - br, 250 - br, 250 - br); 
    } else if (!up && LED_COUNT > l) {
      br += .0016 * t + r;
      strip.setPixelColor(l + 1, 250 - br, 250 - br, 250 - br);
    }
  }
};

void bouncyBall(bool &on, uint8_t &brightnessStand) {
  Ball bouncyBoi;

  while(on) {
    bouncyBoi.go();
    buttonCheck(on, brightnessStand);
    strip.show();
    delay(1);
  }
}
