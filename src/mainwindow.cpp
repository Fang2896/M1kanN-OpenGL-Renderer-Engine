//
// Created by fangl on 2023/9/18.
//

#include "mainwindow.hpp"
#include "ui/ui_MainWindow.h"


MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}
