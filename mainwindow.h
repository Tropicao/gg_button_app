#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QMediaPlayer>
#include <sounditem.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QMenu* buildSoundList();
private:
    QSystemTrayIcon *m_tray;
    QMediaPlayer *m_player;
    SoundItem **m_soundTab;
    QFileInfo m_defaultSound;
public slots:
    void displayMenu(QSystemTrayIcon::ActivationReason reason);
    void updateDefaultSound(QFileInfo sound);
};

#endif // MAINWINDOW_H
