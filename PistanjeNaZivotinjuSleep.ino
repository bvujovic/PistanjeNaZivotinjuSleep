/*
  Opis: pistanje kad zec predje na zabranjeni deo kauca, kod nekih zica i sl.
  Arduino: ATtiny85
  Napajanje: 18650
  Senzori:
    - PIR - detekcija zivotinje
  Aktuatori:
    - Buzzer
  Napomene:
    - Mozda bi trebalo ubaciti V boost zbog stabilnog 5V napona.
*/

const int pinPIR = 2;        // PIR senzor; ako se promeni ova vrednost, obavezno izmeniti PCINT? u sleep() i setup()
const int pinBuzz = 0;       // buzzer/zvucnik
const int itvBuzzOn = 500;   // trajanje u msec piska
const int itvBuzzOff = 1000; // trajanje u msec vremena izmedju 2 piska
// const int buzzLevel = 50;       // jacina zvuka na buzzer-u

volatile long msPirSignal = 0; // vreme (poslednjeg) PIR signala

void setup()
{
  pinMode(pinPIR, INPUT);

  pinMode(pinBuzz, OUTPUT);
  digitalWrite(pinBuzz, false);

  GIMSK |= _BV(PCIE);   // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2); // Use PBX as interrupt pin
  sei();                // Enable interrupts
}

ISR(PCINT0_vect) {}

void loop()
{
  if (digitalRead(pinPIR))
  {
    int itv = millis() % (itvBuzzOn + itvBuzzOff);
    digitalWrite(pinBuzz, itv < itvBuzzOn);
  }
  else
  {
    digitalWrite(pinBuzz, false);
    //TODO sleep
  }
}
