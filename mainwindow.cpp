#include "mainwindow.h"
#include <iostream>
#include "common.h"
#include <QApplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_player = new QMediaPlayer();
    m_player->setVolume(100);

    m_usb = new UsbManager();

    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QIcon("D:\\gg_button_app\\resources\\gear.xpm"));
    m_tray->setToolTip(tr("GG Button Manager"));
    connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                            this, SLOT(displayMenu(QSystemTrayIcon::ActivationReason)));
    m_menu = new QMenu(this);
    m_status = new QAction(tr("Disconnected"));
    connect(m_usb, SIGNAL(deviceStatusChanged(bool)), this, SLOT(setStatus(bool)));
    m_status->setEnabled(false);
    m_menu->addAction(m_status);

    buildSoundList();

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
    delete(m_player);
    for(int i=0; i<GG_BUTTON_MAX_SOUND_LIB; i++)
        delete(m_soundTab[i]);
    delete(m_status);
    delete(m_menu);
    delete(m_tray);

}

void MainWindow::buildSoundList(void)
{
    if(!m_menu)
        m_menu = new QMenu();
    m_soundTab = new SoundItem*[GG_BUTTON_MAX_SOUND_LIB];
    for(int i = 0; i < GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        m_soundTab[i] = new SoundItem();
        m_menu->insertMenu(NULL, m_soundTab[i]->getMenu());
        connect(m_soundTab[i], SIGNAL(defaultSoundChanged(QFileInfo)),
                this, SLOT(updateDefaultSound(QFileInfo)));
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

void MainWindow::updateDefaultSound(QFileInfo sound)
{
    m_defaultSound = sound;
    SoundItem *emitter = (SoundItem *)QObject::sender();
    for(int i=0; i< GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        if(m_soundTab[i] == emitter)
            m_soundTab[i]->check(true);
        else
            m_soundTab[i]->check(false);
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
