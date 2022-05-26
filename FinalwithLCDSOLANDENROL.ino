#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


// change this to make the song slower or faster
int tempo = 180;

// change this to whichever pin you want to use
int buzzer = 12;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Nokia Ringtone 
  // Score available at https://musescore.com/user/29944637/scores/5266155
  
  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4, 
  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4, 
  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
  NOTE_A4, 2, 
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);
#include <Adafruit_Fingerprint.h>
#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #10 is IN from sensor (GREEN wire)
// pin #11 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(10, 11);
#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define Password_Length 5
uint8_t id;
char keypressed;
char Data[Password_Length];
char Master[Password_Length] = "5555";
char Overide[Password_Length] = "#*9*";
byte data_count = 0, master_count = 0;
bool Pass_is_good = true;
char customKey;

const byte numRows = 4; //number of rows on the keypad
const byte numCols = 3; //number of columns on the keypad

//keymap defines the key pressed according to the row and columns just as appears on the keypad
char keymap[numRows][numCols] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//Code that shows the the keypad connections to the arduino terminals
byte rowPins[numRows] = {2, 3, 4, 5}; //Rows 0 to 3
byte colPins[numCols] = {6, 7, 8}; //Columns 0 to 2

Servo myservo;

//initializes an instance of the Keypad class
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//Setup the serial monitor and servo
void setup()
{
  Serial.begin(9600);
  delay(100);
  finger.begin(57600);
  delay(5);
 

  pinMode(9, OUTPUT); //FingerPrint
  pinMode(12, OUTPUT); //Green LED
  pinMode(13, OUTPUT); //Red LED
  delay(15);
  lcd.begin(16, 2);
  lcd.print("- Project SPL -");
  lcd.setCursor(0, 1);
  lcd.print("- BY BLIZZARD -");
   for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
  lcd.clear();
}

//The main loop
void loop()
{
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(9, LOW);
  myservo.write(0);
  getFingerprintID();
  delay(50);
  char recipeNumber = 0;

  if (Serial.available())
  {
    recipeNumber = Serial.read();
  }

  switch (recipeNumber)
  {
    case '1':
      activatelock();
      break;
  }
  recipeNumber = 0;
  //Process key pad inputs
  if (Pass_is_good)
  {
    Serial.println("Enter Password: ");
    lcd.setCursor(0, 0);
    lcd.print(" Enter Password:");
    Pass_is_good = false;
  }
  customKey = myKeypad.getKey();

  if (customKey)
  {
    Serial.println(customKey);
    Data[data_count] = customKey;
    lcd.setCursor(data_count, +1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor

    data_count++;
  }
  if (data_count == Password_Length - 1)
  {
    Serial.println(Data);
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      lcd.print("  Door is Open ");
      Serial.println("*** Password Correct! ***");
      activatelock();
      Pass_is_good = true;

    }
    else if (!strcmp(Data,Overide))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(" MASTER OVERIDE ");
      Serial.println("*** MASTER OVERIDE! ***");
      delay(1000);
      lcd.write("ENROLL NEW USER");
      delay(1000);
      lcd.clear();
      delay(500);
      Enrolling(); //Enrolling function
      delay(1000);
      lcd.clear();
      Pass_is_good = true;
    }
    else
    {
      lcd.clear();
      lcd.print("  Wrong Password");
      Serial.println("Password Incorrect!");
      digitalWrite(13, LOW);
      digitalWrite(13, HIGH);
      delay(1000);
      Pass_is_good = true;

    }

    clearData();
  }
}
//Clear user entered data




void clearData()
{
  //Clear variables
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;
}
//Activate the lock once the correct code has been entered
void activatelock()
{
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  Serial.print(" door unlocked");
  lcd.clear();
  lcd.print("  Door is Open ");
  tone(12, 200);
  
  //Servo control
  digitalWrite(9, HIGH);
  delay(5000);
  digitalWrite(9, LOW);
  noTone(12);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Enter Password:");
}



void getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   FingerPrint  ");
    lcd.setCursor(0, 1);
    lcd.print("    Not Found   ");
    delay(500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password:");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  activatelock();
  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID;
}
void Enrolling() {
  customKey = NULL;
  lcd.clear();
  lcd.print("Enroll New");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("Enter new ID");
  id = readnumber(); //This function gets the Id it was meant to get it from the Serial monitor but we modified it
  if (id == 0) {// ID #0 not allowed, try again!
    return;
  }
  while (! getFingerprintEnroll() );
}
//Enrolling function only the modifications are commented
uint8_t getFingerprintEnroll() {
  int p = -1;
  lcd.clear();
  lcd.print("Enroll ID:"); //Message to print for every step
  lcd.setCursor(10, 0);
  lcd.print(id);
  lcd.setCursor(0, 1);
  lcd.print("Place finger"); //First step
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  // OK success!
  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      return p;
    default:

      return p;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Remove finger"); //After getting the first template successfully
  lcd.setCursor(0, 1);
  lcd.print("please !");
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  p = -1;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place same"); //We launch the same thing another time to get a second template of the same finger
  lcd.setCursor(0, 1);
  lcd.print("finger please");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
  }
  // OK success!
  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      break;
    case FINGERPRINT_IMAGEMESS:
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      return p;
    case FINGERPRINT_FEATUREFAIL:
      return p;
    case FINGERPRINT_INVALIDIMAGE:

      return p;
    default:
      return p;
  }

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    return p;
  } else {
    return p;
  }
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear(); //if both images are gotten without problem we store the template as the Id we entred
    lcd.setCursor(0, 0);
    lcd.print("Stored in"); //Print a message after storing and showing the ID where it's stored
    lcd.setCursor(0, 1);
    lcd.print("ID: ");
    lcd.setCursor(5, 1);
    lcd.print(id);
    delay(1500);
    lcd.clear();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    return p;
  } else {
    return p;
  }
}

//This function gets the ID number as 3 digits format like 001 for ID 1
//And return the number to the enrolling function
uint8_t readnumber(void) {
  uint8_t num = 0;
  while (num == 0) {
    char keey = myKeypad.waitForKey();
    int num1 = keey - 48;
    lcd.setCursor(0, 1);
    lcd.print(num1);
    keey = myKeypad.waitForKey();
    int num2 = keey - 48;
    lcd.setCursor(1, 1);
    lcd.print(num2);
    keey = myKeypad.waitForKey();
    int num3 = keey - 48;
    lcd.setCursor(2, 1);
    lcd.print(num3);
    delay(1000);
    num = (num1 * 100) + (num2 * 10) + num3;
    keey = NO_KEY;

  }
  return num;
}
