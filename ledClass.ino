#include <Adafruit_NeoPixel.h>
#ifndef LED
#define LED
#define LED_PIN 6
#define LED_COUNT 20

int buttonState = 0;
const uint8_t button1 = 9;
const uint8_t button2 = 3;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);


class led {

  //Welcome to the |led| class!
  //|led|'s each have 4 data memebers: loaction, colorCode, brightness, brightnessDir.
  //             Cool Stuff about the class,
  //|brightnessDir| allows the |led| to "remember" which direction it is currently going.
  //Whether this be a direction of brightness or a location. 
  //|colorCode| only takes a signle uint8_t to decide the color, it does the rest of the 
  //work for you. And allows you to change your color library indepent of any display
  //that uses it.
  //             Dumb stuff that I need to fix,
  //|ledUpDate| builds and lights its own led in adafruits strip library. As a result
  //|ledUpDate| needs to be called for EVERY function in the class. 
  //EXCEPT for |setColor|, why?? I have no clue, something about the switch statement
  //does not like calling |ledUpDate|. So if you call |setColor| you need to either 
  //call |ledUpDate| or another function in the class after calling |setColor|.

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

// Set the color and brightness. This uses it's own color code where 0 = white etc.
// Brightness is taken as just a regular 0-255 value.
  setColor(uint8_t, uint8_t);


// Increases brightness from it's current brightness by any given value.
  increaseBrightness(uint8_t);


// Decreases brightness from it's current brightness by any given value.
  decreaseBrightness(uint8_t);


  uint8_t getBrightVal();
  uint8_t getColorCode();

// This changes the bool value in a |led|, aka the direction of movement of that |led|.
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

led::ledUpdate(void) {
  setColor(colorCode, brightness);
  strip.setPixelColor(location, color);
}

int32_t led::getLocation(void) { return location; }

uint32_t led::getBrightness(void) { return brightness; }

led::movePos(uint32_t distanceToMove) {
  if ((location + distanceToMove) > LED_COUNT) {
    location = (location + distanceToMove) - LED_COUNT;
    return;
  }
  location += distanceToMove;
  ledUpdate();
}

led::setLocation(uint32_t newLocation) {
  if (newLocation < 0) {
    newLocation = 0;
    return;
  }
  if (newLocation >= LED_COUNT) {
    newLocation = LED_COUNT - 1;
    return;
  }
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
    color = strip.Color((brightness / 10), brightness, 0);
  }
}

led::increaseBrightness(uint8_t adjustBy) {
  if(brightness + adjustBy >= 255) {
    brightness = 255;
    return;
  }
  brightness += adjustBy;
  ledUpdate();
}

