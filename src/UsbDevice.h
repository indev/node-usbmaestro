#ifndef USBDEVICE_H
#define USBDEVICE_H

#include "protocol.h"
#include <libusb.h>

class UsbDevice
{
    public:
        UsbDevice();
        virtual ~UsbDevice();

        bool ConnectToDevice();

        void Dispose();

        bool SetTarget( char channel, u16 target );

        inline void SetVerbose( bool verbose ) { this->verbose = verbose; }

    protected:

        libusb_context *m_pContext;
        libusb_device_handle *m_pDeviceHandle;

    private:

        bool verbose;
};

#endif // USBDEVICE_H
