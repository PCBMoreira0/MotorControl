#include <Arduino.h> // Main Arduino library, required for projects that use the Arduino framework.
#include <Adafruit_ADS1X15.h> // 16-bit high-linearity with programmable gain amplifier Analog-Digital Converter for measuring current and voltage.
#include <SPI.h> // Required for the ADS1115 ADC.
#include <Wire.h>

typedef Adafruit_ADS1X15 ADS1115;

//esquerda - bombordo
//direita - boreste

const int bb_dac_pin = GPIO_NUM_25;
const int br_dac_pin = GPIO_NUM_26;

const int pot_pin = GPIO_NUM_33;

const int adc_max_bit = 32767;
const int pot_max_bit = 26399;
const int dac_max_bit = 256;
const int dead_zone_threshold = 3000;

volatile int bb_vel = 0;
volatile int br_vel = 0;

ADS1115 adc; 


int get_velocity_linear(int value){
	int mapped_value = map(value, 0, pot_max_bit/2-dead_zone_threshold, 0, 1000);
	const float angular_coeficient = 1.0f;
	const float linear_coeficient = 0.0f;

	int newValue = angular_coeficient * mapped_value + linear_coeficient;
	return map(newValue, 0, 1000, 0, dac_max_bit-1);
}

float LinearCorrection(const float input_value, const float slope, const float intercept) {
    return slope * input_value + intercept;
}

float adc2voltage(int adc_value){
    return (adc_value/32767.0f)*4.096f;
}

void DirectionTask(void *parameter){
    Wire.setPins(GPIO_NUM_21, GPIO_NUM_22);
    Wire.begin(); // I2C master mode to communicate with the ADS1115 ADC

    constexpr uint8_t adc_addresses[] = {0x48, 0x49}; // Address is determined by a solder bridge on the instrumentation board.
    adc.setGain(GAIN_ONE); // Configuring the PGA( Programmable Gain Amplifier) to amplify the signal by 4 times, so that the maximum input voltage is +/- 1.024V
    adc.setDataRate(RATE_ADS1115_860SPS); // Setting a low data rate to increase the oversampling ratio of the ADC and thus reduce the noise.
    
    bool is_adc_initialized = false;
    
    while (!is_adc_initialized) {
        for (auto address : adc_addresses) {
            Serial.printf("\n[ADS]Trying to initialize ADS1115 at address 0x%x\n", address);
            if (adc.begin(address)) {
                Serial.printf("\n[ADS]ADS1115 successfully initialized at address 0x%x\n", address);
                is_adc_initialized = true;
                break;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    while(true){
        int bb_pot = adc.readADC_SingleEnded(0);
        int br_pot = adc.readADC_SingleEnded(2);
        int16_t dir_pot = adc.readADC_SingleEnded(1);

        float speed_porc = (float)bb_pot / pot_max_bit;

        if((dir_pot > (pot_max_bit/2 - dead_zone_threshold)) && (dir_pot < (pot_max_bit/2 + dead_zone_threshold))){
            // Serial.printf("Morta %d\n", 255);	
            bb_vel = 255 * speed_porc;
            br_vel = 255 * speed_porc;
            dacWrite(bb_dac_pin, 255 * speed_porc);
            dacWrite(br_dac_pin, 255 * speed_porc);
        }
        else if(dir_pot < (pot_max_bit/2 - dead_zone_threshold)){
            int bb_value = get_velocity_linear(dir_pot);
            // Serial.printf("BB: %d\n", bb_value);
            // Serial.printf("BR: %d\n\n", 255);
            bb_vel = bb_value * speed_porc;
            br_vel = 255 * speed_porc;
            dacWrite(bb_dac_pin, bb_value * speed_porc);
            dacWrite(br_dac_pin, 255 * speed_porc);
        }
        else{
            int br_value = get_velocity_linear((pot_max_bit-1)-dir_pot);
            // Serial.printf("BB: %d\n", 255);
            // Serial.printf("BR: %d\n\n", br_value);
            bb_vel = 255 * speed_porc;
            br_vel = br_value * speed_porc;
            dacWrite(bb_dac_pin, 255 * speed_porc);
            dacWrite(br_dac_pin, br_value * speed_porc);
        }

        // Serial.printf("\nBombordo: %.2f V\n"
        //                 "Direcao: %d\n"
        //               "Boreste: %.2f V\n"
        //               "Porc: %.2f\n",
        //               adc2voltage(bb_pot), dir_pot, adc2voltage(br_pot), speed_porc);

        vTaskDelay(pdMS_TO_TICKS(1));
    }

    vTaskDelete(NULL);
}

void SerialTask(void *parameter){
    while(true){
        Serial.printf("BB: %d\nBR: %d\n\n", bb_vel, br_vel);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup(){
    Serial.begin(115200);
    xTaskCreate(DirectionTask, "DirTask", 4096, NULL, 1, NULL);
    xTaskCreate(SerialTask, "SerTask", 4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelete(NULL);
}