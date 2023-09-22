//
// Created by fangl on 2023/9/18.
//

#include "mainwindow.hpp"
#include <QObject>
#include <QTimer>
#include "ui/ui_MainWindow.h"

const int OGL_WIDTH = 800;
const int OGL_HEIGHT = 600;

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    glManager = new GLManager(this, OGL_WIDTH, OGL_HEIGHT);
    initWidget();
    initLayout();
    connectConfigure();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateGLManager);
    timer->start(10);

    // filters install
    this->installEventFilter(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::initWidget() {
    sceneGroupBox = ui->sceneGroupBox;
    lightTitleLabel = ui->lightTitleLabel;
    objectTitleLabel = ui->objectTitleLabel;
    lightAddButton = ui->lightAddButton;
    objectAddButton = ui->objectAddButton;
    lightList = ui->lightList;      lightList->setFocusPolicy(Qt::NoFocus);
    objectList = ui->objectList;    objectList->setFocusPolicy(Qt::NoFocus);

    inspectorGroupBox = ui->inspectorGroupBox;
    nameCheckBox = ui->nameCheckBox;
    nameLineEdit = ui->nameLineEdit;
    transformGroupBox = ui->transformGroupBox;

    configureDashTab = ui->configureDashTab;

    // 操作时增加：(需要new)
    posTitleLabel = new QLabel("pos:");
    rotationTitleLabel = new QLabel("rot:");
    scaleTitleLabel = new QLabel("scale:");
    directionTitleLabel = new QLabel("direct:");

    xPosLabel = new QLabel("X");    xPosSpinBox = new QDoubleSpinBox;
    yPosLabel = new QLabel("Y");    yPosSpinBox = new QDoubleSpinBox;
    zPosLabel = new QLabel("Z");    zPosSpinBox = new QDoubleSpinBox;

    // objects
    objXRotLabel = new QLabel("X"); objXRotSpinBox = new QDoubleSpinBox;
    objYRotLabel = new QLabel("Y"); objYRotSpinBox = new QDoubleSpinBox;
    objZRotLabel = new QLabel("Z"); objZRotSpinBox = new QDoubleSpinBox;

    objXScaleLabel = new QLabel("X");   objXScaleSpinBox = new QDoubleSpinBox;
    objYScaleLabel = new QLabel("Y");   objYScaleSpinBox = new QDoubleSpinBox;
    objZScaleLabel = new QLabel("Z");   objZScaleSpinBox = new QDoubleSpinBox;

    // lights
    lightTypeComboBox = new QComboBox;   // 按照comboBox的种类再次出现不同的控件
    lightTypeLabel = new QLabel("Light Type:");
    lightIntensityLabel = new QLabel("Light Intensity:");
    lightIntensitySpinBox = new QDoubleSpinBox;

    // direct light
    lightXDirLabel = new QLabel("X");   lightXDirSpinBox = new QDoubleSpinBox;
    lightYDirLabel = new QLabel("Y");   lightYDirSpinBox = new QDoubleSpinBox;
    lightZDirLabel = new QLabel("Z");   lightZDirSpinBox = new QDoubleSpinBox;

    mainMenuBar = new QMenuBar(this);
    fileMenu = new QMenu("File", this);
    loadModelAction = new QAction("Load Model", this);
}

void MainWindow::initLayout() {
    // Scene Layout
    auto *hLightLayout = new QHBoxLayout;
    hLightLayout->addWidget(lightTitleLabel);
    hLightLayout->addWidget(lightAddButton);

    auto *hObjLayout = new QHBoxLayout;
    hObjLayout->addWidget(objectTitleLabel);
    hObjLayout->addWidget(objectAddButton);

    auto *vSceneLayout = new QVBoxLayout;
    vSceneLayout->addLayout(hLightLayout);
    vSceneLayout->addWidget(lightList, 2);
    vSceneLayout->addLayout(hObjLayout);
    vSceneLayout->addWidget(objectList, 2);

    sceneGroupBox->setLayout(vSceneLayout);

    // name and display Layout
    auto *hDisplayLayout = new QHBoxLayout;
    hDisplayLayout->addWidget(nameCheckBox);
    hDisplayLayout->addWidget(nameLineEdit);

    // Transform Layout
    auto *posLayout = new QHBoxLayout;
    posLayout->addWidget(posTitleLabel);
    posLayout->addStretch(2);
    posLayout->addWidget(xPosLabel);    posLayout->addWidget(xPosSpinBox,4);
    posLayout->addWidget(yPosLabel);    posLayout->addWidget(yPosSpinBox,4);
    posLayout->addWidget(zPosLabel);    posLayout->addWidget(zPosSpinBox,4);

    // for objects:
    // rotation
    auto *rotLayout = new QHBoxLayout;
    rotLayout->addWidget(rotationTitleLabel);
    rotLayout->addStretch(2);
    rotLayout->addWidget(objXRotLabel); rotLayout->addWidget(objXRotSpinBox, 4);
    rotLayout->addWidget(objYRotLabel); rotLayout->addWidget(objYRotSpinBox, 4);
    rotLayout->addWidget(objZRotLabel); rotLayout->addWidget(objZRotSpinBox, 4);

    // scale
    auto *scaleLayout = new QHBoxLayout;
    scaleLayout->addWidget(scaleTitleLabel);
    scaleLayout->addStretch(2);
    scaleLayout->addWidget(objXScaleLabel); scaleLayout->addWidget(objXScaleSpinBox, 4);
    scaleLayout->addWidget(objYScaleLabel); scaleLayout->addWidget(objYScaleSpinBox, 4);
    scaleLayout->addWidget(objZScaleLabel); scaleLayout->addWidget(objZScaleSpinBox, 4);

    // for lights:
    // Light Type
    auto *lightTypeLayout = new QHBoxLayout;
    lightTypeLayout->addWidget(lightTypeLabel);
    lightTypeLayout->addWidget(lightTypeComboBox);

    // Direct Light Direction
    auto *lightDirectionLayout = new QHBoxLayout;
    lightDirectionLayout->addWidget(directionTitleLabel);
    lightDirectionLayout->addStretch(2);
    lightDirectionLayout->addWidget(lightXDirLabel);    lightDirectionLayout->addWidget(lightXDirSpinBox, 4);
    lightDirectionLayout->addWidget(lightYDirLabel);    lightDirectionLayout->addWidget(lightYDirSpinBox, 4);
    lightDirectionLayout->addWidget(lightZDirLabel);    lightDirectionLayout->addWidget(lightZDirSpinBox, 4);

    // Light Intensity
    auto *lightIntensityLayout = new QHBoxLayout;
    lightIntensityLayout->addWidget(lightIntensityLabel);
    lightIntensityLayout->addWidget(lightIntensitySpinBox, 2);

    auto *transformLayout = new QVBoxLayout;
    transformLayout->addLayout(lightTypeLayout);
    transformLayout->addLayout(posLayout);
    transformLayout->addLayout(rotLayout);
    transformLayout->addLayout(scaleLayout);
    transformLayout->addLayout(lightDirectionLayout);
    transformLayout->addLayout(lightIntensityLayout);
    transformLayout->addStretch(1);

    // Inspector Layout
    auto *vInspectorLayout = new QVBoxLayout;
    vInspectorLayout->addLayout(hDisplayLayout);
    vInspectorLayout->addWidget(transformGroupBox, 1);

    inspectorGroupBox->setLayout(vInspectorLayout);

    // Mid Layout
    auto *midLayout = new QVBoxLayout;
    midLayout->addWidget(glManager, 2);
    midLayout->addWidget(configureDashTab, 1);

    // Menu Bar
    mainMenuBar->addMenu(fileMenu);
    fileMenu->addAction(loadModelAction);

    // Content Layout
    auto *contentLayout = new QHBoxLayout;
    contentLayout->addWidget(sceneGroupBox, 1);
    contentLayout->addLayout(midLayout, 3);
    contentLayout->addWidget(inspectorGroupBox, 1);

    // Total Layout
    auto *totalLayout = new QVBoxLayout;
    totalLayout->addWidget(mainMenuBar);
    totalLayout->addLayout(contentLayout);
    totalLayout->setContentsMargins(0,0,0,0);

    this->setLayout(totalLayout);
}

void MainWindow::connectConfigure() {

    // QLineEdit
    connect(nameLineEdit, &QLineEdit::editingFinished, this, &MainWindow::handleEditingFinished);


}

void MainWindow::updateGLManager() {
    glManager->update();
}

// slot functions
void MainWindow::handleEditingFinished() {
    QString currentText = nameLineEdit->text();

    if (!nameLineEdit->hasFocus()) {
        qDebug() << "LineEdit has lost focus with text:" << currentText;
    }

    // other codes...


    nameLineEdit->clearFocus();
}

// filter functions
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::MouseButtonPress) {
        nameLineEdit->clearFocus();
    }

    return QWidget::eventFilter(watched, event);
}

