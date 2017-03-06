#include "controlunit.h"

#include <QDebug>
#include <QString>
#include <QThread>

#define FRA_HEIGHT 120
#define FRA_WIDTH 700

const QString ControlUnit::BASE_URL = "192.168.161.5";

ControlUnit::ControlUnit(QObject *parent) : QObject(parent)
{
    step = 0;
    flag[0] = 's';
    flag[1] = '0';
    flag[2] = '0';
    flag[3] = '0';
    flag[4] = '0';
    flag[5] = 'e';
}

ControlUnit* ControlUnit::create(int num) {
    ControlUnit *cu = new ControlUnit();
    cu->cu_num = num;
    cu->setFrameGeometry();
    cu->initControlUnit();
    return cu;
}

void ControlUnit::setFrameGeometry() {
    qfr = new QFrame();
    qfr->setFixedSize(QSize(FRA_WIDTH, FRA_HEIGHT));
}

void ControlUnit::initControlUnit() {

    // TL NUM LABEL
    QString qlab_str = "TL";
    QString num_str;
    if (cu_num < 10) {
        num_str = "0" + QString::number(cu_num);
    } else {
        num_str = QString::number(cu_num);
    }
    qlab_str += num_str;
    qlab_num.setGeometry(10, 5, 60, 30);
    qlab_num.setText(qlab_str);
    qlab_num.setParent(qfr);

    // TL POS LABEL
    qlab_tl_stop.setGeometry(100, 30, 80, 30);
    qlab_tl_stop.setText("정지");
    qlab_tl_stop.setParent(qfr);

    qlab_tl_warn.setGeometry(200, 30, 80, 30);
    qlab_tl_warn.setText("경고");
    qlab_tl_warn.setParent(qfr);

    qlab_tl_left.setGeometry(300, 30, 80, 30);
    qlab_tl_left.setText("좌회전");
    qlab_tl_left.setParent(qfr);

    qlab_tl_strght.setGeometry(400, 30, 80, 30);
    qlab_tl_strght.setText("직진");
    qlab_tl_strght.setParent(qfr);

    // TL SET CHXBOX
    qchb_stop.setGeometry(115, 70, 30, 30);
    qchb_stop.setParent(qfr);
    qchb_stop.setCheckState(Qt::Unchecked);
    connect(&qchb_stop, SIGNAL(clicked()), this, SLOT(qchb_stop_changed()));

    qchb_warn.setGeometry(215, 70, 30, 30);
    qchb_warn.setParent(qfr);
    qchb_warn.setCheckState(Qt::Unchecked);
    connect(&qchb_warn, SIGNAL(clicked()), this, SLOT(qchb_warn_changed()));

    qchb_left.setGeometry(315, 70, 30, 30);
    qchb_left.setParent(qfr);
    qchb_left.setCheckState(Qt::Unchecked);
    connect(&qchb_left, SIGNAL(clicked()), this, SLOT(qchb_left_changed()));

    qchb_strght.setGeometry(415, 70, 30, 30);
    qchb_strght.setParent(qfr);
    qchb_strght.setCheckState(Qt::Unchecked);
    connect(&qchb_strght, SIGNAL(clicked()), this, SLOT(qchb_strght_changed()));

    btn_reset.setGeometry(480, 50, 80, 40);
    btn_reset.setText("OFF");
    btn_reset.setParent(qfr);
    connect(&btn_reset, SIGNAL(clicked()), this, SLOT(btn_reset_clicked()));

    btn_setall.setGeometry(580, 50, 80, 40);
    btn_setall.setText("ON");
    btn_setall.setParent(qfr);
    connect(&btn_setall, SIGNAL(clicked()), this, SLOT(btn_setall_clicked()));

    disableCU();

    step = 1;
}



void ControlUnit::initTcpCon() {
    tcp_sock = new GtTcpSocket();
    tcp_sock->setTcpSocket(ControlUnit::BASE_URL, ControlUnit::BASE_PORT + cu_num, this);
    tcp_sock->doConnect();
}

void ControlUnit::reset() {
    step = 0;
    flag[0] = 's';
    flag[1] = '0';
    flag[2] = '0';
    flag[3] = '0';
    flag[4] = '0';
    flag[5] = 'e';
    tcp_sock->release();
    qchb_stop.setCheckState(Qt::Unchecked);
    qchb_warn.setCheckState(Qt::Unchecked);
    qchb_left.setCheckState(Qt::Unchecked);
    qchb_strght.setCheckState(Qt::Unchecked);
    initTcpCon();
}

