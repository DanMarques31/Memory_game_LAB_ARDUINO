// Definindo os pinos do Arduino para cada deslocador
#define pino_entrada1 3
#define clock_registrador_saida 5
#define clock_registrador_deslocamento 4
#define clock_registrador_saida2 6
#define clock_registrador_deslocamento2 7
#define clock_registrador_saida3 8
#define clock_registrador_deslocamento3 9

// Definindo o pino do botão
#define BUTTON_PIN 2

// Definindo as cores
#define RED 0b10000000
#define GREEN 0b01000000
#define BLUE 0b00100000
#define YELLOW 0b11000000
#define PURPLE 0b10100000
#define CYAN 0b01100000

// Definindo o número de LEDs
#define NUM_LEDS 6

// Criando um array para armazenar as cores dos LEDs
byte ledColors[NUM_LEDS];

// Variável para armazenar a cor selecionada
byte selectedColor;

// Variável para armazenar a posição do LED selecionado
int selectedLed;

// Variável para armazenar o estado do jogo (se está rodando ou não)
bool isGameRunning = false;

// Variável para armazenar o último estado do botão
bool lastButtonState = HIGH;

// Variável para armazenar o último tempo que o botão mudou de estado
unsigned long lastDebounceTime = 0;

// Tempo de debounce em milissegundos
unsigned long debounceDelay = 50;

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

void clearLeds() {
    // Desliga todos os LEDs
    for (int i = 0; i < NUM_LEDS; i++) {
        shiftOut(pino_entrada1, clock_registrador_deslocamento, 0);
        shiftOut(clock_registrador_saida2, clock_registrador_deslocamento2, 0);
        shiftOut(clock_registrador_saida3, clock_registrador_deslocamento3, 0);
    }

    // Atualiza os LEDs
    digitalWrite(clock_registrador_saida, HIGH);
    digitalWrite(clock_registrador_saida, LOW);
    digitalWrite(clock_registrador_saida2, HIGH);
    digitalWrite(clock_registrador_saida2, LOW);
    digitalWrite(clock_registrador_saida3, HIGH);
    digitalWrite(clock_registrador_saida3, LOW);
}

void lightLed(int index, byte color) {
    // Usa a função clearLeds para desligar todos os LEDs
    clearLeds();

    // Acende o LED na posição 'index' com a cor 'color'
    shiftOut(pino_entrada1, clock_registrador_deslocamento, (color & RED) ? 0b10000000 : 0);
    shiftOut(clock_registrador_saida2, clock_registrador_deslocamento2, (color & GREEN) ? 0b10000000 : 0);
    shiftOut(clock_registrador_saida3, clock_registrador_deslocamento3, (color & BLUE) ? 0b10000000 : 0);

    // Atualiza os LEDs
    digitalWrite(clock_registrador_saida, HIGH);
    digitalWrite(clock_registrador_saida, LOW);
    digitalWrite(clock_registrador_saida2, HIGH);
    digitalWrite(clock_registrador_saida2, LOW);
    digitalWrite(clock_registrador_saida3, HIGH);
    digitalWrite(clock_registrador_saida3, LOW);
}


void startGame() {
    // Gera cores aleatórias para cada LED
    for (int i = 0; i < NUM_LEDS; i++) {
        ledColors[i] = random(1, 8) << 5;
    }

    // Acende todos os LEDs com as cores geradas por 4 segundos
    for (int i = 0; i < NUM_LEDS; i++) {
        lightLed(i, ledColors[i]);
    }
    delay(4000);  // Tempo total de 4 segundos

    // Seleciona uma cor e uma posição aleatoriamente
    selectedColor = ledColors[random(NUM_LEDS)];
    selectedLed = random(NUM_LEDS);

    // Indica que o jogo começou
    isGameRunning = true;
}

void testLeds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        lightLed(i, RED);  // Teste com a cor vermelha
        delay(1000);       // Aguarda 1 segundo antes de passar para o próximo LED
    }
    clearLeds();  // Desliga todos os LEDs no final do teste
}

void setup() {
    // Configura os pinos do Arduino
    pinMode(pino_entrada1, OUTPUT);
    pinMode(clock_registrador_saida, OUTPUT);
    pinMode(clock_registrador_deslocamento, OUTPUT);
    pinMode(clock_registrador_saida2, OUTPUT);
    pinMode(clock_registrador_deslocamento2, OUTPUT);
    pinMode(clock_registrador_saida3, OUTPUT);
    pinMode(clock_registrador_deslocamento3, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Inicia a geração de números aleatórios
    randomSeed(analogRead(0));

    testLeds();
}

void loop() {
    // Leitura do estado atual do botão
    int reading = digitalRead(BUTTON_PIN);

    // Verifica se o botão mudou de estado
    if (reading != lastButtonState) {
        // Reinicia o tempo de debounce
        lastDebounceTime = millis();
    }

    // Se o botão for pressionado, iniciar o jogo após 2 segundos
    if ((millis() - lastDebounceTime) > debounceDelay && !isGameRunning && reading == LOW) {
        delay(2000);  // Aguarda 2 segundos antes de iniciar o jogo
        startGame();
    }

    // Atualiza o estado do botão
    lastButtonState = reading;

    // Se o jogo estiver rodando, acende os LEDs em sequência
    if (isGameRunning) {
        for (int i = 0; i < NUM_LEDS; i++) {
            lightLed(i, (i == selectedLed) ? selectedColor : 0);
            delay(2000);  // Aguarda 2 segundos antes de acender o próximo LED

            // Se o botão for pressionado enquanto o LED selecionado estiver aceso, o jogador ganha
            if (digitalRead(BUTTON_PIN) == LOW && i == selectedLed) {
                // Acende todos os LEDs para indicar que o jogador ganhou
                for (int i = 0; i < NUM_LEDS; i++) {
                    lightLed(i, 0b11100000);  // Acende o LED na cor branca
                }
                delay(3000);  // Aguarda 3 segundos antes de apagar os LEDs

                // Para o jogo
                isGameRunning = false;
                break;
            }
        }
    }
}