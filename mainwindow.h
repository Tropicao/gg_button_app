#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMediaPlayer>
#include <sounditem.h>
#include <UsbManager.h>
#include <SoundManager.h>

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
    SoundItem **m_soundTab;
    UsbManager *m_usb;
    SoundManager *m_sound;
public slots:
    void displayMenu(QSystemTrayIcon::ActivationReason reason);
    void setStatus(bool status);
};

#endif // MAINWINDOW_H
