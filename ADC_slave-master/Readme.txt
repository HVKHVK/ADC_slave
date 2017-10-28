
Basically, this project gets data from ADC continuously and sends that data to master when asked. 

To be more specific, attiny sets it ADC settings in main.c and then skips to usitwislaveadc.c file. In usitwislaveadc file it continuously checks the I2C communication bus and calculates the ADC value inside the main.c and holds it in a variable inside the main.c. When I2C communication established and master asked for the data, attiny sends the ADC value first and the device id second. 

You can use the Makefile without changing. Master.cpp file is not for the attiny. Raspberry pi used as a master device in this project. 

Slave libraries are used from the 'https://github.com/eriksl/usitwislave'. If you want the original ones you can access from here.
