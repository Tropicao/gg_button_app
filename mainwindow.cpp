#include "mainwindow.h"
#include <iostream>
#include "common.h"
#include <QApplication.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_player = new QMediaPlayer();
    m_player->setVolume(100);
    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QIcon("D:\\gg_button_app\\resources\\gear.xpm"));
    m_tray->setToolTip(tr("GG Button Manager"));
    connect(m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                            this, SLOT(displayMenu(QSystemTrayIcon::ActivationReason)));
    QMenu *menu = buildSoundList();

    menu->addSeparator();
    QAction *quitAction = new QAction(tr("Leave"), this);
    menu->addAction(quitAction);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    m_tray->setContextMenu(menu);

    // Start
    m_tray->show();
}

MainWindow::~MainWindow()
{
    delete(m_player);
    for(int i=0; i<GG_BUTTON_MAX_SOUND_LIB; i++)
        delete(m_soundTab[i]);
    delete(m_tray->contextMenu());
    delete(m_tray);

}

QMenu* MainWindow::buildSoundList(void)
{
    QMenu *menu = new QMenu(this);
    m_soundTab = new SoundItem*[GG_BUTTON_MAX_SOUND_LIB];
    for(int i = 0; i < GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        m_soundTab[i] = new SoundItem();
        menu->insertMenu(NULL, m_soundTab[i]->getMenu());
        connect(m_soundTab[i], SIGNAL(defaultSoundChanged(QFileInfo)),
                this, SLOT(updateDefaultSound(QFileInfo)));
    }
    return menu;
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
