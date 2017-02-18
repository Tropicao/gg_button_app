#ifndef SOUNDITEM_H
#define SOUNDITEM_H

#include <QWidget>
#include <QMenu>
#include <QFileInfo>

class SoundItem : public QWidget
{
    Q_OBJECT
public:
    static int nbItem;
    explicit SoundItem(QWidget *parent = 0);
    virtual ~SoundItem();
    QMenu *getMenu();
    void enableSoundFile();
    QAction* getSelectionButton(void);
    void check(bool check);

signals:
    void defaultSoundChanged(QFileInfo name);
    void soundTriggered(QFileInfo soundfile);

public slots:
    void loadSoundFile();
    void selectSoundFile();
    void removeSoundFile();
    void playSoundFile();
private:
    QMenu *m_menu;
    QAction *m_loadSound;
    QAction *m_playSound;
    QAction *m_selectSound;
    QAction *m_removeSound;
    QFileInfo *m_file;
    int m_num;
};

#endif // SOUNDITEM_H
