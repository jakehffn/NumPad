#include <Keyboard.h>
#include <Keypad.h>

#define KEY_LEFT_CTRL  0x80
#define KEY_LEFT_SHIFT 0x81
#define KEY_LEFT_ALT   0x82
#define KEY_LEFT_GUI   0x83
#define KEY_RIGHT_CTRL 0x84
#define KEY_RIGHT_SHIFT    0x85
#define KEY_RIGHT_ALT  0x86
#define KEY_RIGHT_GUI  0x87

#define KEY_UP_ARROW   0xDA
#define KEY_DOWN_ARROW 0xD9
#define KEY_LEFT_ARROW 0xD8
#define KEY_RIGHT_ARROW    0xD7
#define KEY_BACKSPACE  0xB2
#define KEY_TAB        0xB3
#define KEY_RETURN 0xB0
#define KEY_ESC        0xB1
#define KEY_INSERT 0xD1
#define KEY_DELETE 0xD4
#define KEY_PAGE_UP    0xD3
#define KEY_PAGE_DOWN  0xD6
#define KEY_HOME   0xD2
#define KEY_END        0xD5
#define KEY_CAPS_LOCK  0xC1
#define KEY_F1     0xC2
#define KEY_F2     0xC3
#define KEY_F3     0xC4
#define KEY_F4     0xC5
#define KEY_F5     0xC6
#define KEY_F6     0xC7
#define KEY_F7     0xC8
#define KEY_F8     0xC9
#define KEY_F9     0xCA
#define KEY_F10        0xCB
#define KEY_F11        0xCC
#define KEY_F12        0xCD

const byte ROWS = 4;
const byte COLS = 5;
char keys[ROWS][COLS] = {
  {'-','+',' ',' ',' '},
  {'*','9','6','3','\n'},
  {'/','8','5','2','.'},
  {'n','7','4','1','0'},
};

char altKeys[ROWS][COLS] = {
  {KEY_F12,'+',' ',' ',' '},
  {KEY_F9,KEY_PAGE_UP,KEY_RIGHT_ARROW,KEY_PAGE_DOWN,'\n'},
  {KEY_F5,KEY_UP_ARROW,'5',KEY_DOWN_ARROW,KEY_DELETE},
  {'n',KEY_HOME,KEY_LEFT_ARROW,KEY_END,KEY_INSERT},
};

byte rowPins[ROWS] = {A1, A2, A0, 15};
byte colPins[COLS] = {2, 3, 4, 5, 6}; 


char* mainKeymap = makeKeymap(keys);
char* altKeymap = makeKeymap(altKeys);
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );;

unsigned long startTime;
bool numLock = false;
String msg;
bool held[LIST_MAX] = {0};
const int repeatDelay = 150;

void setup() {
  
    delay(3);
    Keyboard.begin();
    startTime = millis();
}


void loop() {

    // This turns the LED on the board on and off when numLock is on or off
    if (numLock) {
        TXLED1;
    } else {
        TXLED0;
    }

    // Update key list and do if there are changes
    if (kpd.getKeys()){  

        // You can avoid running findInList() every loop by initializing with -1 here
        int numLockPos = -1;

        // Switch key outputs depending on numLock toggle
        if (kpd.isPressed('n')){
            
            Serial.print(numLock);
            numLock = !numLock;
            
            if (numLock) {
                kpd.begin(altKeymap);
            } else {
                kpd.begin(mainKeymap);
            }

            numLockPos = kpd.findInList('n');
        } 
        
        // Iterate through updated keys
        for (int i=0; i<LIST_MAX; i++){

            // If the key is equal to the pos of the numLock key, ignore
            if (i == numLockPos) {
              break;
            }
          
            if (kpd.key[i].stateChanged){
              
                switch (kpd.key[i].kstate) { 
                  
                    case PRESSED:
                    
                        Keyboard.write(kpd.key[i].kchar);
                        startTime = millis() - repeatDelay;
                        
                        break;
                    case HOLD:
                    
                        held[i] = true;
                        break;
                    case RELEASED:
                    
                        held[i] = false;
                        break;
                    case IDLE:
                        break;
                }
            }
        }
    }

    // This controls how held keys are repeated and how long
    if ( (millis()-startTime) > 50 ) {
      
        for (int i=0; i<LIST_MAX; i++) {
          
            if (held[i]) {
                Keyboard.write(kpd.key[i].kchar);
            }
        }
        startTime = millis();
    }
    
    
}
