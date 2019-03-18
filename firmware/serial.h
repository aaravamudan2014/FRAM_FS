#ifndef SERIAL_H_
#define _SERIAL_H_
#endif

#include <avr/io.h>
#include<stddef.h>
#include<stdio.h>
#include<string.h>

#define MAX_BUFFER_SIZE 62


// Receiver Buffer Queue

unsigned char receiverByteArray[MAX_BUFFER_SIZE];
unsigned char receiverFront = 0;
unsigned char receiverRear = -1;
unsigned char receiverItemCount = 0;


// Sender (write) Buffer Queue -- NOT needed as data is sent once received (single threaded)

unsigned char senderByteArray[MAX_BUFFER_SIZE];
unsigned char senderFront = 0;
unsigned char senderRear = -1;
unsigned char senderItemCount = 0;


void updateReadBuffer(){
// keep reading receive buffer until empty
// read uart and output and insert into queue


}

void usartSend (unsigned char byte){
    while (!( UCSR0A & (1<<UDRE0))); 			// wait while register is free
	UDR0 = byte; 
}

void USART_Flush( void ) {
unsigned char dummy;
while ( UCSR0A & (1<<RXC0) ) dummy = UDR0; 
}

unsigned char usartReceive (){
    USART_Flush();
    /* Wait for data to be received */ 
    while (!(UCSR0A & (1<<RXC0)));
    /* Get and return received data from buffer */ 
    return (unsigned char)UDR0;	
}


/*
* returns 1 If the serial connection is open, else return 0
* check if receiver and transmitter are enabled
* UCSR0A
*      RXC0
*      TXCN0
*/
int If(){
    return (UCSR0A && (1 << RXC0)) ||(UCSR0A && (1 << TXC0));
}

/**
 * returns number of bytes available for reading from serial port
 * 
 */
int available(void){
    updateReadBuffer();
    return receiverItemCount;
}

/**
 * returns number of bytes available for writing in serial buffer without blocking write operation
*/
int availableForWrite(void){
    return MAX_BUFFER_SIZE - senderItemCount;
}

void usart_flush(){
    unsigned char dummy;
    while(UCSR0A & 1 << RXC0) dummy = UDR0;
}

/*
* sets the baud rate and sets the parity and stop bits
*/
void beginSer(long speed){
    long BAUDRATE = ((F_CPU)/(speed*16UL)-1);
	UBRR0H=(BAUDRATE>>8);
	UBRR0L=BAUDRATE;							//set baud rate
	UCSR0B|=(1<<TXEN0)|(1<<RXEN0); 			//enable receiver and transmitter
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);        // 8bit data format

}

/**
 * disables serial communcation, freeing RX and TX pins
 */
void end(void){
    UCSR0B= 0x00; 			    //disable receiver and transmitter
}


/**
 * returns 1 if data is found in serial (receive) buffer, 0 if not found
 */
int find(unsigned char data){
    updateReadBuffer();
    for( size_t i = 0; i < receiverItemCount; i++){
        if(receiverByteArray[(i + receiverFront) % receiverItemCount] == data)
            return 1;
    }

    return 0;
}


/**
 * returns 1 if data is found in serial buffer until terminal char else returns 0
 */
int findUntil(unsigned char target, unsigned char terminal){
    updateReadBuffer();
    for( size_t i = 0; i < receiverItemCount; i++){
        if(receiverByteArray[(i + receiverFront) % receiverItemCount] == terminal)
            break;
        if(receiverByteArray[(i + receiverFront) % receiverItemCount] == target)
            return 1;
    }
    return 0;
}


/**
 * waits until transmission of outgoing serial data to complete
 */
void flush(void){
    while (!( UCSR0A & (1<<UDRE0))); 
}

/**
 * returns the first valid floating point number from serial (receive) buffer, 
 * terminates when encountered first non float character
 */
float parseFloat(void){
    updateReadBuffer();
    float val = 0.0;
    int shiftVal = 1;
    int byteCount = 0;
    for( size_t i = 0; i < receiverItemCount; i++){
        // check if it is a digit
        if( receiverByteArray[(i + receiverFront) % receiverItemCount]>47 
        && receiverByteArray[(i + receiverFront) % receiverItemCount] < 58){
            //val |= receiverByteArray[i] << 
            byteCount++;
        }
        if (byteCount == 4)
            break;
    }
    return 0.0;
}


/**
 * returns the first valid int number from serial buffer, 
 * terminates when encountered first non float character
 */
int parseInt(void){
    updateReadBuffer();
    int val = 0.0;
    int shiftVal = 1;
    int byteCount = 0;
    for( size_t i = 0; i < receiverItemCount; i++){
            // check if it is a digit
            if( receiverByteArray[(i + receiverFront) % receiverItemCount]>47 
            && receiverByteArray[(i + receiverFront) % receiverItemCount] < 58){
                //val |= receiverByteArray[i] << 
                byteCount++;
            }
            if (byteCount == 4)
                break;
    }
}


/**
 * returns next byte of incoming serial data without removing it from serial buffer
*/
unsigned char peek(void){
    updateReadBuffer();
    return receiverByteArray[receiverFront];
}


/**
 * print passed data as ascii characters(for all data types)
 */
void print(char* printVal){
    for(int i=0; i< strlen(printVal); i++){
        usartSend((unsigned char)printVal[i]);
    }
}


void printChar(char charVal){
    usartSend(charVal);
}

// TODO: add overloaded functions for serial
/**
 * print passed data as ascii characters(for all data types) followed by carriage print,
 *  along with data type interpretation
 */
void println(unsigned char* printVal){
    print(printVal);
    usartSend('\n');
}


/**
 * reads incoming serial data, first byte available
*/
unsigned char read(void){
    updateReadBuffer();
    return receiverByteArray[receiverFront];
}

/**
 * returns number of characters placed in the buffer
 */
size_t readBytes(unsigned char* buffer, int length){
    updateReadBuffer();
    if( length < receiverItemCount)
        return length;
    else
        return receiverItemCount;
}

/**
 * reads number of characters read into the buffer until a terminator character is found
 */
size_t readBytesUntil(unsigned char data, unsigned char* buffer, int length){
updateReadBuffer();
}


/**
 * Reads characters from the serial buffer into a string , function terminates if times out
*/
unsigned char* readString(void){
    updateReadBuffer();

}

/**
 * reads characters from serial buffer into string, terminates if terminator character is found
*/
unsigned char* readStringUntil(unsigned char terminator){
    updateReadBuffer();
}

/**
 * sets the maximum milliseconds to wait for serial data when reading
 */
void setTimeout(void){

}

/**
 * writes binary data into the serial port, returns number of  bytes written
 */
size_t write(unsigned char* buffer){

}



