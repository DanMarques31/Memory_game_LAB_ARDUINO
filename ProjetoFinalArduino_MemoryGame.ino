// Definindo os pinos do Arduino para cada deslocador
#define DS_PIN 3
#define ST_CP_PIN 5
#define SH_CP_PIN 4
#define ST_CP_PIN2 6
#define SH_CP_PIN2 7
#define ST_CP_PIN3 8
#define SH_CP_PIN3 9

// Definindo o pino do botão
#define BUTTON_PIN 2

// Definindo a cor
#define RED 0b10000000

// Definindo o número de LEDs
#define NUM_LEDS 6

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
    // Esta função realiza o shift de 8 bits MSB primeiro, na borda de subida do clock o clock fica em nível baixo

    // Configuração interna da função
    int i;
    int pinState;
    pinMode(myClockPin, OUTPUT);
    pinMode(myDataPin, OUTPUT);

    // Limpa tudo apenas para garantir que o registrador de deslocamento esteja preparado para o shift
    digitalWrite(myDataPin, 0);
    digitalWrite(myClockPin, 0);

    // Para cada bit no byte myDataOut %00000001 ou "1" irá passar pelo loop de forma que
    // seja o pino Q0 que se acende.
    for (i = 7; i >= 0; i--) {
        digitalWrite(myClockPin, 0);

        // Se o valor passado para myDataOut e o resultado de um bitmask
        // for verdadeiro, então se estivermos em i=6 e nosso valor for
        // %11010100, o código compara isso com %01000000
        // e procede para configurar pinState como 1.
        if (myDataOut & (1 << i)) {
            pinState = 1;
        } 
        
        else {
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

void lightLed(int index, byte color) {
    // Desliga todos os LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        shiftOut(DS_PIN, SH_CP_PIN, 0);
        shiftOut(ST_CP_PIN2, SH_CP_PIN2, 0);
        shiftOut(ST_CP_PIN3, SH_CP_PIN3, 0);
    }

    // Acende o LED na posição 'index' com a cor 'color'
    for (int i = 0; i < NUM_LEDS; i++) {
        if (i == index) {
            shiftOut(DS_PIN, SH_CP_PIN, (color & RED) ? 0b10000000 : 0);
            shiftOut(ST_CP_PIN2, SH_CP_PIN2, 0);
            shiftOut(ST_CP_PIN3, SH_CP_PIN3, 0);
        } else {
            shiftOut(DS_PIN, SH_CP_PIN, 0);
            shiftOut(ST_CP_PIN2, SH_CP_PIN2, 0);
            shiftOut(ST_CP_PIN3, SH_CP_PIN3, 0);
        }
    }

    // Atualiza os LEDs
    digitalWrite(ST_CP_PIN, HIGH);
    digitalWrite(ST_CP_PIN, LOW);
    digitalWrite(ST_CP_PIN2, HIGH);
    digitalWrite(ST_CP_PIN2, LOW);
    digitalWrite(ST_CP_PIN3, HIGH);
    digitalWrite(ST_CP_PIN3, LOW);
}

void setup() {
    // Configura os pinos do Arduino
    pinMode(DS_PIN, OUTPUT);
    pinMode(ST_CP_PIN, OUTPUT);
    pinMode(SH_CP_PIN, OUTPUT);
    pinMode(ST_CP_PIN2, OUTPUT);
    pinMode(SH_CP_PIN2, OUTPUT);
    pinMode(ST_CP_PIN3, OUTPUT);
    pinMode(SH_CP_PIN3, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
    // Leitura do estado atual do botão
    int reading = digitalRead(BUTTON_PIN);

    // Verifica se o botão foi pressionado
    if (reading == LOW) {
        // Aguarda até que o botão seja solto
        while (digitalRead(BUTTON_PIN) == LOW) {
            delay(50);
        }

        // Acende os LEDs um de cada vez
        for (int i = 0; i < NUM_LEDS; i++) {
            lightLed(i, RED);  // Acende o LED na cor vermelha
            delay(2000);  // Aguarda 2 segundos antes de acender o próximo LED
        }
    }
}
