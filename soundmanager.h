#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QMediaPlayer>

class SoundManager : public QObject
{
    Q_OBJECT
public:
    explicit SoundManager(QObject *parent = 0);
    virtual ~SoundManager();
signals:
    void soundTriggered(QString filepath);

public slots:
    void setMainSound(QString fileInfo);
    void playDefaultSound();
    void playSound(QString filepath);
private:
    QFileInfo *m_defaultSound;
    QMediaPlayer *m_player;
};

#endif // SOUNDMANAGER_H
