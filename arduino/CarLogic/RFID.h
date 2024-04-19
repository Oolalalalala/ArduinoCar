#ifndef RFID_H
#define RFID_H

#include <stdint.h>

class RFID 
{
public:
    static void Initialize();
    static bool HasValue();
    static uint32_t ReadValue();
};

#endif