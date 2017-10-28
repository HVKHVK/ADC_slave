#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>
#include <wiringPiI2C.h>

#define DEVICE_ID 0x05 //Device id

using namespace std;

int main()
{
	int send, data, result, id;
    
    uint8_t adc_hibyte;
    
	send = wiringPiI2CSetup (0x05); //Send device id I2C communicate with the device

    wiringPiI2CWrite (send, 1); //Does not effect anything
    
   
    data = wiringPiI2CRead (send);
    //Ignore the first value
    
    sleep(1);
    data = wiringPiI2CRead (send);
    //Ignore the second value, because of the changes inside the slave library

    sleep(1);

    result = wiringPiI2CRead(send);
    //Now we can get the results
    
    id = wiringPiI2CRead(send);
        

    cout << "ADC: "<< result << endl;
    cout << "ID  : "<< id << endl;
    cout << "\n" << endl;
}

