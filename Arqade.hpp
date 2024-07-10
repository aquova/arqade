#ifndef ARQADE_HPP
#define ARQADE_HPP

#include <QObject>
#include <QString>

class Arqade : public QObject {
    public slots:
        void RunEmu(QString aRomPath, QString aCorePath);
};

#endif
