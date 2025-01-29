#include <Arduino.h>
#include <serialReader.h>

TaskHandle_t SerialTaskHandle = nullptr;

const int dac0_pin = GPIO_NUM_25;

void setup() {
	Serial.begin(115200);

	CREATE_TASK(SerialTask, STACK_SIZE(4096), PRIORITY(1));
}

void loop() {
	dacWrite(dac0_pin, value);
	delay(20);

	// delay(2000);
}	

