import nl.tue.id.oocsi.*;
import nl.tue.id.oocsi.client.services.*;

OOCSI oocsi;
int step = 0;
int sentmessage = 0;
int safecheck = 0;
String message = "c b z p";
String MagnetMessage;
  
void setup() {
  size(200, 200);
  noStroke();
  OOCSI oocsi = new OOCSI(this, "SafeLevelModule", "oocsi.id.tue.nl");

  //subscribe to the relevant channels
  oocsi.subscribe("SafeModule");
  oocsi.subscribe("LE_level_controller_group11_1");
  oocsi.subscribe("ThePlate");
  oocsi.subscribe("group10");
  
  levelReset();
}

void PaintingAndPlate(OOCSIEvent event) {
  
  oocsi.channel("thePlateHandler").data("string", message).send();

//  if(event.has("right code")){
//    step = 1;
//  }
}

void Safe(OOCSIEvent event) {
  if(step == 1){
    if(sentmessage == 0){
      oocsi.channel("SafeModule").data("safeLaser", 1).send();        
      sentmessage = 1;       
      delay(200);
    }  
  }
  safecheck = event.getInt("completeSafe", 0);
  if(safecheck == 1) {
    step = 2;
  }
}

void MagnetLock(OOCSIEvent event) {
  if(step == 2){
  String resp = oocsi.call("set", 200).data("code", "0127").sendAndWait().getFirstResponse().getString("result");
  println();
  println(resp);
  }
  MagnetMessage = event.getString("type");
  if(MagnetMessage == "success"){
  step = 3;
  print("well done the module is completed");
  }  
}

void levelReset() {
 step = 0;
}
