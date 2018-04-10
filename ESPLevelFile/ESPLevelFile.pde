import nl.tue.id.oocsi.*;
import nl.tue.id.oocsi.client.services.*;
import java.util.*;
import java.io.*;

OOCSI oocsi;

//set integers to be used to check level progress throughout the challenge
int step = 0;
int sentmessage = 0;
int safeCheck = 0;
String MagnetMessage;

void setup() {
  size(200, 200);
  noStroke();
  //start a new oocsi session under the name SafeLevelModule
  OOCSI oocsi = new OOCSI(this, "SafeLevelModule", "oocsi.id.tue.nl");

  //subscribe to the relevant channels for our level
  oocsi.subscribe("SafeModule");
  oocsi.subscribe("group10");
  oocsi.subscribe("Painting");
  oocsi.subscribe("theplate");
  thePlate();
}

void thePlate() {
  
  if(step == 0){
    //send a string to the thePlate module which they will receive to have their module vibrate this string.
    String string = "c b z p ";
    int result;
    println("");
    println("Sending to the plate: " + string);
    delay(3000);
    oocsi.channel("theplate").data("stringCode", string).send();
    OOCSICall call = oocsi.call("thePlateHandler").data("stringCode", string);
    call.sendAndWait();
    //if the thePlate module receives this string succesfully and starts vibrating our string, proceed to the next step.
    if (call.hasResponse()) {
    result = call.getFirstResponse().getInt("result", 0);
    println("Your message has been received succesfully and will be outputted " + result);
    step = 1;
  }
  }
}

void Painting(OOCSIEvent event) {
  /*if the thePlate module is vibrating, start checking whether the Painting module is sending an output. Unfortunately, 
  this module doesn't allow us to send them the order in which we would like the buttons to be pressed (the order in 
  which the thePlate module vibrates), so we had to manually give them the order which we deemed correct and would wait 
  for the Painting module to send a boolean being TRUE or FALSE.*/
  if(step == 1){
    boolean Received = event.getBoolean("Received", false);
    //if the Painting module is finished correctly, stop the thePlate module from buzzing and proceed to the next step.
    if(Received == true){
      step = 2;
      oocsi.channel("theplate").data("stringCode", "").send();
    }
  }
}

void SafeModule(OOCSIEvent event) {
  //if the painting was finished correctly, turn on the laser of the safe.
  if(step == 2){
    if(sentmessage == 0){
      oocsi.channel("SafeModule").data("safeLaser", 1).send();        
      sentmessage = 1;       
      delay(200);
    }  
  }
  safeCheck = event.getInt("completeSafe", 0);
  //if the safe module is completed correctly, the safe opens and the level proceeds to the next step.
  if(safeCheck == 1) {
    step = 3;
  }
}

void handleOOCSIEvent(OOCSIEvent event) {
  //if the safe module is open, a string is sent to the keypad module which indicates which magnets need to be turned.
  //if this is done correctly, we get a 'result' string to verify.
  if(step == 3){
  String resp = oocsi.call("keypadSet", 200).data("code", "3459").sendAndWait().getFirstResponse().getString("result");
  println();
  println(resp);
  }
  MagnetMessage = event.getString("type");
  //get the resulting string from the keypad module. 
  //If this 'type' string has the value 'success', the module is completed and the door is opened.
  if(MagnetMessage.equals("success")){
  step = 4;
  print("well done the module is completed");
  }  
}

void levelReset() {
 step = 0;
}
