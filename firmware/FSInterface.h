#ifndef FSINTERFACE_H_
#define FSINTERFACE_H_
#endif


// following definitions for number of nodes for each partition
// each node has 
#define DIRECTORY_PARTITION 108
#define FILE_PARTITION 866 
#define METADATA_PARTITION 50 
#define NODE_SIZE 32
#include "FRAM.h"






//set up with the default values
struct FSInterface{
char *current_folder_name;
unsigned int folder_address_start;
unsigned int folder_address_end;
};


// working under the assumption that the file system doesnt already exist and creating a new one.
struct FSInterface initiateFileSystem(){



// turn on flag in file sytsem that indicates that the file system has been initiated. 
writeToAddress(0,1);


//initiate numFiles to zero
writeToAddress(1,0);


//initiate numDirectories to 1
writeToAddress(2,1);


//initiate nextFileAdress to file PartitionStart
unsigned int firstFileAdress = (METADATA_PARTITION +DIRECTORY_PARTITION)*32;
char lsb = (0x00ff & firstFileAdress);
char msb = (0xff00 & firstFileAdress) >> 8;
writeToAddress(3,msb);
writeToAddress(4,lsb);



//initiate nextDirAdress to folder PartitionStart
unsigned int firstDirAdress = (METADATA_PARTITION)*32;
lsb = (0x00ff & firstDirAdress);
msb = (0xff00 & firstDirAdress) >> 8;
writeToAddress(5,msb);
writeToAddress(6,lsb);


//intitiate nextMetaData address to the one right after these initialization.
unsigned int firstMetaDataAddress = 9;
lsb = (0x00ff & firstMetaDataAddress);
msb = (0xff00 & firstMetaDataAddress) >> 8;
writeToAddress(7,msb);
writeToAddress(8,lsb);


//create a root Directory called "root"
unsigned int numEntities = 0;
unsigned int root_dir = 1;
// metadata is root_dir + filename + start address + end address
char metadata[15];
char* filename = "root";
unsigned int startAddress = firstDirAdress;
unsigned int endAddress = firstDirAdress;

int size = strlen(filename);
metadata[0] = root_dir;
strcat(metadata,filename);
for(int i=1; i< 11; i++){
    if(i <= size){
        metadata[i] = filename[i-1];
    }else
    {
        metadata[i] = " ";
    }
    
    
}
lsb = (0x00ff & startAddress);
msb = (0xff00 & startAddress) >> 8;
metadata[11] = msb;
metadata[12] = lsb;
lsb = (0x00ff & endAddress);
msb = (0xff00 & endAddress) >> 8;
metadata[13] = msb;
metadata[14] = lsb;
msb = readByteFromAddress(7);
lsb = readByteFromAddress(8);


// if(readByteFromAddress(24) == 0){
//     println("zero");
// }
// else{
//     println("non zero");
// }
writeSequenceBytes((int)(lsb | (msb << 8)), metadata);

lsb = (0x00ff & (endAddress+1));
msb = (0xff00 & (endAddress+1)) >> 8;
writeToAddress(5, msb);
writeToAddress(6, msb);


//update first metadata address
msb = readByteFromAddress(7);
lsb = readByteFromAddress(8);
firstMetaDataAddress =(int) (lsb | (msb << 8));
lsb = (0x00ff & (firstMetaDataAddress+15));
msb = (0xff00 & (firstMetaDataAddress+15)) >> 8;
writeToAddress(7, msb);
writeToAddress(8, lsb);

}

struct FSInterface getFileSystem(){

// read from FRAM and get number of files


// read from FRAM and get number of directories.

struct FSInterface fsi;

    

    return fsi;
}

void createDirectory(char* dirName, char** entityNames){

    // get next directory start address
    
    char lsb = readByteFromAddress(5);
    char msb = readByteFromAddress(6);
    unsigned int dirAddress= (msb << 8 ) + lsb;
    unsigned int numEntities = 0;
    unsigned int root_dir;

    // add info to metadata

    // add entity names to directory data



}


struct FSInterface createFile(char* fileName, char* fileContent, struct FSInterface fsi){
    //get next File start address
    char msb = readByteFromAddress(3);
    char lsb = readByteFromAddress(4);
    unsigned int FileAddress= (msb << 8 ) + lsb;

    // get metadata address
    msb = readByteFromAddress(7);
    lsb = readByteFromAddress(8);
    unsigned int metadataStartAddress = (lsb | (msb << 8));
    char metadata[15];

    int size = strlen(fileName);
    metadata[0] = 2; // non root and is a file
    strcat(metadata,fileName);
    for(int i=1; i< 11; i++){
        if(i <= size){
            metadata[i] = fileName[i-1];
        }else{
            metadata[i] = " ";
        }
    }
    lsb = (0x00ff & FileAddress);
    msb = (0xff00 & FileAddress) >> 8;
    metadata[11] = msb;
    metadata[12] = lsb;
    lsb = (0x00ff & (FileAddress + sizeof(fileContent)));
    msb = (0xff00 & (FileAddress + sizeof(fileContent))) >> 8;
    metadata[13] = msb;
    metadata[14] = lsb;
    msb = readByteFromAddress(7);
    lsb = readByteFromAddress(8);

    writeSequenceBytes((int)(lsb | (msb << 8)), metadata);

    lsb = (0x00ff & (metadataStartAddress+sizeof(metadata)));
    msb = (0xff00 & (metadataStartAddress+ sizeof(metadata))) >> 8;
    writeToAddress(7, msb);
    writeToAddress(8, msb);
    

    // append content to file data 
    writeSequenceBytes(FileAddress, fileContent);
    //update nextFileData in metadata
    lsb = (0x00ff & (FileAddress+sizeof(fileContent)+1));
    msb = (0xff00 & (FileAddress+ sizeof(fileContent)+1)) >> 8;
    writeToAddress(3, msb);
    writeToAddress(4, msb);
    // update current directory with data
    unsigned int dirAddressStart = fsi.folder_address_start;
    unsigned int dirAddressEnd = fsi.folder_address_end;
    unsigned int newDirAddressEnd = dirAddressEnd + sizeof(fileName);
    msb = readByteFromAddress(5);
    lsb = readByteFromAddress(6);
    makeSpace(dirAddressStart,sizeof(fileName));
    writeSequenceBytes(dirAddressStart, fileName);
    fsi.folder_address_end =newDirAddressEnd;

    return fsi;

}


// move directory content space to make room for a new filename
void makeSpace(unsigned int startAddress, unsigned int push_size){
    //first find the maximum directory address space allocated
    char msb = readByteFromAddress(5);
    char lsb = readByteFromAddress(6);

    unsigned int maxDirAddress = (unsigned int) (lsb | msb << 8);

    //update new directory start in metadata
    for(unsigned int i=maxDirAddress; i >= startAddress; i--){
        char byte = readByteFromAddress(i);
        writeToAddress(i + push_size ,byte);
    }
    // update the maximum directory address

    lsb = (0x00ff & (maxDirAddress + push_size));
    msb = (0xff00 & (maxDirAddress + push_size) ) >> 8;
    writeToAddress(5,msb);
    writeToAddress(6,lsb); 
    
}
/**
 * The following functions to be applied to the file system interface.
 **/




/**
 * if new directory name is  "..", will go back one directory
 **/
struct FSInterface changeDirectory(struct FSInterface fsi, char* newDirectoryName){

// check for new directory name in the given directory list

//

// will return updated FSInterfaceObject
return fsi;
}

struct FSInterface removeDirectory(struct FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return fsi;
}

struct FSInterface removeFile(struct FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return fsi;
}



char* getContents(struct FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return "";
}