led::decreaseBrightness(uint8_t adjustBy) {
  if(brightness - adjustBy <= 0) {
    brightness = 0;
    return;
  }
  
  brightness -= adjustBy;
  ledUpdate();
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

// Gives you the direction of a |led|.
bool led::getBrightnessDir(void) {
  if (brightnessDir) {
    return true;
  } else {
    return false;
  }
}

//==============================================================buttonCheck

//checks to see the current state of all the possible button presses
void buttonCheck(bool &on, uint8_t &brightnessStand) {
  if(digitalRead(button1) == HIGH && digitalRead(button2) == HIGH) {
      delay(500);
      if(digitalRead(button1) == HIGH && digitalRead(button2) == HIGH) {
        on = false;
      }
    }
    
    if(digitalRead(button1) == HIGH) {
      delay(50);
      brightnessStand += 5;
      if(brightnessStand >= 250) {
        brightnessStand = 250;
      }
    }
    if(digitalRead(button2) == HIGH) {
      delay(50);
      brightnessStand -= 5;
      if(brightnessStand < 20) {
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


// Lights up the whole strip any single color you want
void lightAll(uint32_t color) {
  for (int i = 0; i < 20; ++i) {
    strip.setPixelColor(i, color);
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

// THIS ONE TAKES BOOL FOOL
// use this for a light feature as it will stay on until a button is pressed
void lightAll(bool &on, uint8_t &brightness) {
 while(on) {
    for (int i = 0; i < 20; ++i) {
      strip.setPixelColor(i, 255, 255, 255);
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
  while(on) {
//    buttonCheck(on, brightness);
    randomPopulate(pixelNum, red, DELAY);
 //   buttonCheck(on, brightness);
    randomVacate(pixelNum, DELAY);
  }
}


//==============================================================randColorFlicker


// slowly turns on and off leds at a random location and color
// takes a bool to turn on the loop
// |ledArrSize| dictates how many leds to flicker
// |flickRate| is the rate at which they flicker
void randColorFlicker(bool &on, uint8_t &brightnessStand, uint32_t ledArrSize, uint8_t flickRate) {

  // check to make sure that you are not overloading the pixel strip
  // if you do it will just light one led as an indication you are feeding it to
  // big a number
  if (ledArrSize >= LED_COUNT) {
    ledArrSize = LED_COUNT - 1;
  }

  // create an array of |led|s and an array of random locations for those |led|s to go.
  led ledArr[ledArrSize];
  uint32_t randomArr[LED_COUNT];
  randomArrMaker(LED_COUNT, randomArr);

  // go through the led array and randomly choose color/location/direction
  for (int i = 0; i < ledArrSize; ++i) {
    ledArr[i].setLocation(randomArr[i]);
    ledArr[i].setColor(random(1, 8), random(250));
    if ((i % 2) == 1) {
      ledArr[i].setBrightnessDir(true);
    } else {
      ledArr[i].setBrightnessDir(false);
    }
  }

  // cycle through the led array and flicker the leds
  while(on) {
    for (int i = 0; i < ledArrSize; ++i) {
      int color = ledArr[i].getColorCode();
      int brightness = ledArr[i].getBrightVal();
      // check direction to see if the led needs to increase in brightness
      if (ledArr[i].getBrightnessDir()) {
          ledArr[i].increaseBrightness(1);
        if (ledArr[i].getBrightVal() >= 255) {
          ledArr[i].setBrightnessDir(false);
        }
        ledArr[i].ledUpdate();
      }
      // check direction to see if the led needs to decrease in brightness
      if (!ledArr[i].getBrightnessDir()) {
          ledArr[i].decreaseBrightness(1);
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
          ledArr[i].setColor(random(1, 8), brightness);
          ledArr[i].setLocation(newRandLoc);
        }
        ledArr[i].ledUpdate();
      }
    }
    strip.show();
    delay(flickRate);
    buttonCheck(on, brightnessStand);
    strip.clear();
  }
}


//==============================================================fadeOnOff


void fadeOnOff(uint8_t colorCode, uint8_t fadeRate, uint8_t brightnessMax) {

   if(brightnessMax > 255) {
     brightnessMax = 255;
   }
  
   uint8_t brightness = 0;
   led ledToFade(0, colorCode, brightness, true);

   while(brightness < brightnessMax) {
     for(int i = 0; i < LED_COUNT; ++i) {
       ledToFade.setLocation(i);
       ledToFade.setColor(colorCode, brightness); 
     }
     strip.show();
     delay(fadeRate);
     brightness += 5;
     strip.show();
   }
   while(brightness >  0) {
     for(int i = 0; i < LED_COUNT; ++i) {
     ledToFade.setLocation(i);
     ledToFade.setColor(colorCode, brightness); 
     }
     strip.show();
     delay(fadeRate);
     brightness -= 5;
     strip.show();
   }
}

void startUp(void) {

    fadeOnOff(0, 0, 90);
  fadeOnOff(0, 0, 100);
  delay(30);
  fadeOnOff(0, 3, 120);
  delay(50);
  fadeOnOff(0, 0, 200);
  delay(300);
  fadeOnOff(0, 2, 100);
  lightAll(255, 255, 255);
  delay(100);
  fadeOnOff(0, 3, 255);
  lightAll(255, 255, 255);
}



//==============================================================Flame


// flickers the strip in verying shades of red and orange
void flame(bool &on, uint8_t &brightnessStand, uint32_t pixelNum, uint32_t flickSpeed) {
  if(pixelNum > LED_COUNT) {
    pixelNum = LED_COUNT - 1;
  }
  
  led ledArr[pixelNum];
  uint32_t randomArr[LED_COUNT];
  randomArrMaker(LED_COUNT, randomArr);

  for(int i = 0; i < pixelNum; ++i) {
    if(i < 10) {
      ledArr[i].setColor(7, random(100, 255));
      ledArr[i].setLocation(randomArr[i]);
      ledArr[i].setBrightnessDir(true);
    } else {
      ledArr[i].setColor(1, random(100, 255));
      ledArr[i].setLocation(randomArr[i]);
      ledArr[i].setBrightnessDir(false);
    }
  }

  while(on) {
    for(int i = 0; i < pixelNum; i++) {
      if(ledArr[i].getBrightnessDir()) {
        ledArr[i].increaseBrightness(random(25));
        if(ledArr[i].getBrightVal() >= random(150, 255)) {
          ledArr[i].setBrightnessDir(false);
        }
        ledArr[i].ledUpdate();
      }
      if(!ledArr[i].getBrightnessDir()) {
        ledArr[i].decreaseBrightness(random(25));
        if(ledArr[i].getBrightVal() <= random(20, 60)) {
          ledArr[i].setBrightnessDir(true);
        }
        ledArr[i].ledUpdate();
      }
    }
    strip.show();
    delay(flickSpeed);
    buttonCheck(on, brightnessStand);
    strip.clear();
  }
}


//==============================================================blob


//sends a blob across the strip
void blob(bool &on, uint8_t &brightnessStand, uint32_t pixelNum) {
  
  led blob(0, 1, 250, true);
  led blob2(0, 1, 250, true);
  led blob3(0, 1, 0, true);
  int cycle = 0;
  int location = 0;
  int fade;
    
  while(on) {
    cycle++;
    uint8_t cycleCount = cycle % (pixelNum * 2);

    if(cycleCount < pixelNum) {
      location++;
      while(blob.getBrightVal() > 0) {
        
        blob.decreaseBrightness(25);
        blob2.ledUpdate();

        if(blob.getBrightVal() < 200) {
        blob3.increaseBrightness(25);
        }
        strip.show();
        delay(10);
      }
      strip.clear();

      blob.setColor(1, 250);
      blob2.setColor(1, 250);
      blob3.setColor(1, 0);
      
      blob.setLocation(location - 1);
      blob2.setLocation(location);
      blob3.setLocation(location + 1);
      strip.show();
    }
    if(cycleCount >= pixelNum) {
      location--;

      while(blob3.getBrightVal() > 0) {
        blob2.ledUpdate();
        blob3.decreaseBrightness(25);

        if(blob3.getBrightVal() < 200) {
        blob.increaseBrightness(25);
        }
        strip.show();
        delay(10);
      }
      strip.clear();

      blob.setColor(1, 0);
      blob2.setColor(1, 250);
      blob3.setColor(1, 250);
      
      blob.setLocation(location - 1);
      blob2.setLocation(location);
      blob3.setLocation(location + 1);
      strip.show();
    }
    strip.clear();
    buttonCheck(on, brightnessStand);
  }
}


//==============================================================recursion


void recursion(bool &on, uint8_t &brightnessStand, int32_t location) {
  
  uint32_t red = strip.Color(0, 255, 0);
  uint32_t blue = strip.Color(0, 0, 255);
  uint32_t orange = strip.Color(150, 255, 0);
  uint32_t teal = strip.Color(255, 0, 255);


  if(location > LED_COUNT / 4) {
    return;
  }
  int DELAY = 30;
  strip.setPixelColor(location, red); //Bottom of the strip.
  strip.setPixelColor(location + LED_COUNT / 4, teal); //A quarter of the way up the strip.
  strip.setPixelColor(location + LED_COUNT / 2, red); //Half way up the strip.
  strip.setPixelColor((location + 3 * (LED_COUNT / 4)), teal); //Three quarters of the way up the strip.
  strip.show();
  delay(DELAY);
//  strip.clear();
  recursion(on, brightnessStand, location + 1);
  strip.setPixelColor(LED_COUNT / 2 - location, red); //Bottom of the strip.
  strip.setPixelColor(LED_COUNT / 2 - location + LED_COUNT / 4, teal); //A quarter of the way up the strip.
  strip.setPixelColor(LED_COUNT / 2 - location + LED_COUNT / 2, red); //Half way up the strip.
  strip.setPixelColor((LED_COUNT / 2 - location + 3 * (LED_COUNT / 4)) % LED_COUNT, teal); //Three quarters of the way up the strip.
  strip.show();
  delay(DELAY);
  buttonCheck(on, brightnessStand);
//  strip.clear();

}
