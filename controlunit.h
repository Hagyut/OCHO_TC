#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H

#include <QObject>

#include <QFrame>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

#include "gttcpsocket.h"
#include "gttcpsocketinterface.h"

class ControlUnit : public QObject, public GtTcpSocketInterface
{
    Q_OBJECT
public:
    explicit ControlUnit(QObject *parent = 0);
    static ControlUnit* create(int num);

    void initTcpCon();
    void reset();
    virtual void tcpCallback(int state);
    void write();

    QFrame* getFrame() {
        return qfr;
    }

    void remove() {
        delete qfr;
        tcp_sock->release();
        this->~ControlUnit();
    }

public:
    static const QString BASE_URL;
    static const int BASE_PORT = 7000;

public slots:
    void qchb_left_changed();
    void qchb_strght_changed();
    void qchb_warn_changed();
    void qchb_stop_changed();
    void btn_reset_clicked();
    void btn_setall_clicked();

private:
    void setFrameGeometry();
    void initControlUnit();
    void disableCU();
    void enableCU();

    ~ControlUnit() {

    }

private:
    int cu_num;
    int step;
    // step 0: CU 생성
    // step 1: GUI 부착 완료
    // step 2: TCP 연결 완료

    GtTcpSocket *tcp_sock;

    QFrame *qfr;
    QLabel qlab_num;

    QLabel qlab_tl_left;
    QLabel qlab_tl_strght;
    QLabel qlab_tl_warn;
    QLabel qlab_tl_stop;

    QCheckBox qchb_left;
    QCheckBox qchb_strght;
    QCheckBox qchb_warn;
    QCheckBox qchb_stop;

    QPushButton btn_reset;
    QPushButton btn_setall;

    char flag[6];
};

#endif // CONTROLUNIT_H
