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
    m_loadSound = new QAction(tr("Select new sound..."), m_menu);
    connect(m_loadSound, SIGNAL(triggered()), this, SLOT(loadSoundFile()));
    m_menu->addAction(m_loadSound);
    m_playSound = new QAction(tr("Play sound"), m_menu);
    m_playSound->setEnabled(false);
    m_menu->addAction(m_playSound);
    connect(m_playSound, SIGNAL(triggered()), this, SLOT(playSoundFile()));
    m_selectSound = new QAction(tr("Set this sound as default"), m_menu);
    m_selectSound->setEnabled(false);
    m_menu->addAction(m_selectSound);
    connect(m_selectSound, SIGNAL(triggered()), this, SLOT(selectSoundFile()));
    m_removeSound = new QAction(tr("Remove sound"), m_menu);
    m_removeSound->setEnabled(false);
    m_menu->addAction(m_removeSound);
    connect(m_removeSound, SIGNAL(triggered()), this, SLOT(removeSoundFile()));
}

SoundItem::SoundItem(QWidget *parent, SoundData *data) : QWidget(parent)
{
    nbItem++;
    m_num = nbItem;
    m_menu = new QMenu(QString("Sound %1").arg(m_num), this);
    m_loadSound = new QAction(tr("Select new sound..."), m_menu);
    connect(m_loadSound, SIGNAL(triggered()), this, SLOT(loadSoundFile()));
    m_menu->addAction(m_loadSound);
    m_playSound = new QAction(tr("Play sound"), m_menu);
    m_menu->addAction(m_playSound);
    connect(m_playSound, SIGNAL(triggered()), this, SLOT(playSoundFile()));
    m_selectSound = new QAction(tr("Set this sound as default"), m_menu);
    m_menu->addAction(m_selectSound);
    connect(m_selectSound, SIGNAL(triggered()), this, SLOT(selectSoundFile()));
    m_removeSound = new QAction(tr("Remove sound"), m_menu);
    m_menu->addAction(m_removeSound);
    connect(m_removeSound, SIGNAL(triggered()), this, SLOT(removeSoundFile()));
    if(data && !data->path->isNull())
    {
        if(!data->name->isNull())
        {
            m_loadSound->setText(*(data->name));
        }
        m_file = new QFileInfo(*(data->path));
        m_playSound->setEnabled(true);
        m_selectSound->setEnabled(true);
        m_removeSound->setEnabled(true);

    }
    else
    {
        m_playSound->setEnabled(false);
        m_selectSound->setEnabled(false);
        m_removeSound->setEnabled(false);
    }
}

SoundItem::~SoundItem()
{
    delete(m_loadSound);
    delete(m_playSound);
    delete(m_selectSound);
    delete(m_removeSound);
    delete(m_menu);
    nbItem--;
}

/****************************************************************/
/********************* Public slots *****************************/
/****************************************************************/


void SoundItem::loadSoundFile()
{

}

void SoundItem::selectSoundFile()
{
    SoundItem *sound = (SoundItem *)QObject::sender()->parent()->parent();
    emit defaultSoundChanged(sound->m_file->absoluteFilePath());
}

void SoundItem::removeSoundFile()
{
    SoundItem *sound = (SoundItem *)QObject::sender()->parent()->parent();
    sound->m_menu->setTitle(QString("Sound %1").arg(sound->m_num));
    sound->m_playSound->setEnabled(false);
    sound->m_selectSound->setEnabled(false);
    sound->m_removeSound->setEnabled(false);
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
        m_menu->setIcon(QIcon("D:\\gg_button_app\\resources\\gear.xpm"));
    else
        m_menu->setIcon(QIcon());
}

QAction* SoundItem::getSelectionButton(void)
{
    return m_selectSound;
}

