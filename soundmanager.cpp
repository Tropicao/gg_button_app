#include "soundmanager.h"

SoundManager::SoundManager(QObject *parent) : QObject(parent)
{
    m_player = new QMediaPlayer();
    m_player->setVolume(100);
    m_defaultSound = new QFileInfo();
}

SoundManager::~SoundManager()
{
    delete(m_player);
}

void SoundManager::setMainSound(QFileInfo fileInfo)
{
    m_defaultSound->setFile(fileInfo.absoluteFilePath());
}

void SoundManager::playDefaultSound()
{
    m_player->stop();
    m_player->setMedia(QUrl::fromLocalFile(m_defaultSound->absoluteFilePath()));
    m_player->play();
}
