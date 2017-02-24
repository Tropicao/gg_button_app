#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMediaPlayer>
#include <sounditem.h>
#include <UsbManager.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void buildSoundList();
private:
    QSystemTrayIcon *m_tray;
    QMenu *m_menu;
    QAction *m_status;
    QMediaPlayer *m_player;
    SoundItem **m_soundTab;
    QFileInfo m_defaultSound;
    UsbManager *m_usb;
public slots:
    void displayMenu(QSystemTrayIcon::ActivationReason reason);
    void updateDefaultSound(QFileInfo sound);
    void setStatus(bool status);
};

#endif // MAINWINDOW_H
