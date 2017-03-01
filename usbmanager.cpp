#include "usbmanager.h"
#include "common.h"


#define GG_BUTTON_VID                       0x16c0
#define GG_BUTTON_PID                       0x05dc

#define DEVICE_POLLING_PERIOD_MS            1000
#define DEVICE_STATUS_POLLING_PERIOD_MS     10
#define DEVICE_STATUS_RESPONSE_TIMEOUT_MS   50

/* General values, defined by HID specification */
#define HID_REQ_GET_REPORT                  0x1
#define HID_REPORT_TYPE_IN                  0x1

/* Custom request implementation */
#define SWITCH_GET_REPORT                   HID_REQ_GET_REPORT
#define SWITCH_REQUEST_TYPE                 (HID_REPORT_TYPE_IN<<8)
#define SWITCH_REQUEST_ID                   0x00

#define SWITCH_STATE_REQUEST_SIZE           0x0008
#define SWITCH_STATE_REQUEST_INTERFACE      0x0000

UsbManager::UsbManager(QObject *parent) : QObject(parent)
{
    libusb_error error;

    if((error = (libusb_error)libusb_init(&m_ctx)) != 0)
    {
        DBG("Cannot initialize usb : %s", libusb_strerror(error));
    }
    libusb_set_debug(m_ctx, LIBUSB_LOG_LEVEL_NONE);
    m_deviceConnected = false;
    m_getSwitchStateTimer = new QTimer();
    connect(m_getSwitchStateTimer, SIGNAL(timeout()), this, SLOT(sendSyncStatusRequest()));

    m_switchState = 0;

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

void UsbManager::openDevice(libusb_device *dev)
{
    libusb_error error;
    if(!dev)
    {
        DBG("Cannot open usb device : device structure is empty");
        return;
    }

    error = (libusb_error)libusb_open(dev, &m_deviceHandle);
    if(error)
    {
        DBG("Error opening usb device : %s", libusb_strerror(error));
        return;
    }

    error = (libusb_error)libusb_claim_interface(m_deviceHandle, SWITCH_STATE_REQUEST_INTERFACE);
    if(error)
    {
        DBG("Cannot claim interface : %s", libusb_strerror(error));
        libusb_close(m_deviceHandle);
        return;
    }

    DBG("Device properly opened and claimed, starting status polling");
    m_getSwitchStateTimer->start(DEVICE_STATUS_POLLING_PERIOD_MS);
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

        if(desc.idVendor == GG_BUTTON_VID && desc.idProduct == GG_BUTTON_PID)
        {
            deviceFound = true;
            if(!m_deviceConnected)
            {
                DBG("Device found !");
                m_deviceConnected = true;
                emit deviceConnectionStatusChanged(m_deviceConnected);
                openDevice(dev);
            }
        }
        else
        {
            libusb_unref_device(dev);
        }
    }

    if(!deviceFound && m_deviceConnected)
    {
        DBG("Device disconnected")
        m_deviceConnected = false;
        emit deviceConnectionStatusChanged(m_deviceConnected);
        m_getSwitchStateTimer->stop();

    }
    libusb_free_device_list(devs, 0);
}

void UsbManager::sendSyncStatusRequest()
{
    int size = 0;;
    uchar data[64] = {0};

    if(!m_deviceHandle)
    {
        DBG("Cannot send request : usb device is not opened");
        return;
    }
    size = (libusb_error)libusb_control_transfer(m_deviceHandle,
                                    LIBUSB_ENDPOINT_IN|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE,
                                    SWITCH_GET_REPORT,
                                    SWITCH_REQUEST_TYPE|SWITCH_REQUEST_ID,
                                    SWITCH_STATE_REQUEST_INTERFACE,
                                    data,
                                    SWITCH_STATE_REQUEST_SIZE,
                                    DEVICE_STATUS_RESPONSE_TIMEOUT_MS);
    if(size <= 0)
    {
        if(size < 0)
        {
            DBG("Cannot send request : %s", libusb_strerror((libusb_error)size));
        }
        else
        {
            DBG("Cannot parse data : received 0 byte");
        }
        return;
    }

    /* Expected format : a singly byte equal to 1 if switch pushed, otherwise 0 */
    if(data[0] && !m_switchState)
    {
        DBG("Switched pressed !")
        m_switchState = 1;
        emit switchPressed();
    }
    else if(!data[0] && m_switchState)
    {
        DBG("Switch released");
        m_switchState = false;
    }
}


static void _switchStateRequestCb(struct libusb_transfer *transfer)
{
    if(!transfer)
    {
        DBG("Cannot process request result : transfer is empty");
        return;
    }

    switch(transfer->status)
    {
        case LIBUSB_TRANSFER_COMPLETED:
            DBG("Transfer completed successfully");
            break;
        case LIBUSB_TRANSFER_TIMED_OUT:
            DBG("Error encountered during transfer");
            break;
        case LIBUSB_TRANSFER_ERROR:
            DBG("Error encountered during transfer");
            break;
        case LIBUSB_TRANSFER_CANCELLED:
            DBG("Error encountered during transfer");
            break;
        default:
            DBG("Unprocessed status");
            break;
    }

    libusb_free_transfer(transfer);
}

void UsbManager::sendAsyncStatusRequest()
{
    struct libusb_transfer *transfer = NULL;
    unsigned char setup_packet[8] = {0};
    libusb_error error;

    if(!m_deviceHandle)
    {
        DBG("Cannot send request : usb device is not opened");
        return;
    }

    transfer = libusb_alloc_transfer(0);
    libusb_fill_control_setup(setup_packet,
                              LIBUSB_ENDPOINT_IN|LIBUSB_REQUEST_TYPE_CLASS|LIBUSB_RECIPIENT_INTERFACE,
                              SWITCH_GET_REPORT,
                              SWITCH_REQUEST_TYPE|SWITCH_REQUEST_ID,
                              SWITCH_STATE_REQUEST_INTERFACE,
                              SWITCH_STATE_REQUEST_SIZE);
    libusb_fill_control_transfer(transfer,
                                 m_deviceHandle,
                                 setup_packet,
                                 (libusb_transfer_cb_fn)_switchStateRequestCb,
                                 NULL,
                                 DEVICE_STATUS_RESPONSE_TIMEOUT_MS);
    if((error = (libusb_error)libusb_submit_transfer(transfer)) != 0)
    {
        DBG("Did not submit transfer : %s", libusb_strerror(error));
        libusb_free_transfer(transfer);
    }
}