void ControlUnit::tcpCallback(int state) {
    qDebug() << "TCP CONN STATE: " << state;
    switch (state) {
    case 0:
        step = 2;
        enableCU();
        break;
    case 1:
        disableCU();
        break;
    case 2:
        disableCU();
        break;
    case 7:
        break;
    default:
        disableCU();
        break;
    }
}

void ControlUnit::qchb_stop_changed() {
    if (qchb_stop.checkState() == Qt::Checked) {
        flag[1] = '1';

        flag[2] = '0';
        qchb_warn.setCheckState(Qt::Unchecked);
        flag[3] = '0';
        qchb_left.setCheckState(Qt::Unchecked);
        flag[4] = '0';
        qchb_strght.setCheckState(Qt::Unchecked);
    } else if (qchb_stop.checkState() == Qt::Unchecked) {
        flag[1] = '0';
    }
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_stop_changed(), write";
    }
}

void ControlUnit::qchb_warn_changed() {
    if (qchb_warn.checkState() == Qt::Checked) {
        flag[2] = '1';
        flag[1] = '0';
        qchb_stop.setCheckState(Qt::Unchecked);
        flag[3] = '0';
        qchb_left.setCheckState(Qt::Unchecked);
        flag[4] = '0';
        qchb_strght.setCheckState(Qt::Unchecked);
    } else if (qchb_warn.checkState() == Qt::Unchecked) {
        flag[2] = '0';
    }
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_warn_changed(), write";
    }
}

void ControlUnit::qchb_left_changed() {
    if (qchb_left.checkState() == Qt::Checked) {
        flag[3] = '1';
        flag[1] = '0';
        qchb_stop.setCheckState(Qt::Unchecked);
        flag[2] = '0';
        qchb_warn.setCheckState(Qt::Unchecked);
    } else if (qchb_left.checkState() == Qt::Unchecked) {
        flag[3] = '0';
    }
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_left_changed(), write";
    }
}

void ControlUnit::qchb_strght_changed() {
    if (qchb_strght.checkState() == Qt::Checked) {
        flag[4] = '1';
        flag[1] = '0';
        qchb_stop.setCheckState(Qt::Unchecked);
        flag[2] = '0';
        qchb_warn.setCheckState(Qt::Unchecked);
    } else if (qchb_strght.checkState() == Qt::Unchecked) {
        flag[4] = '0';
    }
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_strght_changed(), write";
    }
}

void ControlUnit::btn_reset_clicked() {
    flag[0] = 's';
    flag[1] = '0';
    flag[2] = '0';
    flag[3] = '0';
    flag[4] = '0';
    flag[5] = 'e';
    qchb_stop.setCheckState(Qt::Unchecked);
    qchb_warn.setCheckState(Qt::Unchecked);
    qchb_left.setCheckState(Qt::Unchecked);
    qchb_strght.setCheckState(Qt::Unchecked);
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_strght_changed(), write";
    }
}

void ControlUnit::btn_setall_clicked() {
    flag[0] = 's';
    flag[1] = '1';
    flag[2] = '1';
    flag[3] = '1';
    flag[4] = '1';
    flag[5] = 'e';
    qchb_stop.setCheckState(Qt::Checked);
    qchb_warn.setCheckState(Qt::Checked);
    qchb_left.setCheckState(Qt::Checked);
    qchb_strght.setCheckState(Qt::Checked);
    if (step == 2) {
        write();
        qDebug() << "ControlUnit::qchb_strght_changed(), write";
    }
}

void ControlUnit::write() {
    tcp_sock->write(flag);
}


void ControlUnit::disableCU() {
    qchb_left.setEnabled(false);
    qchb_strght.setEnabled(false);
    qchb_warn.setEnabled(false);
    qchb_stop.setEnabled(false);
    btn_reset.setEnabled(false);
    btn_setall.setEnabled(false);
}

void ControlUnit::enableCU() {
    qchb_left.setEnabled(true);
    qchb_strght.setEnabled(true);
    qchb_warn.setEnabled(true);
    qchb_stop.setEnabled(true);
    btn_reset.setEnabled(true);
    btn_setall.setEnabled(true);
}


