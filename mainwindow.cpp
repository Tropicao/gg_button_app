#include "mainwindow.h"
#include <iostream>
#include "common.h"
#include <QApplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    int size = 0;
    SoundData *data = NULL;

    m_usb = new UsbManager();
    m_sound = new SoundManager();
    connect(m_usb, SIGNAL(switchPressed()), m_sound, SLOT(playDefaultSound()));

    m_settings = new Settings();
    data = m_settings->loadSettings(&size);

    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QIcon(":/resources/resources/gear.png"));
    m_tray->setToolTip(tr("GG Button Manager"));
    connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                            this, SLOT(displayMenu(QSystemTrayIcon::ActivationReason)));
    m_menu = new QMenu(this);

    m_status = new QAction(tr("Disconnected"));
    connect(m_usb, SIGNAL(deviceConnectionStatusChanged(bool)), this, SLOT(setStatus(bool)));
    m_status->setEnabled(false);
    m_menu->addAction(m_status);

    m_menu->addSeparator();
    m_settingsGeneration = new QAction ("Create/flush settings file");
    m_menu->addAction(m_settingsGeneration);
    connect(m_settingsGeneration, SIGNAL(triggered()), m_settings, SLOT(generateIniSkeleton()));

    m_menu->addSeparator();
    buildSoundList(data, size);

    m_menu->addSeparator();
    QAction *quitAction = new QAction(tr("Leave"), this);
    m_menu->addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_tray->setContextMenu(m_menu);

    // Start
    m_tray->show();
}

MainWindow::~MainWindow()
{
    for(int i=0; i<GG_BUTTON_MAX_SOUND_LIB; i++)
        delete(m_soundTab[i]);
    delete(m_status);
    delete(m_menu);
    delete(m_tray);
    delete(m_usb);
    delete(m_sound);

}

void MainWindow::buildSoundList(SoundData *data, int size)
{
    if(!m_menu)
        m_menu = new QMenu();
    m_soundTab = new SoundItem*[GG_BUTTON_MAX_SOUND_LIB];
    for(int i = 0; i < GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        if((data != NULL) && (i < size))
            m_soundTab[i] = new SoundItem(this, &(data[i]));
        else
            m_soundTab[i] = new SoundItem(this);

        connect(m_soundTab[i], SIGNAL(defaultSoundChanged(QString)), m_sound, SLOT(setMainSound(QString)));
        connect(m_soundTab[i], SIGNAL(soundTriggered(QString)), m_sound, SLOT(playSound(QString)));
        connect(m_soundTab[i], SIGNAL(defaultSoundChanged(QString)), this, SLOT(updateSelectedSoundGui(QString)));
        m_menu->insertMenu(NULL, m_soundTab[i]->getMenu());
    }
}

void MainWindow::displayMenu(QSystemTrayIcon::ActivationReason reason)
{
    QPoint pos = QCursor::pos();
    if(reason == QSystemTrayIcon::QSystemTrayIcon::Trigger)
    {
        if(m_tray->contextMenu()->isHidden())
        {
            m_tray->contextMenu()->move(pos.x(), pos.y()-m_tray->contextMenu()->height());
            m_tray->contextMenu()->show();
        }
        else
            m_tray->contextMenu()->hide();
    }
}

void MainWindow::setStatus(bool status)
{
    if(status)
    {
        m_status->setText(tr("Connected"));
        m_status->setEnabled(true);
    }
    else
    {
        m_status->setText(tr("Disconnected"));
        m_status->setEnabled(false);
    }
}

void MainWindow::updateSelectedSoundGui(QString filepath)
{
    Q_UNUSED(filepath)
    SoundItem *emitter = (SoundItem *)QObject::sender();
    for(int i=0; i<GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        if(m_soundTab[i] == emitter)
        {
            DBG("Default sound checked (%d)", i);
            m_soundTab[i]->check(true);
        }
        else
        {
            DBG("Sound %d unchecked", i)
            m_soundTab[i]->check(false);
        }
    }
}
