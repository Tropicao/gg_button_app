#include "usbmanager.h"
#include "common.h"

UsbManager::UsbManager(QObject *parent) : QObject(parent)
{
    libusb_error error;

    if((error = (libusb_error)libusb_init(&m_ctx)) != 0)
        DBG("Cannot initialize usb : %s", libusb_strerror(error));
    libusb_set_debug(m_ctx, LIBUSB_LOG_LEVEL_INFO);
    m_deviceConnected = false;

    if(libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG))
    {
        DBG("Library has hotplug detection capability. Setting callbacks");
        /** @Todo : set callback capability */
    }
    else
    {
        DBG("Library does not support device hotplug. Starting device polling");
        m_devicePollingTimer = new QTimer();
        connect(m_devicePollingTimer, SIGNAL(timeout()), this, SLOT(checkDevicePresence()));
        m_devicePollingTimer->start(DEVICE_POLLING_PERIOD_MS);
    }
}

UsbManager::~UsbManager()
{
    libusb_exit(m_ctx);
}

void UsbManager::checkDevicePresence()
{
    libusb_device **devs = NULL, *dev = NULL;
    libusb_error error;
    int i = 0;
    bool deviceFound = false;


    error = (libusb_error)libusb_get_device_list(m_ctx, &devs);
    if (error < 0)
    {
        DBG("Cannot get device list : %s", libusb_strerror(error));
        return;
    }

    while ((dev = devs[i++]) != NULL)
    {
        struct libusb_device_descriptor desc;
        error = (libusb_error)libusb_get_device_descriptor(dev, &desc);
        if (error < 0)
        {
            DBG("Cannot get device descriptor : %s", libusb_strerror(error));
            return;
        }

        DBG("%04x:%04x (bus %d, device %d)",
            desc.idVendor, desc.idProduct,
            libusb_get_bus_number(dev), libusb_get_device_address(dev));
        if(desc.idVendor == GG_BUTTON_VID && desc.idProduct == GG_BUTTON_PID)
        {
            DBG("GG Button found !");
            deviceFound = true;
            if(!m_deviceConnected)
            {
                m_deviceConnected = true;
                emit deviceStatusChanged(m_deviceConnected);
            }
        }
        else
        {
            libusb_unref_device(dev);
        }
    }

    if(!deviceFound && m_deviceConnected)
    {
        m_deviceConnected = false;
        emit deviceStatusChanged(m_deviceConnected);

    }
    libusb_free_device_list(devs, 0);
}
