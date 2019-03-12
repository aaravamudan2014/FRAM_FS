#define F_CPU 1000000
#define SCL_CLOCK 50000
#define BAUD 9600
#define MYUBRR    F_CPU/8/BAUD-1
#define TWIMUX  0x1C 


#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <util/twi.h>
#include <avr/interrupt.h>


void initiate_i2c(){
    TWSR = 0;                         // no prescaler
     TWBR = ((F_CPU/SCL_CLOCK)-16)/2; 
     TWCR = (1<<(TWINT))|(1<<TWSTA )|(1<<TWEN);
     while (!(TWCR & (1<<TWINT))) ;
     printf("Send Start");
}

int main(void){
    initiate_i2c();
}