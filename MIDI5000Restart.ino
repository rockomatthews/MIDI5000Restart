#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

#define SerialMon Serial
#define APPLEMIDI_DEBUG SerialMon
#include <AppleMIDI.h>

char ssid[] = "ATT4K"; //  your network SSID (name)
char pass[] = "passpasspass";    // your network password (use for WPA, or use as key for WEP)

// set pin numbers
const int button0_Pin = 14;     // the number of the pushbutton pin
const int led0_Pin =  15;      // the number of the LED pin
const int button1_Pin = 4;     // the number of the pushbutton pin
const int led1_Pin =  16;      // the number of the LED pin
const int button2_Pin = 22;     // the number of the pushbutton pin
const int led2_Pin =  23;      // the number of the LED pin
const int button3_Pin = 12;     // the number of the pushbutton pin
const int led3_Pin =  13;      // the number of the LED pin
const int button4_Pin = 32;     // the number of the pushbutton pin
const int led4_Pin =  33;      // the number of the LED pin

const int DEBOUNCE_DELAY = 50;   // the debounce time; increase if the output flickers

// -----------------------------------------------------------------------------
// button 0 states Momentary Control
// -----------------------------------------------------------------------------
int lastSteady0State = LOW;       // the previous steady state from the input pin
int lastFlickerable0State = LOW;  // the previous flickerable state from the input pin
int led0State = LOW;     // the current state of LED
int lastButton0State = LOW;    // the previous state of button
int currentButton0State; // the current state of button


// -----------------------------------------------------------------------------
// button 1 states Momentary Note
// -----------------------------------------------------------------------------
int lastSteady1State = LOW;       // the previous steady state from the input pin
int lastFlickerable1State = LOW;  // the previous flickerable state from the input pin
int led1State = LOW;     // the current state of LED
int lastButton1State = LOW;    // the previous state of button
int currentButton1State; // the current state of button

// -----------------------------------------------------------------------------
// button 2 states Momentary Control
// -----------------------------------------------------------------------------
int lastSteady2State = LOW;       // the previous steady state from the input pin
int lastFlickerable2State = LOW;  // the previous flickerable state from the input pin
int led2State = LOW;     // the current state of LED
int lastButton2State = LOW;    // the previous state of button
int currentButton2State; // the current state of button

// -----------------------------------------------------------------------------
// button 3 states Toggle Control
// -----------------------------------------------------------------------------
int lastSteady3State = LOW;       // the previous steady state from the input pin
int lastFlickerable3State = LOW;  // the previous flickerable state from the input pin
int led3State = LOW;     // the current state of LED
int lastButton3State;    // the previous state of button
int currentButton3State; // the current state of button

// -----------------------------------------------------------------------------
// button 4 states Toggle Control
// -----------------------------------------------------------------------------
int lastSteady4State = LOW;       // the previous steady state from the input pin
int lastFlickerable4State = LOW;  // the previous flickerable state from the input pin
int led4State = LOW;     // the current state of LED
int lastButton4State;    // the previous state of button
int currentButton4State; // the current state of button

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

  pinMode(button0_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led0_Pin, OUTPUT);          // set arduino pin to output mode
  pinMode(button1_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led1_Pin, OUTPUT);          // set arduino pin to output mode
  pinMode(button2_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led2_Pin, OUTPUT);          // set arduino pin to output mode
  pinMode(button3_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led3_Pin, OUTPUT);          // set arduino pin to output mode
  pinMode(button4_Pin, INPUT_PULLUP); // set arduino pin to input pull-up mode
  pinMode(led4_Pin, OUTPUT);          // set arduino pin to output mode

  currentButton0State = digitalRead(button0_Pin);
  currentButton1State = digitalRead(button1_Pin);
  currentButton2State = digitalRead(button2_Pin);
  currentButton3State = digitalRead(button3_Pin);
  currentButton4State = digitalRead(button4_Pin);
  
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
// Button 0 Momentary Control
// -----------------------------------------------------------------------------

  if (currentButton0State != lastFlickerable0State) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the the last flickerable state
      lastFlickerable0State = currentButton0State;
    }
    
    lastButton0State = currentButton0State;      // save the last state 
    currentButton0State = digitalRead(button0_Pin);
 
  if(lastButton0State == HIGH && currentButton0State == LOW) {
    Serial.println("The button is pressed");
    led0State = 0;
    digitalWrite(led0_Pin, led0State);
    MIDI.sendControlChange(85, 127, 13);
    } else if (lastButton0State == LOW && currentButton0State == HIGH) {
      Serial.println("The button is released");
      led0State = !led0State;
      digitalWrite(led0_Pin, led0State);
      MIDI.sendControlChange(85, 0, 13);
      // save the the last state
      lastButton0State = currentButton0State;
    }

// -----------------------------------------------------------------------------
// Button 1 Momentary Note
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
    led1State = 0;
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

// -----------------------------------------------------------------------------
// Button 2 Momentary Control
// -----------------------------------------------------------------------------

  if (currentButton2State != lastFlickerable2State) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the the last flickerable state
      lastFlickerable2State = currentButton2State;
    }
    
    lastButton2State = currentButton2State;      // save the last state 
    currentButton2State = digitalRead(button2_Pin);
 
  if(lastButton2State == HIGH && currentButton2State == LOW) {
    Serial.println("The button is pressed");
    led2State = 0;
    digitalWrite(led2_Pin, led2State);
    MIDI.sendControlChange(85, 127, 13);
    } else if (lastButton2State == LOW && currentButton2State == HIGH) {
      Serial.println("The button is released");
      led2State = !led2State;
      digitalWrite(led2_Pin, led2State);
      MIDI.sendControlChange(85, 0, 13);
      // save the the last state
      lastButton2State = currentButton2State;
    }

// -----------------------------------------------------------------------------
// Button 3 Toggle Control
// -----------------------------------------------------------------------------

    if (currentButton3State != lastFlickerable3State) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the the last flickerable state
      lastFlickerable3State = currentButton3State;
    }
    
    lastButton3State = currentButton3State;      // save the last state 
    currentButton3State = digitalRead(button3_Pin);
    
  
    if(lastButton3State == HIGH && currentButton3State == LOW) {
      Serial.println("The button is not active");
      led3State = !led3State;
      digitalWrite(led3_Pin, led3State);

      if (led3State == HIGH) {
        MIDI.sendControlChange(86, 0, 13);
      } else {
        MIDI.sendControlChange(86, 127, 13);
        Serial.println("The button is active");
      }
    }

// -----------------------------------------------------------------------------
// Button 4 Toggle Control
// -----------------------------------------------------------------------------

    if (currentButton4State != lastFlickerable4State) {
      // reset the debouncing timer
      lastDebounceTime = millis();
      // save the the last flickerable state
      lastFlickerable4State = currentButton4State;
    }
    
    lastButton4State = currentButton4State;      // save the last state 
    currentButton4State = digitalRead(button4_Pin);
    
  
    if(lastButton4State == HIGH && currentButton4State == LOW) {
      Serial.println("The button is pressed");
      led4State = !led4State;
      digitalWrite(led4_Pin, led4State);

      if (led4State == HIGH) {
        MIDI.sendControlChange(87, 127, 13);
      } else {
        MIDI.sendControlChange(87, 0, 13);
        Serial.println("The button is not active");
      }
    }
}
