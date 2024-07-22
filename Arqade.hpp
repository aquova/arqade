#ifndef ARQADE_HPP
#define ARQADE_HPP

#include <Qt>
#include <QObject>
#include <QString>

#include "Database.hpp"

class Arqade : public QObject {
    public slots:
        void RunEmu(QString aRomPath, QString aCorePath, ControllerMapType aBindings);
};

#endif
