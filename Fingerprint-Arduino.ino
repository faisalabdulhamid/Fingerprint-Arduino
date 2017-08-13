#include <WString.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//--------{ CONSTANS }-------------------------------------------
#define LED_TIMEOUT 1000
#define SERIAL_BAUDRATE 9600
#define STX "\x02"
#define ETX "\x03"
#define RS  "$"

#define LED_BLUE_PIN 13
#define LED_GREEN_PIN 12
#define LED_RED_PIN 11
#define BUZZER_PIN 10
#define FINGERPRINT_BAUDRATE 57600

//-------{ WARNING, ERROR AND STATUS CODE }------------------
#define MSG_METHOD_SUCCESS 0
#define WRG_NO_SERIAL_DATA_AVAILABLE 250
#define ERR_SERIAL_IN_COMMAND_NOT_TERMINATED -1

#define FINGER_OK 0
#define ENROLL 2
#define FINGER_NOT_OK 255

//-------{ KONFIGURASI FINGERPRINT }------------
SoftwareSerial mySerial(2,3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//-------{ METHOD DECLARATIONS }---------------
int readSerialInputCommand(String *command);
int getFingerprintIDez();

//-------{ VARIABLE GLOBAL }------------
byte menu_pilihan = 255;
long interval = 5000; // 5 detik
unsigned long previousMillis = 0;


void setup() {
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  Serial.begin(SERIAL_BAUDRATE);
  while (!Serial);

  //------{ Init Fingerprint }-------
  Serial.println("Initialization Fingerprint");
  finger.begin(FINGERPRINT_BAUDRATE);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  }
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }
  
  //------------{ CEK MODUL LED & BUZZER }---------------
  digitalWrite(LED_BLUE_PIN, HIGH);
  delay(LED_TIMEOUT);
  digitalWrite(LED_BLUE_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(LED_TIMEOUT);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(LED_TIMEOUT);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(LED_TIMEOUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  String command = "";  //Used to store the latest received command
  byte serialResult = 0; //return value for reading operation method on serial in put buffer
  
  
  serialResult = readSerialInputCommand(&command);
  if(serialResult == MSG_METHOD_SUCCESS){
    if(command == "1#"){
      menu_pilihan = FINGER_OK;
      //WriteDummyWeatherData();
    }
    if(command == "2#"){
      menu_pilihan = FINGER_NOT_OK;
    }
    if(command == "3#"){
      menu_pilihan = ENROLL;
    }
    //Serial.println("MSG_METHOD_SUCCESS");
    //Serial.println(serialResult);
  }
  
  if(serialResult == WRG_NO_SERIAL_DATA_AVAILABLE){//If there is no data AVAILABLE at the serial port, let the LED blink
    if(menu_pilihan == FINGER_NOT_OK){
       //Serial.println(menu_pilihan);
       digitalWrite(LED_BLUE_PIN, HIGH);
       delay(LED_TIMEOUT);
       digitalWrite(LED_BLUE_PIN, LOW);
       delay(LED_TIMEOUT);
    }
    //Serial.println("WRG_NO_SERIAL_DATA_AVAILABLE");
    //Serial.println(serialResult);
  }
  else{
    if(serialResult == ERR_SERIAL_IN_COMMAND_NOT_TERMINATED){//If the command format was invalid, the led is turned off for two seconds
      //Serial.println("ERR_SERIAL_IN_COMMAND_NOT_TERMINATED");
      //Serial.println(serialResult);
      digitalWrite(LED_BLUE_PIN, LOW);
      delay(2000);
    }
  }

  if(menu_pilihan == FINGER_OK)
  { 
    //Serial.println("FINGER_OK");
    int id_finger = getFingerprintIDez();
    if(id_finger != -1)
    {
      digitalWrite(LED_GREEN_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(800);
      digitalWrite(LED_GREEN_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      delay(100);
      //menu_pilihan = FINGER_NOT_OK;
      //Serial.println("getFingerprintIDez");
      //Serial.println(getFingerprintIDez());
      
      // found a match!
      switch(id_finger)
      {
        case 1:{
          Serial.print(STX);
          Serial.print("151610005");
          Serial.print(ETX);
          break;
        }
        case 2:{
          Serial.print(STX);
          Serial.print("151610006");
          Serial.print(ETX);
          break;
        }
        case 3:{
          Serial.print(STX);
          Serial.print("151610007");
          Serial.print(ETX);
          break;
        }
        case 4:{
          Serial.print(STX);
          Serial.print("151610008");
          Serial.print(ETX);
          break;
        }
        case 5:{
          Serial.print(STX);
          Serial.print("151610009");
          Serial.print(ETX);
          break;
        }
        case 6:{
          Serial.print(STX);
          Serial.print("151610010");
          Serial.print(ETX);
          break;
        }
        case 7:{
          Serial.print(STX);
          Serial.print("151610011");
          Serial.print(ETX);
          break;
        }
        case 8:{
          Serial.print(STX);
          Serial.print("151610012");
          Serial.print(ETX);
          break;
        }
        case 9:{
          Serial.print(STX);
          Serial.print("151610013");
          Serial.print(ETX);
          break;
        }
        case 10:{
          Serial.print(STX);
          Serial.print("151610014");
          Serial.print(ETX);
          break;
        }
      }
    }
  }
  
  if(menu_pilihan == ENROLL)
  {
    /*
    Serial.println("Type in the ID # you want to save this finger as...");
    uint8_t id = 2;
    while (true) {
      while (! Serial.available());
      char c = Serial.read();
      if (! isdigit(c)) break;
      id *= 10;
      id += c - '0';
    }
    Serial.print("Enrolling ID #");
    Serial.println(id);
    
    while (!  getFingerprintEnroll(id) );
    */
    menu_pilihan = FINGER_NOT_OK;
  }
}

int readSerialInputCommand(String *command){
  
  int operationStatus = MSG_METHOD_SUCCESS;//Default return is MSG_METHOD_SUCCESS reading data from com buffer.
  
  //check if serial data is available for reading
  if (Serial.available()) {
     char serialInByte;//temporary variable to hold the last serial input buffer character
     
     do{//Read serial input buffer data byte by byte 
       serialInByte = Serial.read();
       *command = *command + serialInByte;//Add last read serial input buffer byte to *command pointer
     }while(serialInByte != '#' && Serial.available());//until '#' comes up or no serial data is available anymore
     
     if(serialInByte != '#') {
       operationStatus = ERR_SERIAL_IN_COMMAND_NOT_TERMINATED;
     }
  }
  else{//If not serial input buffer data is AVAILABLE, operationStatus becomes WRG_NO_SERIAL_DATA_AVAILABLE (= No data in the serial input buffer AVAILABLE)
    operationStatus = WRG_NO_SERIAL_DATA_AVAILABLE;
  }
  
  return operationStatus;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_NOTFOUND) {
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
