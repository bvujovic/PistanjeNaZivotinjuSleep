/*
  Opis: pistanje kad zec predje na zabranjeni deo kauca, kod nekih zica i sl.
  Arduino: ATtiny85
  Napajanje: 18650
  Senzori:
    On/off dugme
    - PIR - detekcija zivotinje
  Aktuatori:
    - Buzzer
  Napomene:
    - Mozda bi trebalo ubaciti V boost zbog stabilnog 5V napona.
*/


const int pinPIR = 2;

const int pinBuzz = 3;
const int jacinaPiska = 50;
const int itvBuzz = 500; // trajanje u msec piska i vremena izmedju 2 piska
const int itvBuzzOdmor = 10000; // delay interval koji ide posle pishtanja

const int itvMain = 200;
//B const int itvInit = 5000;

void setup() {
  pinMode(pinPIR, INPUT);

  pinMode(pinBuzz, OUTPUT);
  digitalWrite(pinBuzz, false);

  //B delay(itvBuzzOdmor);
  do
    delay(itvMain);
  while (digitalRead(pinPIR)); // cekamo dogod je inicijalni HIGH signal na PIRu
}

void loop() {
  if (digitalRead(pinPIR))
    pishtanje();

  delay(itvMain);
}

void pishtanje()
{
  for (int i = 1; i <= 3; i++)
  {
    //B analogWrite(pinBuzz, jacinaPiska);
    digitalWrite(pinBuzz, true);
    delay(itvBuzz);
    digitalWrite(pinBuzz, false);
    delay(itvBuzz);
  }
  delay(itvBuzzOdmor);
}
