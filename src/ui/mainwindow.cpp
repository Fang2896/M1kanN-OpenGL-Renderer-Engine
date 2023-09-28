//
// Created by fangl on 2023/9/18.
//

#include "ui/mainwindow.hpp"
#include <QFileDialog>
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

    objectAddButton = ui->objectAddButton;
    lightAddButton = ui->lightAddButton;

    lightList = ui->lightList;      lightList->setFocusPolicy(Qt::NoFocus);
    objectList = ui->objectList;    objectList->setFocusPolicy(Qt::NoFocus);

    // Tool Button and Menus and Actions
    objectAddMenu = new QMenu(objectAddButton);
    objectShapeSelectMenu = new QMenu("shape select menu" ,objectAddMenu);
    objectLoadShapeCubeAction = objectShapeSelectMenu->addAction("cube");
    objectLoadShapeQuadAction = objectShapeSelectMenu->addAction("quad");
    objectLoadModelAction = objectAddMenu->addAction("model load");
    objectAddMenu->addMenu(objectShapeSelectMenu);
    objectAddButton->setMenu(objectAddMenu);
    objectAddButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    objectAddButton->setPopupMode(QToolButton::InstantPopup);

    lightAddMenu = new QMenu(lightAddButton);
    pointLightAddAction = lightAddMenu->addAction("add point light");
    spotLightAddAction = lightAddMenu->addAction("add spot light");
    lightAddButton->setMenu(lightAddMenu);
    lightAddButton->setToolButtonStyle(Qt::ToolButtonTextOnly);
    lightAddButton->setPopupMode(QToolButton::InstantPopup);

    inspectorGroupBox = ui->inspectorGroupBox;
    nameCheckBox = ui->nameCheckBox;
    nameLineEdit = ui->nameLineEdit;
    transformGroupBox = ui->transformGroupBox;

    configureDashTab = ui->configureDashTab;

    // 操作时隐藏或者显示：
    positionFrame = ui->positionFrame;
    rotationFrame = ui->rotationFrame;
    scaleFrame = ui->scaleFrame;
    directionFrame = ui->directionFrame;

    posTitleLabel = ui->posTitleLabel;
    rotationTitleLabel = ui->rotationTitleLabel;
    scaleTitleLabel = ui->scaleTitleLabel;
    directionTitleLabel = ui->directionTitleLabel;

    xPosLabel = ui->xPosLabel;    xPosSpinBox = ui->xPosSpinBox;
    yPosLabel = ui->yPosLabel;    yPosSpinBox = ui->yPosSpinBox;
    zPosLabel = ui->zPosLabel;    zPosSpinBox = ui->zPosSpinBox;

    // objects
    objXRotLabel = ui->objXRotLabel; objXRotSpinBox = ui->objXRotSpinBox;
    objYRotLabel = ui->objYRotLabel; objYRotSpinBox = ui->objYRotSpinBox;
    objZRotLabel = ui->objZRotLabel; objZRotSpinBox = ui->objZRotSpinBox;

    objXScaleLabel = ui->objXScaleLabel;   objXScaleSpinBox = ui->objXScaleSpinBox;
    objYScaleLabel = ui->objYScaleLabel;   objYScaleSpinBox = ui->objYScaleSpinBox;
    objZScaleLabel = ui->objZScaleLabel;   objZScaleSpinBox = ui->objZScaleSpinBox;

    // lights

    // direct light
    lightXDirLabel = ui->lightXDirLabel;   lightXDirSpinBox = ui->lightXDirSpinBox;
    lightYDirLabel = ui->lightYDirLabel;   lightYDirSpinBox = ui->lightYDirSpinBox;
    lightZDirLabel = ui->lightZDirLabel;   lightZDirSpinBox = ui->lightZDirSpinBox;

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

    auto *posLayout = new QHBoxLayout;
    posLayout->addWidget(xPosLabel, 1);    posLayout->addWidget(xPosSpinBox,4);
    posLayout->addWidget(yPosLabel, 1);    posLayout->addWidget(yPosSpinBox,4);
    posLayout->addWidget(zPosLabel, 1);    posLayout->addWidget(zPosSpinBox,4);

    // for objects:
    // rotation
    auto *rotLayout = new QHBoxLayout;
    rotLayout->addWidget(objXRotLabel, 1); rotLayout->addWidget(objXRotSpinBox, 4);
    rotLayout->addWidget(objYRotLabel, 1); rotLayout->addWidget(objYRotSpinBox, 4);
    rotLayout->addWidget(objZRotLabel, 1); rotLayout->addWidget(objZRotSpinBox, 4);

    // scale
    auto *scaleLayout = new QHBoxLayout;
    scaleLayout->addWidget(objXScaleLabel, 1); scaleLayout->addWidget(objXScaleSpinBox, 4);
    scaleLayout->addWidget(objYScaleLabel, 1); scaleLayout->addWidget(objYScaleSpinBox, 4);
    scaleLayout->addWidget(objZScaleLabel, 1); scaleLayout->addWidget(objZScaleSpinBox, 4);

    // direct
    auto *directionLayout = new QHBoxLayout;
    directionLayout->addWidget(lightXDirLabel, 1); directionLayout->addWidget(lightXDirSpinBox, 4);
    directionLayout->addWidget(lightYDirLabel, 1); directionLayout->addWidget(lightYDirSpinBox, 4);
    directionLayout->addWidget(lightZDirLabel, 1); directionLayout->addWidget(lightZDirSpinBox, 4);

    // position frame
    auto *posFrameLayout = new QVBoxLayout;
    posFrameLayout->addWidget(posTitleLabel);
    posFrameLayout->addLayout(posLayout);
    positionFrame->setLayout(posFrameLayout);

    // rotation frame
    auto *rotFrameLayout = new QVBoxLayout;
    rotFrameLayout->addWidget(rotationTitleLabel);
    rotFrameLayout->addLayout(rotLayout);
    rotationFrame->setLayout(rotFrameLayout);

    // scale frame
    auto *scaleFrameLayout = new QVBoxLayout;
    scaleFrameLayout->addWidget(scaleTitleLabel);
    scaleFrameLayout->addLayout(scaleLayout);
    scaleFrame->setLayout(scaleFrameLayout);

    // direction frame
    auto *directFrameLayout = new QVBoxLayout;
    directFrameLayout->addWidget(directionTitleLabel);
    directFrameLayout->addLayout(directionLayout);
    directionFrame->setLayout(directFrameLayout);

    // final transform
    auto *transformLayout = new QVBoxLayout;
    transformLayout->addWidget(positionFrame);
    transformLayout->addWidget(rotationFrame);
    transformLayout->addWidget(scaleFrame);
    transformLayout->addWidget(directionFrame);
    transformLayout->addStretch(1);
    // 暂时全部隐藏，按照选择的QListWidget的item来显示

    positionFrame->hide();
    rotationFrame->hide();
    scaleFrame->hide();
    directionFrame->hide();

    transformGroupBox->setLayout(transformLayout);

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
    // QToolButton and Actions
    connect(objectLoadShapeCubeAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectCube);
    connect(objectLoadShapeQuadAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectQuad);
    connect(objectLoadModelAction, &QAction::triggered,
            this, &MainWindow::onLoadModel);

    // QList
    connect(ui->objectList, &QListWidget::itemClicked,
            this, &MainWindow::onObjectItemSelect);

    // QLineEdit
    connect(nameLineEdit, &QLineEdit::editingFinished,
            this, &MainWindow::handleEditingFinished);


}

