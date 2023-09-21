//
// Created by fangl on 2023/9/18.
//

#include "mainwindow.hpp"
#include "ui/ui_MainWindow.h"


const int OGL_WIDTH = 800;
const int OGL_HEIGHT = 600;

MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    glManager = new GLManager(this, OGL_WIDTH, OGL_HEIGHT);

    initWidget();
    initLayout();
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
    lightList = ui->lightList;
    objectList = ui->objectList;

    inspectorGroupBox = ui->inspectorGroupBox;
    nameCheckBox = ui->nameCheckBox;
    nameLineEdit = ui->nameLineEdit;
    transformGroupBox = ui->transformGroupBox;

    configureDashTab = ui->configureDashTab;

    // 操作时增加：(需要new)
    posTitleLabel = new QLabel;         posTitleLabel->setText("pos:");
    rotationTitleLabel = new QLabel;    rotationTitleLabel->setText("rot:");
    scaleTitleLabel = new QLabel;       scaleTitleLabel->setText("scale:");
    directionTitleLabel = new QLabel;   directionTitleLabel->setText("direct:");

    xPosLabel = new QLabel; xPosLabel->setText("X");    xPosSpinBox = new QDoubleSpinBox;
    yPosLabel = new QLabel; yPosLabel->setText("Y");    yPosSpinBox = new QDoubleSpinBox;
    zPosLabel = new QLabel; zPosLabel->setText("Z");    zPosSpinBox = new QDoubleSpinBox;

    // objects
    objXRotLabel = new QLabel;  objXRotLabel->setText("X"); objXRotSpinBox = new QDoubleSpinBox;
    objYRotLabel = new QLabel;  objYRotLabel->setText("Y"); objYRotSpinBox = new QDoubleSpinBox;
    objZRotLabel = new QLabel;  objZRotLabel->setText("Z"); objZRotSpinBox = new QDoubleSpinBox;

    objXScaleLabel = new QLabel;    objXScaleLabel->setText("X");   objXScaleSpinBox = new QDoubleSpinBox;
    objYScaleLabel = new QLabel;    objYScaleLabel->setText("Y");   objYScaleSpinBox = new QDoubleSpinBox;
    objZScaleLabel = new QLabel;    objZScaleLabel->setText("Z");   objZScaleSpinBox = new QDoubleSpinBox;

    // lights
    lightTypeComboBox = new QComboBox;   // 按照comboBox的种类再次出现不同的控件
    lightTypeLabel = new QLabel;        lightTypeLabel->setText("Light Type:");
    lightIntensityLabel = new QLabel;   lightIntensityLabel->setText("Light Intensity:");
    lightIntensitySpinBox = new QDoubleSpinBox;

    // direct light
    lightXDirLabel = new QLabel;    lightXDirLabel->setText("X");   lightXDirSpinBox = new QDoubleSpinBox;
    lightYDirLabel = new QLabel;    lightYDirLabel->setText("Y");   lightYDirSpinBox = new QDoubleSpinBox;
    lightZDirLabel = new QLabel;    lightZDirLabel->setText("Z");   lightZDirSpinBox = new QDoubleSpinBox;

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