#include "gttcpsocket.h"

GtTcpSocket::GtTcpSocket(QObject *parent) : QObject(parent)
{

}

void GtTcpSocket::setTcpSocket(QString url, int port, GtTcpSocketInterface *intf) {
    this->conn_url = url;
    this->conn_port = port;
    this->intf = intf;

    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
}

void GtTcpSocket::write(char* msg) {
    if (socket->isWritable()) {
        socket->write(msg, 6);
        qDebug() << "socket writable";
    } else {
        qDebug() << "socket not writable";
    }
}

void GtTcpSocket::doConnect()
{
    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost(conn_url, conn_port);

    if(!socket->waitForConnected(100))
    {
        qDebug() << "Error: " << socket->errorString();
        if (intf != NULL) {
            intf->tcpCallback(1);
        }
    }
}

void GtTcpSocket::connected()
{
    qDebug() << "connected...";
    // Hey server, tell me about you.
    //socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
    if (intf != NULL) {
        intf->tcpCallback(0);
    }
}

void GtTcpSocket::disconnected()
{
    //qDebug() << "disconnected...";
    if (intf != NULL) {
        intf->tcpCallback(2);
    }
}

void GtTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";

    if (intf != NULL) {
        intf->tcpCallback(7);
    }
}

void GtTcpSocket::readyRead()
{
    qDebug() << "reading...";

    // read the data from the socket
    qDebug() << socket->readAll();
}
