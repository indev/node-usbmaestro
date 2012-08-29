#include <stdio.h>
#include "UsbDevice.h"

UsbDevice::UsbDevice()
{
    m_pDeviceHandle = NULL;
    m_pContext = NULL;

    this->verbose = false;
}

UsbDevice::~UsbDevice()
{
    //Dispose();
}

void UsbDevice::Dispose()
{
    if ( m_pDeviceHandle )
    {
        libusb_close(m_pDeviceHandle);
        m_pContext = NULL;
    }
    if ( m_pContext )
    {
        libusb_exit(m_pContext);
        m_pContext = NULL;
    }
#ifdef VERBOSE
    printf("UsbDevice::Dispose - Connected device released\n");
#endif
}

bool UsbDevice::ConnectToDevice()
{
    const unsigned short vendorId = 0x1ffb;
    //unsigned short productIDArray[] = {0x0089, 0x008a, 0x008b, 0x008c};

    libusb_device   **deviceList = NULL;

    libusb_init(&m_pContext);

    u32 numDevices = libusb_get_device_list(m_pContext, &deviceList);

    if ( this->verbose )
        printf("UsbDevice::ConnectToDevice - Found %d devices\n", numDevices);

    m_pDeviceHandle = NULL;
    for ( u32 i = 0; i < numDevices; ++i )
    {
        libusb_device *device = deviceList[i];

        libusb_device_descriptor desc;
        libusb_get_device_descriptor(device, &desc);

        // Should check product if here aswell, but for now we assume we only
        // have one device from this vendor connected
        if ( desc.idVendor == vendorId )
        {
            if ( this->verbose )
                printf("UsbDevice::ConnectToDevice - Found device with matching vendor ID\n");

            libusb_open(device, &m_pDeviceHandle);
            if ( m_pDeviceHandle != NULL )
            {
                if ( this->verbose )
                    printf("UsbDevice::ConnectToDevice - Device opened and ready to use\n");

                break;
            }
        }
    }

    // Clean up
    libusb_free_device_list(deviceList, 0);

    if ( m_pDeviceHandle == NULL )
    {
        if ( this->verbose )
            printf("UsbDevice::ConnectToDevice - Found no matching device\n");

        return false;
    }

    return true;
}

/**
 *  Set the target on the device channel
 *  Channel number range from 0 to 23
 *  Target, in units of quarter microseconds.  For typical servos,
 *  6000 is neutral and the acceptable range is 4000-8000.
 **/
bool UsbDevice::SetTarget( char channel, u16 target )
{
    if ( m_pDeviceHandle == NULL || m_pContext == NULL )
    {
        if ( this->verbose )
            printf("UsbDevice::SetTarget - No valid device open\n");

        return false;
    }

    if ( this->verbose )
        printf("UsbDevice::SetTarget - Try to set target %d on channel %d\n", target, channel);

    
    libusb_control_transfer(m_pDeviceHandle, 0x40, REQUEST_SET_TARGET, target * 4, channel, 0, 0, (ushort)5000);

    return true;
}