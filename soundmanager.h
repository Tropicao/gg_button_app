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

public slots:
    void setMainSound(QFileInfo fileInfo);
    void playDefaultSound();
private:
    QFileInfo *m_defaultSound;
    QMediaPlayer *m_player;
};

#endif // SOUNDMANAGER_H
