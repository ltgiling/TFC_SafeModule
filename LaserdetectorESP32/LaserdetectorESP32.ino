#include "OOCSI.h"
#include <Adafruit_NeoPixel.h>
#include "esp32-hal-ledc.h"

//define the network data for the ESP32, the oocsi server and our oocsi name on that server.
OOCSI oocsi = OOCSI();
const char* ssid = "********";
const char* password = "********";
const char* OOCSIName = "SafeModuleChip";
const char* hostserver = "oocsi.id.tue.nl";

//define the ESP32 output pins and starting integers
const int Laser = 14;
int LaserOn = 0;
#define BUTTON_PIN 32
#define TIMER_WIDTH 16
bool oldState = HIGH;
int challenge = 0;

//Setting up the first sensor integers, checkpoints for the LEDs and the LED strip itself (yellow / green wires)
int Detector = 27;
int Timeval;
int val;
int set100 = 0;
int set200 = 0;
int set300 = 0;
int setfinal1 = 0;
int finalcheck1 = 0;
#define PIN 18
#define N_LEDS 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);

//Setting up the second sensor integers, checkpoints for the LEDs and the LED strip itself (orange / purple wires)
int Detector2 = 26;
int Timeval2;
int val2;
int set1002 = 0;
int set2002 = 0;
int set3002 = 0;
int setfinal2 = 0;
int finalcheck2 = 0;
#define PIN2 19
#define N_LEDS2 3
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_LEDS2, PIN2, NEO_GRB + NEO_KHZ800);

//Setting up the third sensor integers, checkpoints for the LEDs and the LED strip itself (blue / grey wires)
int Detector3 = 25;
int Timeval3;
int val3;
int set1003 = 0;
int set2003 = 0;
int set3003 = 0;
int setfinal3 = 0;
int finalcheck3 = 0;
#define PIN3 21
#define N_LEDS3 3
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(N_LEDS3, PIN3, NEO_GRB + NEO_KHZ800);


void setup()
{
  Serial.begin (2400);
//start a new oocsi session on the channel 'SafeModule'.
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);
  oocsi.subscribe("SafeModule");

//define pinModes for the sensors, set up the channel frequency for the servo and start the LED strips.
  pinMode(Detector,  INPUT);
  pinMode(Detector2, INPUT);
  pinMode(Detector3, INPUT);
  pinMode(Laser, OUTPUT);
  digitalWrite(Laser, LOW);
  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(2, 1);   // GPIO 22 assigned to channel 1
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.setBrightness(25);
  strip2.begin();
  strip2.setBrightness(25);
  strip3.begin();
  strip3.setBrightness(25);
}

