#include "OOCSI.h"
#include <Adafruit_NeoPixel.h>
#include "esp32-hal-ledc.h"

OOCSI oocsi = OOCSI();
const char* ssid = "TCLP";
const char* password = "Tclp@533112";
const char* OOCSIName = "SafeModuleChip";
const char* hostserver = "oocsi.id.tue.nl";


//servo & reset definitions
int Laser = 14;
#define BUTTON_PIN 32
#define TIMER_WIDTH 16
bool oldState = HIGH;
int challenge = 0;

//1st sensor (yellow)
int Detector = 27;
int Timeval;
int val;
int set100 = 0;
int set200 = 0;
int set300 = 0;
int setfinal1 = 0;
int finalcheck1 = 0;
//2nd sensor (purple)
int Detector2 = 26;
int Timeval2;
int val2;
int set1002 = 0;
int set2002 = 0;
int set3002 = 0;
int setfinal2 = 0;
int finalcheck2 = 0;
//3rd sensor (blue)
int Detector3 = 25;
int Timeval3;
int val3;
int set1003 = 0;
int set2003 = 0;
int set3003 = 0;
int setfinal3 = 0;
int finalcheck3 = 0;

//strip 1 (green)
#define PIN 18
#define N_LEDS 3
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
//strip 2 (orange)
#define PIN2 19
#define N_LEDS2 3
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_LEDS2, PIN2, NEO_GRB + NEO_KHZ800);
//strip 3 (grey)
#define PIN3 21
#define N_LEDS3 3
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(N_LEDS3, PIN3, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin (2400);
  oocsi.connect(OOCSIName, hostserver, ssid, password, processOOCSI);
  oocsi.subscribe("testchannel");

  pinMode(Detector,  INPUT);
  pinMode(Detector2, INPUT);
  pinMode(Detector3, INPUT);
  pinMode(Laser, OUTPUT);
  digitalWrite(Laser, LOW);
  ledcSetup(1, 50, TIMER_WIDTH); // channel 1, 50 Hz, 16-bit width
  ledcAttachPin(2, 1);   // GPIO 22 assigned to channel 1
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.setBrightness(30);
  strip2.begin();
  strip2.setBrightness(30);
  strip3.begin();
  strip3.setBrightness(30);
}

void loop() {
  //short general loop of some variables for the detectors
  val = digitalRead(Detector);
  val2 = digitalRead(Detector2);
  val3 = digitalRead(Detector3);
  if (challenge == 0) {
    //Serial.println("running");
    //delay(500);
  }
  //checks for 1st sensor
  if (val == 1) {
    Timeval++;
    Serial.println(Timeval);
  }
  if (val == 0) {
    delay(20);
    if (finalcheck1 == 0) {
      Timeval = 0;
      set100 = 0;
      set200 = 0;
      set300 = 0;
      setfinal1 = 0;
      uint32_t low = strip.Color(0, 0, 0);
      for (int i1 = 0; i1 < N_LEDS; i1++) {
        strip.setPixelColor(i1, low);
        strip.show();
      }
      //strip.setPixelColor(0, 0, 0, 0);
      //strip.setPixelColor(1, 0, 0, 0);
      //strip.setPixelColor(2, 0, 0, 0);
      //strip.show();
      delay(2);
    }
  }

  //checks for 2nd sensor
  if (val2 == 1) {
    Timeval2++;
    Serial.println(Timeval2);
  }
  if (val2 == 0) {
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

  //checks for 3rd sensor
  if (val3 == 1) {
    Timeval3++;
    Serial.println(Timeval3);
  }
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

  //thresholds for the pixel settings for 1st LED
  if (Timeval > 10 && set100 == 0) {
    Serial.println("check1");
    if (!set100) {
      strip.setPixelColor(0, 255, 0, 0);
      strip.show();
      set100 = 1;
      Serial.println("check2");
    }
  }
  if (Timeval > 20 && set200 == 0) {
    Serial.println("check3");
    if (!set200) {
      strip.setPixelColor(1, 255, 200, 0);
      strip.show();
      set200 = 1;
      Serial.println("check4");
    }
  }
  if (Timeval > 30 && set300 == 0) {
    Serial.println("check5");
    if (!set300) {
      strip.setPixelColor(2, 0, 255, 0);
      strip.show();
      set300 = 1;
      finalcheck1 = 1;
      Serial.println("check6");
    }
  }

  //thresholds for the pixel settings for 2nd LED
  if (Timeval2 > 10 && set1002 == 0) {
    Serial.println("check2.1");
    if (!set1002) {
      strip2.setPixelColor(0, 255, 0, 0);
      strip2.show();
      set1002 = 1;
      Serial.println("check2.2");
    }
  }
  if (Timeval2 > 20 && set2002 == 0) {
    Serial.println("check2.3");
    if (!set2002) {
      strip2.setPixelColor(1, 255, 200, 0);
      strip2.show();
      set2002 = 1;
      Serial.println("check2.4");
    }
  }
  if (Timeval2 > 30 && set3002 == 0) {
    Serial.println("check2.5");
    if (!set3002) {
      strip2.setPixelColor(2, 0, 255, 0);
      strip2.show();
      set3002 = 1;
      finalcheck2 = 1;
      Serial.println("check2.6");
    }
  }

  //thresholds for the pixel settings for 3rd LED
  if (Timeval3 > 10 && set1003 == 0) {
    Serial.println("check3.1");
    if (!set1003) {
      strip3.setPixelColor(0, 255, 0, 0);
      strip3.show();
      set1003 = 1;
      Serial.println("check3.2");
    }
  }
  if (Timeval3 > 20 && set2003 == 0) {
    Serial.println("check3.3");
    if (!set2003) {
      strip3.setPixelColor(1, 255, 200, 0);
      strip3.show();
      set2003 = 1;
      Serial.println("check3.4");
    }
  }
  if (Timeval3 > 30 && set3003 == 0) {
    Serial.println("check3.5");
    if (!set3003) {
      strip3.setPixelColor(2, 0, 255, 0);
      strip3.show();
      set3003 = 1;
      finalcheck3 = 1;
      Serial.println("check3.6");
    }
  }

  //final checks if all thresholds have been met
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
  //if all three thresholds have been met, module is completed
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
  //reset button to reset module
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
      digitalWrite(Laser, LOW);
      oocsi.newMessage("testchannel");
      oocsi.addInt("completeSafe", 0);
      oocsi.sendMessage();
      int SafeLaser = 0;
      oocsi.check();
      delay(1000);
    }
  }
  oocsi.check();
}
void processOOCSI() {
  int SafeLaser = oocsi.getInt("safeLaser", -200);
  Serial.print(oocsi.getInt("safeLaser", -200));
  Serial.println();
  if (SafeLaser == 1) {
    digitalWrite(Laser, HIGH);
    delay(10);
  }
}

