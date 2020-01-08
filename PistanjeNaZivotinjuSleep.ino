/*
  Opis: pistanje kad zec predje na zabranjeni deo kauca, kod nekih zica i sl.
  Arduino: ATtiny85
  Napajanje: 18650
  Senzori:
    - PIR: detekcija zivotinje
    - 2 tastera: vol+ i vol- za buzzer
  Aktuatori:
    - Buzzer
  Napomene:
    - Mozda bi trebalo ubaciti V boost zbog stabilnog 5V napona.
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>

const int pinPIR = 2;           // PIR senzor; ako se promeni ova vrednost, obavezno izmeniti PCINT? u sleep() i setup()
const int pinVolUp = 1;         // taster za pojacavanje zvuka na buzzer-u
const int pinVolDown = 3;       // taster za smanjivanje zvuka na buzzer-u
const int pinBuzz = 0;          // buzzer/zvucnik
const int itvBuzzOn = 350;      // trajanje u msec piska
const int itvBuzzOff = 700;     // trajanje u msec vremena izmedju 2 piska
const int itvVolPress = 3000;   // broj ms za koji se pistanje buzzer-a produzava kod pritiska na neki vol taster
int vol = 4;                    // jacina zvuka na buzzer-u
bool volUpPress = false;        // taster za pojacavanje zvuka je pritisnut
bool volDownPress = false;      // taster za smanjivanje zvuka je pritisnut
volatile long msPirSignal = 0;  // vreme poslednjeg PIR signala
long msVolPress = -itvVolPress; // vreme poslednjeg pritiska na neki vol taster
bool firstPirSignal = true;     // prvi PIR signal se javlja odmah po paljenju aparata i smatra se laznim

void setup()
{
  pinMode(pinPIR, INPUT);
  pinMode(pinVolUp, INPUT_PULLUP);
  pinMode(pinVolDown, INPUT_PULLUP);

  pinMode(pinBuzz, OUTPUT);
  digitalWrite(pinBuzz, false);

  GIMSK |= _BV(PCIE);   // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2); // Use PBX as interrupt pin
  sei();                // Enable interrupts
}

void sleep()
{
  GIMSK |= _BV(PCIE);                  // Enable Pin Change Interrupts
  PCMSK |= _BV(PCINT2);                // Use PBX as interrupt pin
  ADCSRA &= ~_BV(ADEN);                // ADC off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // replaces above statement
  sleep_enable();                      // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
  sei();                               // Enable interrupts
  sleep_cpu();                         // sleep
}

ISR(PCINT0_vect)
{
  msPirSignal = millis();
}

// podesavanje jacine zvuka buzzer-a na osnovu pritiska na vol tastere
void check_vol_press()
{
  const int volCoef = 4;       // koeficijent promene jacine zvuka buzzer-a
  const int minVol = 1;        // najmanji jacina zvuka buzzer-a
  const int maxVol = 255;      // najveca jacina zvuka buzzer-a
  const int itvDebounce = 100; // vol button debounce interval

  // da li je neki taster pritisnut
  if (!digitalRead(pinVolUp) || !digitalRead(pinVolDown))
  {
    msVolPress = millis();
    delay(itvDebounce);

    if (digitalRead(pinVolUp) == volUpPress)
    {
      volUpPress = !volUpPress;
      if (volUpPress)
        vol *= volCoef;

      if (vol > maxVol)
        vol = maxVol;

      analogWrite(pinBuzz, vol);
      //B delay(itvBuzzOn);
    }
    if (digitalRead(pinVolDown) == volDownPress)
    {
      volDownPress = !volDownPress;
      if (volDownPress)
        vol /= volCoef;

      if (vol < minVol)
        vol = minVol;

      analogWrite(pinBuzz, vol);
      //B delay(itvBuzzOn);
    }
  }
}

void loop()
{
  check_vol_press();

  // PIR -> HIGH => detektovan je pokret
  if ((digitalRead(pinPIR) && !firstPirSignal) || millis() - msVolPress < itvVolPress)
  {
    int itv = (millis() - msPirSignal) % (itvBuzzOn + itvBuzzOff);
    analogWrite(pinBuzz, itv < itvBuzzOn ? vol : 0);
    delay(10);
  }
  // PIR -> LOW => nije detektovana kretnja
  else
  {
    firstPirSignal = false;
    digitalWrite(pinBuzz, false);
    sleep();
  }
}
