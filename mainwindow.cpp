#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QString>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>

#define WIN_HEIGHT 1200
#define WIN_WIDTH 1500

#define TL_COUNT 10
#define ITEMS_IN_ROW 2

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(WIN_WIDTH, WIN_HEIGHT));

    cu_vec = new QVector<ControlUnit*>();

    auto *layout = new QVBoxLayout();

    _menu = menuBar()->addMenu(tr("&Menu"));
    reset_act = new QAction(tr("&Reset"), this);
    connect(reset_act, &QAction::triggered, this, &MainWindow::reset);
    _menu->addAction(reset_act);

    int ROW_COUNT;
    if (fabs((float)TL_COUNT / ITEMS_IN_ROW - TL_COUNT / ITEMS_IN_ROW) > 0.00001f) {
        ROW_COUNT = TL_COUNT / ITEMS_IN_ROW + 1;
    } else {
        ROW_COUNT = TL_COUNT / ITEMS_IN_ROW;
    }

    int c = 0;
    for (int i = 0; i < ROW_COUNT; i++) {
        auto *sub_layout = new QHBoxLayout();
        for (int j = 0; j < ITEMS_IN_ROW; j++) {
            ControlUnit *cu = ControlUnit::create(c);
            cu->initTcpCon();
            cu_vec->append(cu);
            sub_layout->addWidget(cu->getFrame());
            c++;
            if (c == TL_COUNT) {
                break;
            }
        }
        layout->addLayout(sub_layout);
    }

    auto contentWidget = new QWidget();
    contentWidget->setLayout(layout);
    auto scrollArea = new QScrollArea();
    scrollArea->setWidget(contentWidget);
    setCentralWidget(scrollArea);

}

void MainWindow::reset() {
    qDebug() << "reset";
    for (int i = 0; i < cu_vec->size(); i++) {
        ControlUnit *cu = cu_vec->at(i);
        cu->reset();
    }
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < cu_vec->size(); i++) {
       cu_vec->at(i)->remove();
    }
    delete cu_vec;
    delete ui;
}