void loop() {
  //create an integer out of the laser sensor readings.
  val = digitalRead(Detector);
  val2 = digitalRead(Detector2);
  val3 = digitalRead(Detector3);

  //checks for 1st sensor to be activated.
  if (val == 1) {
    Timeval++;
    Serial.println(Timeval);
  }
  //if the 1st sensor isn't being activated, check whether it has already been completed.
  //if the 1st sensor hasn't been completed yet, reset all progress and the LED's to an off-state.
  if (val == 0) {
    delay(20);
      if (finalcheck1 == 0) {
        Timeval = 0;
        set100 = 0;
        set200 = 0;
        set300 = 0;
        setfinal1 = 0;
        strip.setPixelColor(0, 0, 0, 0);
        strip.setPixelColor(1, 0, 0, 0);
        strip.setPixelColor(2, 0, 0, 0);
        strip.show();
        delay(2);
      }
    }

  //checks for 2nd sensor to be activated
  if (val2 == 1) {
    Timeval2++;
    Serial.println(Timeval2);
    delay(2);
  }
  //if the 2nd sensor isn't being activated, check whether it has already been completed.
  //if the 2nd sensor hasn't been completed yet, reset all progress and the LED's to an off-state.
  if (val2 == 0){
    if (finalcheck2 == 0) {
      Timeval2 = 0;
      set1002 = 0;
      set2002 = 0;
      set3002 = 0;
      setfinal2 = 0;
      strip2.setPixelColor(0, 0, 0, 0);
      strip2.setPixelColor(1, 0, 0, 0);
      strip2.setPixelColor(2, 0, 0, 0);
      strip2.show();
      delay(2);
    }
  }

  //checks for 3rd sensor to be activated 
  if (val3 == 1) {
    Timeval3++;
    Serial.println(Timeval3);
  }
  
  //if the 3rd sensor isn't being activated, check whether it has already been completed.
  //if the 3rd sensor hasn't been completed yet, reset all progress and the LED's to an off-state.
  if (val3 == 0) {
    if (finalcheck3 == 0) {
      Timeval3 = 0;
      set1003 = 0;
      set2003 = 0;
      set3003 = 0;
      setfinal3 = 0;
      strip3.setPixelColor(0, 0, 0, 0);
      strip3.setPixelColor(1, 0, 0, 0);
      strip3.setPixelColor(2, 0, 0, 0);
      strip3.show();
      delay(2);
    }
  }

  //thresholds for the LED settings for the 1st LED strip
  if (Timeval > 15 && set100 == 0) {
    Serial.println("check1");
    if (!set100) {
      strip.setPixelColor(0, 255, 0, 0);
      strip.show();
      set100 = 1;
      Serial.println("check2");
    }
  }
  if (Timeval > 30 && set200 == 0) {
    Serial.println("check3");
    if (!set200) {
      strip.setPixelColor(1, 255, 200, 0);
      strip.show();
      set200 = 1;
      Serial.println("check4");
    }
  }
  if (Timeval > 45 && set300 == 0) {
    Serial.println("check5");
    if (!set300) {
      strip.setPixelColor(2, 0, 255, 0);
      strip.show();
      set300 = 1;
      finalcheck1 = 1;
      Serial.println("check6");
    }
  }

  //thresholds for the LED settings for 2nd LED strip
  if (Timeval2 > 15 && set1002 == 0) {
    Serial.println("check2.1");
    if (!set1002) {
      strip2.setPixelColor(0, 255, 0, 0);
      strip2.show();
      set1002 = 1;
      Serial.println("check2.2");
    }
  }
  if (Timeval2 > 30 && set2002 == 0) {
    Serial.println("check2.3");
    if (!set2002) {
      strip2.setPixelColor(1, 255, 200, 0);
      strip2.show();
      set2002 = 1;
      Serial.println("check2.4");
    }
  }
  if (Timeval2 > 45 && set3002 == 0) {
    Serial.println("check2.5");
    if (!set3002) {
      strip2.setPixelColor(2, 0, 255, 0);
      strip2.show();
      set3002 = 1;
      finalcheck2 = 1;
      Serial.println("check2.6");
    }
  }

  //thresholds for the LED settings for 3rd LED strip
  if (Timeval3 > 15 && set1003 == 0) {
    Serial.println("check3.1");
    if (!set1003) {
      strip3.setPixelColor(0, 255, 0, 0);
      strip3.show();
      set1003 = 1;
      Serial.println("check3.2");
    }
  }
  if (Timeval3 > 30 && set2003 == 0) {
    Serial.println("check3.3");
    if (!set2003) {
      strip3.setPixelColor(1, 255, 200, 0);
      strip3.show();
      set2003 = 1;
      Serial.println("check3.4");
    }
  }
  if (Timeval3 > 45 && set3003 == 0) {
    Serial.println("check3.5");
    if (!set3003) {
      strip3.setPixelColor(2, 0, 255, 0);
      strip3.show();
      set3003 = 1;
      finalcheck3 = 1;
      Serial.println("check3.6");
    }
  }

  //checks for each individual sensor whether that particular sensor has been completed.
  //if so, set integer to deny the possibility for the program to reset the progress of a sensor.
  if (finalcheck1 == 1 && setfinal1 == 0) {
    setfinal1 = 1;
    Serial.println("testfinal");
  }
  if (finalcheck2 == 1 && setfinal2 == 0) {
    setfinal2 = 1;
    Serial.println("testfinal2");
  }
  if (finalcheck3 == 1 && setfinal3 == 0) {
    setfinal3 = 1;
    Serial.println("testfinal3");
  }
  //if all three thresholds have been met, module is completed.
  //an integer will be sent on to the network for other modules to check whether our module is complete.
  //the safe will also open itself, which allows for other modules to make things accessable which were previously inaccessable.
  if (setfinal1 == 1 && setfinal2 == 1 && setfinal3 == 1) {
    challenge = 1;
    delay(200);
    oocsi.newMessage("testchannel");
    oocsi.addInt("completeSafe", 1);
    oocsi.sendMessage();
    int SafeLaser = 0;
    oocsi.check();
    for (int i = 7000 ; i < 7010 ; i = i + 100)
    {
      ledcWrite(1, i);       // sweep servo 1
      if (i > 7000) {
        break;
      }
      delay(50);
    }
    Serial.println("congrats, the module has been completed!");
    delay(1000);
  }
  //A hidden physical button inside the safe allows for the module to be reset to it's base state.
  //This allows for an easy way to reset the module for another round without having to restart the oocsi subscription. 
  bool newState = digitalRead(BUTTON_PIN);
  if (newState == LOW && oldState == HIGH) {
    delay(20);
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      for (int i = 3000 ; i < 3010 ; i = i + 100)
      {
        ledcWrite(1, i);       // sweep servo 1
        if (i > 301000) {
          break;
        }
        delay(50);
      }
      Serial.println("resetting module.....");
      challenge = 0;
      finalcheck1 = 0;
      finalcheck2 = 0;
      finalcheck3 = 0;
      setfinal1 = 0;
      setfinal2 = 0;
      setfinal3 = 0;
      LaserOn = 0;
      digitalWrite(Laser, LOW);
      oocsi.newMessage("SafeModule");
      oocsi.addInt("completeSafe", 0);
      oocsi.sendMessage();
      int SafeLaser = 0;
      oocsi.check();
      delay(1000);
    }
  }
  //check whether oocsi code has been sent to this module.
  oocsi.check();
}
//if the laser isn't already on, check the network if the integer 'safeLaser' is present.
//if this integer is present and has the value '1', turn on the laser and stop this check.
void processOOCSI() {
  if (LaserOn == 0) {
    int SafeLaser = oocsi.getInt("safeLaser", -200);
    Serial.print(oocsi.getInt("safeLaser", -200));
    Serial.println();
    if (SafeLaser == 1) {
      digitalWrite(Laser, HIGH);
      LaserOn = 1;
      delay(10);
    }
  }
}

