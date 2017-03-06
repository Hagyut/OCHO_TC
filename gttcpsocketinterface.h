#ifndef GTTCPSOCKETINTERFACE_H
#define GTTCPSOCKETINTERFACE_H

#include <QObject>

class GtTcpSocketInterface {
public:
    virtual void tcpCallback(int state) = 0;
};

#endif // GTTCPSOCKETINTERFACE_H
