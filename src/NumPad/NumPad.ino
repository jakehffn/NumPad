#include <Keyboard.h>
#include <Keypad.h>

#define KEY_NUM_LOCK 0x53
#define KEY_7_HOME 0x5F
#define KEY_8_UP 0x60
#define KEY_9_PGUP 0x61
#define KEY_4_LEFT 0x5C
#define KEY_5 0x22
#define KEY_6_RIGHT 0x5E
#define KEY_PLUS 0x57
#define KEY_MINUS 0x56
#define KEY_ENTER 0x58
#define KEY_SLASH 0x54
#define KEY_STAR 0x55
#define KEY_1_END 0x59
#define KEY_2_DOWN 0x5A
#define KEY_3_PGDN 0x5B
#define KEY_0_INS 0x62
#define KEY_PERIOD_DEL 0x63

const byte ROWS = 4;
const byte COLS = 5;

char keys[ROWS][COLS] = {
  {KEY_MINUS, KEY_PLUS, ' ', ' ', ' '},
  {KEY_STAR, KEY_9_PGUP, KEY_6_RIGHT, KEY_3_PGDN, KEY_ENTER},
  {KEY_SLASH, KEY_8_UP, KEY_5, KEY_2_DOWN, KEY_PERIOD_DEL},
  {KEY_NUM_LOCK, KEY_7_HOME, KEY_4_LEFT, KEY_1_END, KEY_0_INS},
};

byte rowPins[ROWS] = {A1, A2, A0, 15};
byte colPins[COLS] = {2, 3, 4, 5, 6}; 

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

bool numLock = false;

void setup() {
    Keyboard.begin();
}


void loop() {

    // Update key list and do if there are changes
    if (kpd.getKeys()) {  
      
        // Iterate through updated keys
        for (int i=0; i<LIST_MAX; i++) {
          
            if (kpd.key[i].stateChanged) {
              
                switch (kpd.key[i].kstate) { 
                  
                    case PRESSED:
                        Keyboard.press(kpd.key[i].kchar + 136);
                        break;
                        
                    case RELEASED:
                        Keyboard.release(kpd.key[i].kchar + 136);
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }
}
