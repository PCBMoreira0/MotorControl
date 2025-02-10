// #include <Arduino.h>
// //esquerda - bombordo
// //direita - boreste

// const int bb_dac_pin = GPIO_NUM_25;
// const int br_dac_pin = GPIO_NUM_26;

// const int pot_pin = GPIO_NUM_33;

// const int pot_max_bit = 4096;
// const int dac_max_bit = 256;
// const int dead_zone_threshold = 668;

// int bb_motor_speed = 0;
// int br_motor_speed = 0;

// int get_velocity_linear(int value){
// 	int mapped_value = map(value, 0, pot_max_bit/2-dead_zone_threshold, 0, 1000);
// 	const float angular_coeficient = 1.0f;
// 	const float linear_coeficient = 0.0f;

// 	int newValue = angular_coeficient * mapped_value + linear_coeficient;
// 	return map(newValue, 0, 1000, 0, dac_max_bit-1);
// }

// void setup() {
// 	Serial.begin(115200);

// 	// CREATE_TASK(SerialTask, STACK_SIZE(4096), PRIORITY(1));
// }

// void loop() {
// 	// int pot = analogRead(pot_pin);
// 	// if((pot > (pot_max_bit/2 - dead_zone_threshold)) && (pot < (pot_max_bit/2 + dead_zone_threshold))){
// 	// 	Serial.printf("Morta %d\n", 255);	
// 	// 	dacWrite(bb_dac_pin, 255);
// 	// 	dacWrite(br_dac_pin, 255);
// 	// }
// 	// else if(pot < (pot_max_bit/2 - dead_zone_threshold)){
// 	// 	int bb_value = get_velocity_linear(pot);
// 	// 	Serial.printf("BB: %d\n", bb_value);
// 	// 	Serial.printf("BR: %d\n\n", 255);
// 	// 	dacWrite(bb_dac_pin, bb_value);
// 	// 	dacWrite(br_dac_pin, 255);
// 	// }
// 	// else{
// 	// 	int br_value = get_velocity_linear((pot_max_bit-1)-pot);
// 	// 	Serial.printf("BB: %d\n", 255);
// 	// 	Serial.printf("BR: %d\n\n", br_value);
// 	// 	dacWrite(bb_dac_pin, 255);
// 	// 	dacWrite(br_dac_pin, br_value);
// 	// }

	
// }	

