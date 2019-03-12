#include <iostream>
struct elf_node_abstract{
     uint16_t version;
    uint32_t beginOffset; 
    uint32_t locaLen; 
    uint16_t startPg : 11; 
    int16_t flag : 5;
    struct elf_node_abstract *next; 
}__attribute__((packed));

typedef struct elf_node_abstract elf_node_abstract_t;


struct elf_file_abstract{ 
    uint16_t version;
    uint16_t inodeNumber; 
    uint32_t totaLen;
    uint8_t flag;
    uint8_t writeCounter : 4;
    uint8_t readCounter : 4; 
    uint16_t pInode; 
    elf_node_abstract_t *first;
}__attribute__((packed));


void open();
void close();
void read();
void write();
void lseek();
void delete_();
void truncate();
void mkdir();
void rmdir();