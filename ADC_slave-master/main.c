// main.c
//  HVKHVK
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>

#include "usitwislave_devicesadc.h"
#include "usitwislaveadc.h"
#include "usitwislaveadc.c"
#include "main.h"

#define DEVICE_ID 0x05 //Device Id, the I2C connection adress
#define F_CPU 8000000  //Clock Frequency of Attiny85


int sum;
int value;
uint8_t result;
int state = 0;

void blinkcheck(int no){
    int i = 0;
    while( i < no){
        PORTB |= (1 << PB3);
        _delay_ms(20);
        PORTB &= ~(1 << PB3);
        _delay_ms(20);
        i++;
    }
}

void ADC_enable(){
    
    
    ADMUX =            //Attiny85 Datasheet page 134
    (1 << ADLAR) |     //Left shifts the result, for one byte
    (0 << REFS0) |     //Referance to input voltage values
    (0 << REFS1) |
    
    (0 << MUX3)  |     //use ADC2 for input (PB4), MUX bit 3
    (0 << MUX2)  |     //use ADC2 for input (PB4), MUX bit 2
    (1 << MUX1)  |     //use ADC2 for input (PB4), MUX bit 1
    (0 << MUX0);       //use ADC2 for input (PB4), MUX bit 0
    
    ADCSRA =           //Attiny85 Datasheet page 136
    (1 << ADPS2) |     //set prescaler to 64, bit 2
    (1 << ADPS1) |     //set prescaler to 64, bit 1
    (0 << ADPS0);      //set prescaler to 64, bit 0
                       //Attiny85 adc works better at 125KHz. Make it 128 if you use 16MHZ clock
 }

void get_voltage(){
    sum = 0;
    ADCSRA |= (1 << ADEN);          // Enable ADC
    
    int sample;
    for(sample = 0 ; sample < 10; sample++)
    {
        ADCSRA |= (1 << ADSC);          // start ADC measurement
        while (ADCSRA & (1 << ADSC) );  // wait till conversion complete
        
    }
    for(sample = 0 ; sample < 10; sample++)
    {
        ADCSRA |= (1 << ADSC);          // start ADC measurement
        while (ADCSRA & (1 << ADSC) );  // wait till conversion complete
        
        sum = sum + ADCH;
    }
    ADCSRA &= ~(1 << ADEN);       //Disable ADC
    
    value = sum/10;
    result = (uint8_t)value;
    state = 1;
}

static void twi_callback(uint8_t input_buffer_length, const uint8_t *input_buffer,
                         uint8_t *output_buffer_length, uint8_t *output_buffer)
{
    if(input_buffer_length > 0){
        //Do nothing
        }
    else{
        *output_buffer_length = 8;
        switch(state)
        {
            case(1):{ // Sending the ADC value
                *output_buffer = result;
                state = 0;
                blinkcheck(1);
                break;
            }
            case(0):{ // Sending device id
                *output_buffer = DEVICE_ID;
                break;
            }
        }
    }
}

int main (void)
{
    DDRB |= ( 1 << PB3); //set PB3 output. For LED.
    PORTB &= ~(1 << PB3); //set PB3 value 0
    
    ADC_enable();
    
    usi_twi_slave(DEVICE_ID, 1, *twi_callback, 0);
    return 1; // No need
}

