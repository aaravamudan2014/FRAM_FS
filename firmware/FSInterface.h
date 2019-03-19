#ifndef FSINTERFACE_H_
#define FSINTERFACE_H_
#endif


// following definitions for number of nodes for each partition
// each node has 
#define DIRECTORY_PARTITION 100 
#define FILE_PARTITION 100 
#define METADATA_PARTITION 2 
#define NODE_SIZE 32

struct Volume{
FSInterface FSI;
};

//set up with the default values
struct FSInterface{
char* current_folder_name = "test";
char* files[] = {
    "First file",
    "Second entry",
    "Third entry",
};
char* folders[] = {
    "First directory",
    "Second directory",
    "Third directory",
}; 

};



/**
 * The following functions to be applied to the file system interface
 * Feel free to create new functions as you seem necessary
 **/

/**
 * if new directory name is  "..", will go back one directory
 **/
FSInterface changeDirectory(FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return fsi;
}

FSInterface removeDirectory(FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return fsi;
}

FSInterface removeFile(FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return fsi;
}



char* getContents(FSInterface fsi, char* newDirectoryName){

//TODO

// will return updated FSInterfaceObject
return "";
}





