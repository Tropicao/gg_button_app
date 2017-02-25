#ifndef USBMANAGER_H
#define USBMANAGER_H

#include <QObject>
#include <QTimer>
#include "libusb.h"


class UsbManager : public QObject
{
    Q_OBJECT
public:
    explicit UsbManager(QObject *parent = 0);
    ~UsbManager();
    void openDevice(libusb_device *dev);
signals:
    void deviceStatusChanged(bool status);

public slots:
    void checkDevicePresence(void);
    void sendSyncStatusRequest(void);
    void sendAsyncStatusRequest(void);
private:
    libusb_context *m_ctx;
    libusb_device_handle *m_deviceHandle;
    QTimer *m_devicePollingTimer;
    QTimer *m_getSwitchStateTimer;
    bool m_deviceConnected;
};

#endif // USBMANAGER_H
