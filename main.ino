

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  strip.begin();
  strip.setBrightness(50);
  strip.show();
}

void loop() {
  bool on = true;
  randColorFlicker(on, 19, 1);
}
