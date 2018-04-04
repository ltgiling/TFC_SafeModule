import nl.tue.id.oocsi.*;
import nl.tue.id.oocsi.client.services.*;

OOCSI oocsi;
int step = 0;
int sentmessage = 0;
int safeCheck = 0;
String message = "c b z p";
String MagnetMessage;
  
void setup() {
  size(200, 200);
  noStroke();
  OOCSI oocsi = new OOCSI(this, "SafeLevelModule", "oocsi.id.tue.nl");

  //subscribe to the relevant channels
  oocsi.subscribe("SafeModule");
  oocsi.subscribe("thePlate");
  oocsi.subscribe("thePlateAPI");
  oocsi.subscribe("group10");
  oocsi.subscribe("Painting");
  
  levelReset();
}

void thePlate() {
  if(step == 0){
    String message = "c b z p   ";
    int result;
    println("");
    println("Sending to the plate: " + message);
    OOCSICall call = oocsi.call("thePlateHandler", 1000).data("stringCode", message);
    call.sendAndWait();
    if (call.hasResponse()) {
      result = call.getFirstResponse().getInt("result", 0);
      println("Your message has been received succesfully and will be outputted" + result);
      step = 1;
    }
  }
}  

void Painting(OOCSIEvent event) {
  if(step == 1){
    boolean Received = event.getBoolean("Received", false);
    if(Received == true){
      step = 2;
      oocsi.channel("thePlateAPI").data("stringCode", "").send();
      oocsi.channel("thePlate").data("stringCode", "").send();
    }
  }
}

void SafeModule(OOCSIEvent event) {
  if(step == 2){
    if(sentmessage == 0){
      oocsi.channel("SafeModule").data("safeLaser", 1).send();        
      sentmessage = 1;       
      delay(200);
    }  
  }
  safeCheck = event.getInt("completeSafe", 0);
  if(safeCheck == 1) {
    step = 3;
  }
}

void MagnetLock(OOCSIEvent event) {
  if(step == 3){
  String resp = oocsi.call("keypadSet", 200).data("code", "0127").sendAndWait().getFirstResponse().getString("result");
  println();
  println(resp);
  }
  MagnetMessage = event.getString("type");
  if(MagnetMessage == "success"){
  step = 4;
  print("well done the module is completed");
  }  
}

void levelReset() {
 step = 0;
}