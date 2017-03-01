#include "soundmanager.h"
#include "common.h"

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

void SoundManager::setMainSound(QString fileinfo)
{
    m_defaultSound->setFile(fileinfo);
}

void SoundManager::playDefaultSound()
{
    DBG("Module asked to play default sound");
    if(m_defaultSound->isFile())
    {
        m_player->stop();
        m_player->setMedia(QUrl::fromLocalFile(m_defaultSound->absoluteFilePath()));
        m_player->play();
    }
    else
    {
        DBG("Default sound not found, will not play sound");
    }
}

void SoundManager::playSound(QString filepath)
{
    DBG("Module asked to play specific sound :");
    std::cout<<filepath.toStdString()<<std::endl;

    if(!filepath.isEmpty())
    {
        m_player->stop();
        m_player->setMedia(QUrl::fromLocalFile(filepath));
        m_player->play();
    }
    else
    {
        DBG("Asked sound path is empty, will not play sound");
    }
}
