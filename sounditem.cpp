#include "sounditem.h"
#include <QFileDialog>
#include "common.h"
#include <QMediaPlayer>
#include <QUrl>

int SoundItem::nbItem = 0;

//****************************************************************/
/**************** Constructors and destructors ******************/
/****************************************************************/

SoundItem::SoundItem(QWidget *parent) : QWidget(parent)
{
    nbItem++;
    m_num = nbItem;
    m_menu = new QMenu(QString("Sound %1").arg(m_num), this);
    m_playSound = new QAction(tr("Play sound"), m_menu);
    m_playSound->setEnabled(false);
    m_menu->addAction(m_playSound);
    connect(m_playSound, SIGNAL(triggered()), this, SLOT(playSoundFile()));
    m_selectSound = new QAction(tr("Set this sound as default"), m_menu);
    m_selectSound->setEnabled(false);
    m_menu->addAction(m_selectSound);
    connect(m_selectSound, SIGNAL(triggered()), this, SLOT(selectSoundFile()));
}

SoundItem::SoundItem(QWidget *parent, SoundData *data) : QWidget(parent)
{
    nbItem++;
    m_num = nbItem;
    m_menu = new QMenu(QString("Sound %1").arg(m_num), this);
    m_playSound = new QAction(tr("Play sound"), m_menu);
    m_menu->addAction(m_playSound);
    connect(m_playSound, SIGNAL(triggered()), this, SLOT(playSoundFile()));
    m_selectSound = new QAction(tr("Set this sound as default"), m_menu);
    m_menu->addAction(m_selectSound);
    connect(m_selectSound, SIGNAL(triggered()), this, SLOT(selectSoundFile()));
    if(data && !data->path->isEmpty())
    {
        DBG("Creating loaded sound item");
        m_file = new QFileInfo(*(data->path));
        m_menu->setTitle(m_file->fileName());
        m_playSound->setEnabled(true);
        m_selectSound->setEnabled(true);

    }
    else
    {
        DBG("Creating empty sound item");
        m_playSound->setEnabled(false);
        m_selectSound->setEnabled(false);
    }
}

SoundItem::~SoundItem()
{
    delete(m_playSound);
    delete(m_selectSound);
    delete(m_menu);
    nbItem--;
}

/****************************************************************/
/********************* Public slots *****************************/
/****************************************************************/

void SoundItem::selectSoundFile()
{
    DBG("New default sound file asked");
    SoundItem *sound = (SoundItem *)QObject::sender()->parent()->parent();
    emit defaultSoundChanged(sound->m_file->absoluteFilePath());
}

void SoundItem::playSoundFile()
{
    SoundItem *emitter = (SoundItem *)QObject::sender()->parent()->parent();
    if(emitter && m_file)
    {
        emit soundTriggered(emitter->m_file->absoluteFilePath());
    }
    else
    {
        DBG("Missing data, cannot play sound");
    }
}

/******************************************************************/
/********************* Public methods *****************************/
/******************************************************************/

QMenu* SoundItem::getMenu()
{
    return m_menu;
}

void SoundItem::check(bool check)
{
    if(check)
        m_menu->setIcon(QIcon(":/resources/resources/gear.png"));
    else
        m_menu->setIcon(QIcon());
}

QAction* SoundItem::getSelectionButton(void)
{
    return m_selectSound;
}

