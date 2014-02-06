#define LED_MAX 200
#define THRESHOLD 400
#define OPEN_TIME 5000

#define OPEN 3

// #define DEBUG 1

const byte unusedPins[] = {0, 1, 10, 11, 12};

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif
    for (byte i=2; i <= 9; i++) {
        pinMode(i, OUTPUT);
        digitalWrite(i, LOW);  // I think they default to low - this is just paranoia
    }

    // Set pullup resistor mode for the unused pins - this apparently saves a little
    // bit of power
    for (byte i=0; i < sizeof(unusedPins); i++) {
        pinMode(unusedPins[i], INPUT);
        digitalWrite(unusedPins[i], HIGH);
    }
}

const byte sensor[] = {0, 1, 2, 3};
const byte LED[] = {3, 5, 6, 9};
const byte lock[] = {2, 4, 7, 8};
byte opened[4] = {};
unsigned long otime[4];

void loop() {
    for (int c=0; c<4; c++) {
        if (opened[c] != OPEN) {
            int v = analogRead(sensor[c]);
#ifdef DEBUG
            Serial.print(c);
            Serial.print(“: “);
            Serial.println(v);
#endif
            if (v > THRESHOLD) {
                if (++opened[c] == OPEN)
                    open(c);
            } else {
                opened[c] = 0;
            }
        } else {
            unsigned long now = millis();
            unsigned long openFor = now - otime[c];
            if (openFor >= OPEN_TIME) {
                close(c);
            } else {
                int LEDLevel = LED_MAX - ((LED_MAX * openFor) / OPEN_TIME);
                analogWrite(LED[c], LEDLevel);
            }
        }
    }
#ifdef DEBUG
    delay(200);
#endif
}

void open(int c) {
#ifdef DEBUG
    Serial.print(“Opening “);
    Serial.println(c);
#endif
    digitalWrite(lock[c], HIGH);
    otime[c] = millis();
    analogWrite(LED[c], LED_MAX);
}

void close(int c) {
#ifdef DEBUG
    Serial.print(“Closing “);
    Serial.println(c);
#endif
    digitalWrite(lock[c], LOW);
    opened[c] = 0;
    analogWrite(LED[c], 0);
}
