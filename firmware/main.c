
#define Dev24C02  0xA0

#include<avr/io.h>
#include "FRAM.h"
#include "FSInterface.h"
//Adafruit_FRAM_I2C fram     = Adafruit_FRAM_I2C();

int main(void)
{   beginSer(9600);
    unsigned char ret;
    initiate_i2c();
    println("Writing to address 32: abcde");
    char* test = "abcde";
    writeSequenceBytes(32, test);
    ret = readByteFromAddress(32);
    test = readSequenceFromAddress(32,5);
    println(test);
    println("Write complete ");
    println("FRAM operations complete");
    for(;;);	

}
