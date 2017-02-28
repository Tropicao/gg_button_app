#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#define SETTINGS_FILE   "config.json"

typedef struct
{
    QString *name;
    QString *path;
}SoundData;

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = 0);
    Settings(QObject *parent, QString filepath);
    SoundData* loadSettings(int *size);

signals:
    void settingsLoaded(SoundData *data, int size);

public slots:
private:
    QString m_configName;

};

#endif // SETTINGS_H
