#include <Arduino.h>

int value = 0;
int delay_ms = 5;

bool TryParseASCIICommand(char input) {
    constexpr int inputBufferLength = 256;
    static char inputBuffer[inputBufferLength];
    static int bufferIndex = 0;

    if (input == '\r') return false;
    if (input == '\n') {
        inputBuffer[bufferIndex] = '\0';
        bufferIndex = 0;
        if (inputBuffer[0] == '+') {
            for(int i = 0; inputBuffer[i] == '+'; i++){
                value += 5;
                value = constrain(value, 0, 255);
                delay(500);
            }
        }
        else if (inputBuffer[0] == '-') {
            for(int i = 0; inputBuffer[i] == '-'; i++){
                value -= 5;
                value = constrain(value, 0, 255);
                delay(500);
            }
        }
        Serial.printf("Value: %d\n", value);

        return true;   
    }

    inputBuffer[bufferIndex++] = input;
    if (bufferIndex >= inputBufferLength) {
        bufferIndex = 0;
    }
    return false;
}

/// @brief Parses the serial input from the user char-by-char and posts the result to the event loop.
/// The use of the event loop allows the serial parser to be decoupled from the receiver of the parsed data.
void SerialTask(void *parameter) {

    while (true) {
        if (!Serial.available()) {
            vTaskDelay(pdMS_TO_TICKS(100));
            continue;
        }

        char input = Serial.read();
        TryParseASCIICommand(input);
    }
}


