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
    //SoundItem *sound = (SoundItem *)QObject::sender()->parent()->parent();
    DBG("Loading file explorer");
    QFileInfo filename;
    QFileDialog *fd = new QFileDialog();

    fd->setOption(QFileDialog::DontUseNativeDialog, true);
    fd->setOption(QFileDialog::DontUseCustomDirectoryIcons, true);
    fd->exec();
    //filename = QFileDialog::getOpenFileName(this, tr("Open new sound file"));

    /*if(true)
    {
        DBG("Yup, definitely a sound file");
        sound->m_menu->setTitle("gameboy.mp3");
        DBG("Sound name set");
        sound->m_playSound->setEnabled(true);
        sound->m_selectSound->setEnabled(true);
        sound->m_removeSound->setEnabled(true);
        m_file = new QFileInfo(filename);
        DBG("New sound loaded !");
    }
    else
        DBG("No file selected");*/
    delete(fd);
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
    emit soundTriggered(*(emitter->m_file));
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

