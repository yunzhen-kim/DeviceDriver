#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice *hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int ret = (int)(m_hardware->read(address));
    for (int i = 0; i < 4; ++i) {
        int read_data = (int)(m_hardware->read(address));
        if (ret != read_data) throw ReadFailException();
    }
    
    return ret;
}

void DeviceDriver::write(long address, int data)
{
    if (m_hardware->read(address) != 0) throw WriteFailException();
    m_hardware->write(address, (unsigned char)data);
}
