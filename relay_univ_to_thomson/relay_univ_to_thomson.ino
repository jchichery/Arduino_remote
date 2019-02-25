#include <IRremote.h>

int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

unsigned long conv;

// Dumps out the decode_results structure.
// Call this after IRrecv::decode()
void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.println("Could not decode message");
  } 
  else {
    if (results->decode_type == NEC) {
      Serial.print("Decoded NEC: ");
    } 
    else if (results->decode_type == SONY) {
      Serial.print("Decoded SONY: ");
    } 
    else if (results->decode_type == THOMSON) {
      Serial.print("Decoded THOMSON: ");
    }
    else if (results->decode_type == RC5) {
      Serial.print("Decoded RC5: ");
    } 
    else if (results->decode_type == RC6) {
      Serial.print("Decoded RC6: ");
    }
    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
  }
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}

/*
bool converte_to_THOMSON(const decode_results results, unsigned long *converted)
{
  switch ( results.value ) {
  case 0x:      // On / Off
    *converted = 0xF2A0D5;
    break;
  case 0x:      // Source
    *converted = 0xFA305C;
    break;
  case 0x:      // List
    *converted = 0xF6109E;
    break;
  case 0x:      // TV
    *converted = 0xF3A0C5;
    break;
  case 0x:      // DVD
    *converted = 0xFE7018;
    break;
  case 0x:      // 1
    *converted = 0xF310CE;
    break;
  case 0x:      // 2
    *converted = 0xF320CD;
    break;
  case 0x:      // 3
    *converted = 0xF330CC;
    break;
  case 0x:      // 4
    *converted = 0xF340CB;
    break;
  case 0x:      // 5
    *converted = 0xF350CA;
    break;
  case 0x:      // 6
    *converted = 0xF360C9;
    break;
  case 0x:      // 7
    *converted = 0xF370C8;
    break;
  case 0x:      // 8
    *converted = 0xF380C7;
    break;
  case 0x:      // 9
    *converted = 0xF390C6;
    break;
  case 0x:      // AV
    *converted = 0x;
    break;
  case 0x:      // 0/--
    *converted = 0x;
    break;
  case 0x:      // FAV
    *converted = 0x;
    break;
  case 0x:      // up
    *converted = 0x;
    break;
  case 0x:      // down
    *converted = 0x;
    break;
  case 0x:      // right
    *converted = 0x;
    break;
  case 0x:      // left
    *converted = 0x;
    break;
  case 0x:      // ok
    *converted = 0x;
    break;
  case 0x:      // guide
    *converted = 0x;
    break;
  case 0x:      // return
    *converted = 0x;
    break;
  case 0x:      // info
    *converted = 0x;
    break;
  case 0x:      // presets
    *converted = 0x;
    break;
  case 0x:      // mute
    *converted = 0x;
    break;
  case 0x:      // vol +
    *converted = 0x;
    break;
  case 0x:      // vol -
    *converted = 0x;
    break;
  case 0x:      // pr + 
    *converted = 0x;
    break;
  case 0x:      // pr - 
    *converted = 0x;
    break;
  case 0x:      // rouge
    *converted = 0x;
    break;
  case 0x:      // vert
    *converted = 0x;
    break;
  case 0x:      // jaune
    *converted = 0x;
    break;
  case 0x:      // bleu
    *converted = 0x;
    break;
  case 0x:      // violet
    *converted = 0x;
    break;
  case 0x:      // STOP
    *converted = 0x;
    break;
  case 0x:      // ???
    *converted = 0x;
    break;
  case 0x:      // ZOOM
    *converted = 0x;
    break;
  case 0x:      // Media
    *converted = 0x;
    break;
  case 0x:      // <<
    *converted = 0x;
    break;
  case 0x:      // play
    *converted = 0x;
    break;
  case 0x:      // >>
    *converted = 0x;
    break;
  case 0x:      // T-LINK
    *converted = 0x;
    break;
  case 0x:      // enregistre
    *converted = 0x;
    break;
  case 0x:      // arret
    *converted = 0x;
    break;
  case 0x:      // pause
    *converted = 0x;
    break;
 
  default:
    Serial.print("Je ne connais pas ce code de touche: ");
    Serial.println(results.value, HEX);
    return false;
    break;
  }
  return true;
}
*/



void setup()
{
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    // irsend.sendNEC(0xF7C03F, 32);
    // delay(200);
    // dump(&results);
    // Serial.println(results.value, HEX);
    // Serial.println("______________");
    // irrecv.resume(); // Receive the next value

    converte_to_THOMSON(results, conv);
    irsend.sendTHOMSON(conv, 24);
    irrecv.enableIRIn();
  }
  delay(100);
}