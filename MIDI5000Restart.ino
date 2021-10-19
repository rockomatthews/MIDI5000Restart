#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#define SerialMon Serial
#define APPLEMIDI_DEBUG SerialMon
#include <AppleMIDI.h>

char ssid[] = "ATT4K"; //  your network SSID (name)
char pass[] = "passpasspass";    // your network password (use for WPA, or use as key for WEP)

// set pin numbers
const int button1_Pin = 4;     // the number of the pushbutton pin
const int led1_Pin =  16;      // the number of the LED pin

const int DEBOUNCE_DELAY = 50;   // the debounce time; increase if the output flickers

// -----------------------------------------------------------------------------
// button 1 states
// -----------------------------------------------------------------------------
int lastSteady1State = LOW;       // the previous steady state from the input pin
int lastFlickerable1State = LOW;  // the previous flickerable state from the input pin
int led1State = LOW;     // the current state of LED
int lastButton1State;    // the previous state of button
int currentButton1State; // the current state of button

// -----------------------------------------------------------------------------
// more variables
// -----------------------------------------------------------------------------
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long t0 = millis();
int8_t isConnected = 0;


APPLEMIDI_CREATE_DEFAULTSESSION_INSTANCE();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------


void setup() {
  Serial.begin(115200);  
  
  pinMode(button1_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led1_Pin, OUTPUT);          // set arduino pin to output mode

  currentButton1State = digitalRead(button1_Pin);


  DBG_SETUP(115200);
  DBG("Booting");
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DBG("Establishing connection to WiFi..");
  }
  DBG("Connected to network");

  DBG(F("OK, now make sure you an rtpMIDI session that is Enabled"));
  DBG(F("Add device named Arduino with Host"), WiFi.localIP(), "Port", AppleMIDI.getPort(), "(Name", AppleMIDI.getName(), ")");
  DBG(F("Select and then press the Connect button"));
  DBG(F("Then open a MIDI listener and monitor incoming notes"));
  DBG(F("Listen to incoming MIDI commands"));

  MIDI.begin();

  AppleMIDI.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
    isConnected++;
    DBG(F("Connected to session"), ssrc, name);
  });
  AppleMIDI.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
    isConnected--;
    DBG(F("Disconnected"), ssrc);
  });
}

void loop() {

  // Listen to incoming notes
  MIDI.read();
  
// -----------------------------------------------------------------------------
// Button 1
// -----------------------------------------------------------------------------
   
   if (currentButton1State != lastFlickerable1State) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the the last flickerable state
      lastFlickerable1State = currentButton1State;
    }
    
    lastButton1State = currentButton1State;      // save the last state 
    currentButton1State = digitalRead(button1_Pin);
 
  if(lastButton1State == HIGH && currentButton1State == LOW) {
    Serial.println("The button is pressed");
    digitalRead(button1_Pin);
    led1State = LOW;
    digitalWrite(led1_Pin, led1State);
    MIDI.sendNoteOn(53, 110, 13);
    } else if (lastButton1State == LOW && currentButton1State == HIGH) {
      Serial.println("The button is released");
      led1State = !led1State;
      digitalWrite(led1_Pin, led1State);
      MIDI.sendNoteOff(53, 0, 13);
      // save the the last state
      lastButton1State = currentButton1State;
    }
}
