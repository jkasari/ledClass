void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();
  startUp(8); //8 is soft white on the color code index.
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}

int buttonState1 = digitalRead(button1);
int buttonState2 = digitalRead(button2);
int lightShow = 1;
uint8_t brightnessStand = 200;
          // Brightness standard, each display shares/changes this specific
         // number via |buttonCheck|.
bool on = true;

void loop() {
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);

  if (!on) {
    lightShow += 1;
    strip.clear();
    on = true;
  }

  // All displays share at least two of the same variables: |on|
  // |brightnessStand|. Each display relies on |on| to keep the loop going. They
  // also take |brightnessStand| only for the purpose of changing it with
  // |buttonCheck|. |brightnessStand| is never manipulated or used in actual
  // logic of a display.

  switch (lightShow) {
  case 1:
    lightAll(on, brightnessStand, 8);
    break;
  case 2:
    // Number of leds to randomly flicker, not the size of the area you want
    // them in.
    flickerRandDots(on, brightnessStand, LED_COUNT / 3);
    break;
  case 3:
    Flame(on, brightnessStand);
    break;
  case 4:
    heatDiss(on, brightnessStand);
    break;
  case 5:
    bouncyBall(on, brightnessStand);
    break;
  case 6:
    stars(on, brightnessStand);
    break;
  case 7:
    lavaLamp(on, brightnessStand);
    break;
  case 8: 
    randColorFade(on, brightnessStand);
    break;
  case 9:
    heatBounce(on, brightnessStand);
    lightShow = 0;
    break;
  }
}
