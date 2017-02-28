#include "settings.h"
#include <QFile>
#include "common.h"
#include <QDir>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

static const char* _default_sound_names[GG_BUTTON_MAX_SOUND_LIB] = {
    "Gameboy",
    "Fatality",
    "Leroy",
    "Jack",
    "Moman"
};

Settings::Settings(QObject *parent) : QObject(parent), m_configName(SETTINGS_FILE)
{
}

Settings::Settings(QObject *parent, QString filepath) : QObject(parent), m_configName(filepath)
{
}

/*static SoundData* _parseDocument(const QJsonObject &document)
{
    Q_UNUSED(document);
    SoundData *data = new SoundData[GG_BUTTON_MAX_SOUND_LIB];
    for (int i = 0; i < GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        QString rootName("sound");
        rootName.append(QString("%1").arg(i+1));
        QJsonObject sound = document[rootName].toObject();
        if(sound.isEmpty())
            continue;
        data[i].name = new QString(QString::fromUtf8((sound["name"].toString()));
        data[i].path = new QString(sound["path"].toString());
    }
    return data;
    return NULL;
}*/

SoundData* Settings::loadSettings(int *size)
{
    SoundData *data = NULL;

    if(!size)
    {
        DBG("Size pointer is NULL, will not load sound file");
        return data;
    }
    *size = 0;

    for (int i=0; i< GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        QString sound_file_path = QDir::currentPath();
        sound_file_path.append("/");
        sound_file_path.append(QString("sound%1.mp3").arg(i+1));
        QFile sound_file(sound_file_path);
        if(sound_file.exists())
        {
            DBG("Found sound file");
            if(!data)
                data = new SoundData[GG_BUTTON_MAX_SOUND_LIB];
            data[i].name = new QString(_default_sound_names[i]);
            data[i].path = new QString(sound_file_path);
            (*size)++;
        }
        else
        {
            DBG("Sound file does not exists");
        }
    }
    return data;

    /*QString config_file_path = QDir::currentPath();
    config_file_path.append("/");
    config_file_path.append(m_configName);
    QFile config_file(config_file_path);
    if(!(config_file.exists()))
    {
        qWarning("Configuration file not found :");
        std::cout<<config_file_path.toStdString()<<std::endl;
        return data;
    }

    if(!config_file.open(QIODevice::ReadOnly))
    {
        qWarning("Cannot open configuration file");
        return data;
    }
    QByteArray config_data = config_file.readAll();
    QJsonDocument document (QJsonDocument::fromJson(config_data));

    data = _parseDocument(document.object());
    if(!data)
    {
        DBG("Did to parse json document");
        return data;
    }
    for(int i=0; i<GG_BUTTON_MAX_SOUND_LIB; i++)
    {
        DBG("Sound %d:", i+1);
        if(!data[i].name->isNull())
            std::cout<<"Name : "<<data[i].name->toStdString()<<std::endl;
        if(!data[i].path->isNull())
            std::cout<<"Path : "<<data[i].path->toStdString()<<std::endl;
    }
    return data;*/

}
