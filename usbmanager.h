#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QObject>
#include <QTimer>
#include "libusb.h"

#define GG_BUTTON_VID               0x16c0
#define GG_BUTTON_PID               0x05dc
#define DEVICE_POLLING_PERIOD_MS    1000

class UsbManager : public QObject
{
    Q_OBJECT
public:
    explicit UsbManager(QObject *parent = 0);
    ~UsbManager();
signals:
    void deviceStatusChanged(bool status);

public slots:
    void checkDevicePresence(void);
private:
    libusb_context *m_ctx;
    libusb_device_handle *m_ggButton;
    QTimer *m_devicePollingTimer;
    bool m_deviceConnected;
};

#endif // USBMANAGER_H
