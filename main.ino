

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  strip.begin();
  strip.setBrightness(255);
  strip.clear();
  strip.show();
//  startUp();
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
}


int buttonState1 = digitalRead(button1);
int buttonState2 = digitalRead(button2);
int lightShow = 1;
uint8_t brightnessStand = 255; //Brightness standard, each display shares/changes this specific number via |buttonCheck|.
bool on = true;

void loop() {
  int buttonState1 = digitalRead(button1);
  int buttonState2 = digitalRead(button2);

  
  if(buttonState1 == HIGH && buttonState2 == HIGH) {
    delay(1000);
    if(buttonState1 == HIGH && buttonState2 == HIGH) {
      lightShow += 1;
      strip.clear();
    }
  }

  //All displays share at least two of the same variables: |on| |brightnessStand|.
  //Each display relies on |on| to keep the loop going. They also take |brightnessStand|
  //only for the purpose of changing it with |buttonCheck|. |brightnessStand| is never 
  //manipulated or used in actual logic of a display.
  
  switch(lightShow) {
   case 2:
    lightAll(on, brightnessStand);
    on = true;
    break;
   case 3:
    //Number of leds to randomly flicker, not the size of the area you want them in. 
    //The last variable is the speed of the flickering. 
    randColorFlicker(on, brightnessStand, LED_COUNT, 0); 
    on = true;
    break;
   case 1:
    flame(on, brightnessStand, LED_COUNT, 30);
    on = true;
    break;
   case 4:
    recursion(on, brightnessStand, 0);
    on = true;
    break;
   case 5:
    blob(on, brightnessStand, LED_COUNT);
    on = true;
    lightShow = 0;
    break;
   }
}
