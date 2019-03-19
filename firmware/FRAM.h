#ifndef FRAM_H_
#define FRAM_H_
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "i2cmaster/twimaster.h"
#define Dev24C02  0xA0


void initiate_i2c(){
    beginSer(9600);
    i2c_init();                                // init I2C interface
    i2c_start(Dev24C02+ I2C_WRITE);
    clearAllMemoryContents();
}
void clearAllMemoryContents(){
    i2c_start_wait(Dev24C02+I2C_WRITE);
    char size[2];
    //starting writing from 
    i2c_write(0x00);
    i2c_write(0x00); 
    for(unsigned int i=0; i < 32768; i++){        
        i2c_write(0x00);   
    }
    println("All memory contents set to zero");
    i2c_stop();
}
void writeToAddress(unsigned int address, char data ){
    i2c_start_wait(Dev24C02+I2C_WRITE);
    i2c_write((0xff00 & address) >> 8);
    i2c_write(0x00ff & address);
    i2c_write(data);
    i2c_stop();
}

void writeSequenceBytes(unsigned int startAddress, char* data ){
    i2c_start_wait(Dev24C02+I2C_WRITE);
    i2c_write((0xff00 & startAddress) >> 8);
    i2c_write(0x00ff & startAddress);
    unsigned int length = strlen(data);
    for( unsigned int i=0; i < length; i++){
        i2c_write(data[i]);
    }

    i2c_stop();
}

char readByteFromAddress(unsigned int address){
    char ret;
    i2c_start_wait(Dev24C02+I2C_WRITE);     // set device address and write mode
    i2c_write((0xff00 & address) >> 8);
    i2c_write(0x00ff & address);
    i2c_rep_start(Dev24C02+I2C_READ);       // set device address and read mode
    ret = i2c_readNak();                    // read one byte
    i2c_stop();
    return ret;
}


//TO BE CHANGED TO PREVENT MEMORY LEAK
char* readSequenceFromAddress(unsigned int address, int length){
    char* ret = (char*)malloc(length*sizeof(char));
    int cnt = 0;
    i2c_start_wait(Dev24C02+I2C_WRITE);     // set device address and write mode
    i2c_write((0xff00 & address) >> 8);
    i2c_write(0x00ff & address);
    i2c_rep_start(Dev24C02+I2C_READ);       // set device address and read mode
    while(cnt < length - 1){
    *(ret + cnt) = i2c_readAck();                    // read one byte
    cnt++;
    }
    *(ret + cnt) = i2c_readNak();                    // read one byte
    i2c_stop();
    return ret;
}