/************ slot functions ************/
void MainWindow::updateGLManager() {
    glManager->update();
}

void MainWindow::onLoadGameObjectCube() {
    int id = glManager->addObject(ObjectType::Cube);
    if(id == -1) {
        return;
    }

    QString objectDisplayName = "Cube " + QString::number(id);
    objectList->addItem(objectDisplayName);
}

void MainWindow::onLoadGameObjectQuad() {
    int id = glManager->addObject(ObjectType::Quad);
    if(id == -1) {
        return;
    }

    QString objectDisplayName = "Quad " + QString::number(id);
    objectList->addItem(objectDisplayName);
}

void MainWindow::onLoadModel() {
    // 打开文件选择对话框并只显示 .obj 文件。
    QString filePath = QFileDialog::getOpenFileName(
        this,                            // parent window
        "Load OBJ Model",                // dialog title
        modelDirectory,                // initial directory
        "OBJ Files (*.obj);;All Files (*)"  // file filter
    );

    if (!filePath.isEmpty()) {
        int id = glManager->addObject(filePath);
        if(id == -1) {
            return;
        }

        QString objectDisplayName = "Model " + QString::number(id);
        objectList->addItem(objectDisplayName);
    }
}

void MainWindow::onObjectItemSelect(QListWidgetItem *item) {
    if(item) {

    }
}

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

    if (watched == ui->objectList && event->type() == QEvent::FocusOut) {
        transformGroupBox->setLayout(nullptr);
    }

    return QWidget::eventFilter(watched, event);
}

