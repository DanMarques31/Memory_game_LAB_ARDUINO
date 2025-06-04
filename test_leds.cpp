// Definindo os pinos do Arduino para cada deslocador
#define DS_PIN 3
#define ST_CP_PIN 5
#define SH_CP_PIN 4

// Definindo os pinos dos LEDs RGB
#define LED1_RED_PIN 1
#define LED1_GREEN_PIN 2
#define LED1_BLUE_PIN 3
#define LED2_RED_PIN 4
#define LED2_GREEN_PIN 5
#define LED2_BLUE_PIN 6

// Definindo o pino do botão
#define BUTTON_PIN 2

// Definindo a cor
#define RED 0b01111111  // Invertido para LEDs de cátodo comum
#define GREEN 0b10111111  // Invertido para LEDs de cátodo comum
#define BLUE 0b11011111  // Invertido para LEDs de cátodo comum

// Definindo o número de LEDs
#define NUM_LEDS 2

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
    // Esta função realiza o shift de 8 bits MSB primeiro,
    // na borda de subida do clock,
    // o clock fica em nível baixo

    // Configuração interna da função
    int i;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    // Limpa tudo apenas para garantir que o registrador de deslocamento esteja preparado para o shift
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    // Para cada bit no byte myDataOut
    // PERCEBA QUE ESTAMOS CONTANDO PARA BAIXO no nosso loop for
    // Isso significa que %00000001 ou "1" irá passar pelo loop de forma que
    // seja o pino Q0 que se acende.
    for (i = 7; i >= 0; i--) {
        digitalWrite(myClockPin, 0);

        // Se o valor passado para myDataOut e o resultado de um bitmask
        // for verdadeiro, então... então se estivermos em i=6 e nosso valor for
        // %11010100, o código compara isso com %01000000
        // e procede para configurar pinState como 1.
        if (myDataOut & (1 << i)) {
            pinState = 1;
        } else {
            pinState = 0;
        }

        // Define o pino como HIGH ou LOW dependendo de pinState
        digitalWrite(myDataPin, pinState);
        // O registrador desloca os bits no pulso de subida do clock
        digitalWrite(myClockPin, 1);
        // Zera o pino de dados após o shift para evitar vazamento de corrente
        digitalWrite(myDataPin, 0);
    }

    // Interrompe o shift
    digitalWrite(myClockPin, 0);
}

void lightLed(int led, byte color) {
    // Desliga todos os LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        shiftOut(DS_PIN, SH_CP_PIN, 0);
    }

    // Acende o LED com a cor especificada
    if (led == 0) {
        shiftOut(LED1_RED_PIN, SH_CP_PIN, (color & RED) ? 0 : 0b10000000);
        shiftOut(LED1_GREEN_PIN, SH_CP_PIN, (color & GREEN) ? 0 : 0b10000000);
        shiftOut(LED1_BLUE_PIN, SH_CP_PIN, (color & BLUE) ? 0 : 0b10000000);
    } else if (led == 1) {
        shiftOut(LED2_RED_PIN, SH_CP_PIN, (color & RED) ? 0 : 0b10000000);
        shiftOut(LED2_GREEN_PIN, SH_CP_PIN, (color & GREEN) ? 0 : 0b10000000);
        shiftOut(LED2_BLUE_PIN, SH_CP_PIN, (color & BLUE) ? 0 : 0b10000000);
    }

    // Atualiza os LEDs
    digitalWrite(ST_CP_PIN, HIGH);
    digitalWrite(ST_CP_PIN, LOW);
}

void setup() {
    // Configura os pinos do Arduino
    pinMode(DS_PIN, OUTPUT);
    pinMode(ST_CP_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED1_RED_PIN, OUTPUT);
    pinMode(LED1_GREEN_PIN, OUTPUT);
    pinMode(LED1_BLUE_PIN, OUTPUT);
    pinMode(LED2_RED_PIN, OUTPUT);
    pinMode(LED2_GREEN_PIN, OUTPUT);
    pinMode(LED2_BLUE_PIN, OUTPUT);
}

void loop() {
    // Leitura do estado atual do botão
    int reading = digitalRead(BUTTON_PIN);

    // Verifica se o botão foi pressionado
    if (reading == LOW) {
        // Acende os LEDs um de cada vez
        for (int i = 0; i < NUM_LEDS; i++) {
            lightLed(i, RED);  // Acende o LED na cor vermelha
            delay(2000);  // Aguarda 2 segundos antes de acender o próximo LED
        }
    }
}