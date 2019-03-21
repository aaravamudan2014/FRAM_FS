
#define Dev24C02  0xA0


#include "FSInterface.h"

int main(void)
{   beginSer(9600);

      // check if file system already exists by a flag in the metadata
      // if it doesnt, initiate file system
      // then get the file system state
   initiate_i2c();
   struct FSInterface fsi;
   char* inputString;
   
   clearAllMemoryContents();
   if((unsigned int)readByteFromAddress(0) == 0){
      println("File System not initialized, Intializing File System from scratch");
      fsi= initiateFileSystem();
   }
   
   while(1){
      inputString = receiveString();
      
   }





   //  unsigned char ret;
   //  println("Writing to address 32: abcde")  
   
   //  char* test = "abcde";
   //  writeSequenceBytes(32, test);
   //  ret = readByteFromAddress(32);
   //  test = readSequenceFromAddress(32,5);
   //  println(test);
   //  println("Write complete ");
   //  println("FRAM operations complete");
   //  for(;;);	

}
