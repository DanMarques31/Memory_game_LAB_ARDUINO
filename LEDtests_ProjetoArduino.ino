#define DATA_PIN 3
#define LATCH_PIN 5
#define CLOCK_PIN 4

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void shiftOutRgb(byte red, byte green, byte blue) {
  // Define a ordem correta dos bits para os LEDs RGB
  byte ledsData = (blue << 2) | (green << 1) | red;

  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, ledsData);
  digitalWrite(LATCH_PIN, HIGH);
}

void loop() {
  // Acende o LED RGB em vermelho
  shiftOutRgb(1, 0, 0);
  delay(1000);

  // Acende o LED RGB em verde
  shiftOutRgb(0, 1, 0);
  delay(1000);

  // Acende o LED RGB em azul
  shiftOutRgb(0, 0, 1);
  delay(1000);

  // Apaga o LED RGB
  shiftOutRgb(0, 0, 0);
  delay(1000);
}
