#ifndef FILEPLAYER_H
#define FILEPLAYER_H

#include <QObject>
#include <QProcess>

class filePlayer : public QObject
{
    Q_OBJECT
    QProcess * player;
    QString currentFile;
    QString nextFile;
    bool paused;

public:
    explicit filePlayer(QObject *parent = 0);
    ~filePlayer();

signals:
    void playbackInfo(QString filename);
    void statusChanged(QString newStatus);

public slots:
    void loadFile(QString path);
    void play();
    void stop();
    void pause();
    void ff(int frames);
    void rw(int frames);

private slots:
    void player_update();
};

#endif // FILEPLAYER_H
