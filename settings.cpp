#include "settings.h"
#include <QFile>
#include "common.h"
#include <QDir>
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

Settings::Settings(QObject *parent) : QObject(parent)
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QDir::currentPath());
    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Tropicao", "GG Button App");

}

Settings::Settings(QObject *parent, QString filepath) : QObject(parent)
{
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, filepath);
    m_settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Tropicao", "GG Button App");
}

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
        QString current_sound("sound");
        current_sound.append(QString::number(i));
        m_settings->beginGroup(current_sound);
        if (m_settings->contains("name") && m_settings->contains("path"))
        {
            *size = i+1;
            if(!data)
                data = new SoundData[GG_BUTTON_MAX_SOUND_LIB];
            data[i].name = new QString(m_settings->value("name").toString());
            data[i].path = new QString(m_settings->value("path").toString());
        }
        m_settings->endGroup();
    }
    return data;
}
