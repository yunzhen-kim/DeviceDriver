#pragma once
#include "flash_memory_device.h"
#include <exception>

class ReadFailException : public std::exception {
public:
    char const* what() const {
        return "Read failed because read data is not same 5 times";
    }
};

class WriteFailException : public std::exception {
public:
    char const* what() const {
        return "Write failed because it has valid data";
    }
};

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);

protected:
    FlashMemoryDevice* m_hardware;
};
