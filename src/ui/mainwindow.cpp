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
    currentObjectID = -1;
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

// used to set widget size for now
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    int midHeight = glDashSplitter->height();
    int dashHeight = midHeight / 5;
    int glHeight = midHeight - dashHeight;
    glDashSplitter->setSizes({glHeight, dashHeight});

    // 初始大小比例
    int totalWidth = contentSplitter->width();
    int leftAndRightWidth = totalWidth / 5; // 根据1:3:1的比例计算左和右部分的宽度
    int midWidth = 3 * leftAndRightWidth;
    contentSplitter->setSizes({leftAndRightWidth, midWidth, leftAndRightWidth});
}

void MainWindow::initWidget() {
    contentSplitter = new QSplitter(Qt::Horizontal, this);
    glDashSplitter = new QSplitter(Qt::Vertical, this);

    sceneGroupBox = ui->sceneGroupBox;
    lightTitleLabel = ui->lightTitleLabel;
    objectTitleLabel = ui->objectTitleLabel;

    objectAddButton = ui->objectAddButton;
    objectDeleteButton = ui->objectDeleteButton;
    lightAddButton = ui->lightAddButton;
    lightDeleteButton = ui->lightDeleteButton;

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
    envTab = ui->envTab;
    postProcessingTab = ui->postProcessingTab;

    enableLightingCheckBox = ui->enableLightingCheckBox;
    enableLineModeCheckBox = ui->enableLineModeCheckBox;

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
    hLightLayout->addWidget(lightDeleteButton);

    auto *hObjLayout = new QHBoxLayout;
    hObjLayout->addWidget(objectTitleLabel);
    hObjLayout->addWidget(objectAddButton);
    hObjLayout->addWidget(objectDeleteButton);

    auto *vSceneLayout = new QVBoxLayout;
    vSceneLayout->addLayout(hLightLayout);
    vSceneLayout->addWidget(lightList, 2);
    vSceneLayout->addLayout(hObjLayout);
    vSceneLayout->addWidget(objectList, 2);

    sceneGroupBox->setLayout(vSceneLayout);

    // Dash configure layout
    auto *vDashLayout = new QVBoxLayout;
    vDashLayout->addWidget(enableLineModeCheckBox);
    vDashLayout->addWidget(enableLightingCheckBox);
    envTab->setLayout(vDashLayout);

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

    // Mid Layout (use splitter)
    glDashSplitter->addWidget(glManager);
    glDashSplitter->addWidget(configureDashTab);

    // Menu Bar
    mainMenuBar->addMenu(fileMenu);
    mainMenuBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    fileMenu->addAction(loadModelAction);

    // Content Layout (use splitter)
    // auto *contentLayout = new QHBoxLayout;
    contentSplitter->addWidget(sceneGroupBox);
    contentSplitter->addWidget(glDashSplitter);
    contentSplitter->addWidget(inspectorGroupBox);

    // Total Layout
    auto *totalLayout = new QVBoxLayout;
    totalLayout->addWidget(mainMenuBar);
    totalLayout->addWidget(contentSplitter);
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
    connect(objectDeleteButton, &QPushButton::clicked,
            this, &MainWindow::onObjectDeleteButtonClicked);



    // QList
    connect(ui->objectList, &QListWidget::itemClicked,
            this, &MainWindow::onObjectItemSelect);

    // Dash Configure
    connect(enableLightingCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onEnableLightingCheckBox);
    connect(enableLineModeCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onEnableLineModeCheckBox);

    // Inspector:
    connect(nameCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onDisplayCheckBox);

    // QSpinBox
    connect(xPosSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onXPosSpinBoxValueChanged);
    connect(yPosSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onYPosSpinBoxValueChanged);
    connect(zPosSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onZPosSpinBoxValueChanged);

    connect(objXRotSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onXRotSpinBoxValueChanged);
    connect(objYRotSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onYRotSpinBoxValueChanged);
    connect(objZRotSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onZRotSpinBoxValueChanged);

    connect(objXScaleSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onXScaSpinBoxValueChanged);

    connect(objYScaleSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onYScaSpinBoxValueChanged);

    connect(objZScaleSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::onZScaSpinBoxValueChanged);

    // 这里还要加一个direction，等灯光系统完成后再说吧


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

    auto *item = new QListWidgetItem("Cube " + QString::number(id), objectList);
    item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
    objectList->addItem(item);
}

void MainWindow::onLoadGameObjectQuad() {
    int id = glManager->addObject(ObjectType::Quad);
    if(id == -1) {
        return;
    }

    auto *item = new QListWidgetItem("Quad " + QString::number(id), objectList);
    item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
    objectList->addItem(item);
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

        auto temp = glManager->getTargetGameObject(id);
        auto *item = new QListWidgetItem(temp->displayName, objectList);
        item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
        objectList->addItem(item);
    }
}

void MainWindow::onObjectDeleteButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    auto tempItem = getItemById(objectList, currentObjectID);
    int row = objectList->row(tempItem);
    objectList->takeItem(row);
    delete tempItem;
    tempItem = nullptr;
    glManager->deleteObject(currentObjectID);
}

// dash configure slot functions
void MainWindow::onEnableLightingCheckBox(int state) {
    bool enableLighting;
    if (state == Qt::Checked) {
        enableLighting = true;
    } else {
        enableLighting = false;
    }

    glManager->setEnableLighting(enableLighting);
}

void MainWindow::onEnableLineModeCheckBox(int state) {
    bool enableLineMode;
    if (state == Qt::Checked) {
        enableLineMode = true;
    } else {
        enableLineMode = false;
    }

    glManager->setLineMode(enableLineMode);
}

void MainWindow::onDisplayCheckBox(int state) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    bool display;
    if (state == Qt::Checked) {
        display = true;
    } else {
        display = false;
    }

    tempObj->display = display;
}

// slot functions of SpinBox
void MainWindow::onXPosSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }
    // 注意OBJ的存在性？
    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempPos = tempObj->getPosition();
    tempPos.setX(value);
    tempObj->setPosition(tempPos);
}
void MainWindow::onYPosSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempPos = tempObj->getPosition();
    tempPos.setY(value);
    tempObj->setPosition(tempPos);
}
void MainWindow::onZPosSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempPos = tempObj->getPosition();
    tempPos.setZ(value);
    tempObj->setPosition(tempPos);
}

void MainWindow::onXRotSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempRot = tempObj->getRotation();
    tempRot.setX(value);
    tempObj->setRotation(tempRot);
}
void MainWindow::onYRotSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempRot = tempObj->getRotation();
    tempRot.setY(value);
    tempObj->setRotation(tempRot);
}
void MainWindow::onZRotSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempRot = tempObj->getRotation();
    tempRot.setZ(value);
    tempObj->setRotation(tempRot);
}

void MainWindow::onXScaSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempScale = tempObj->getScale();
    tempScale.setX(value);
    tempObj->setScale(tempScale);
}
void MainWindow::onYScaSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempScale = tempObj->getScale();
    tempScale.setY(value);
    tempObj->setScale(tempScale);
}
void MainWindow::onZScaSpinBoxValueChanged(double value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    QVector3D tempScale = tempObj->getScale();
    tempScale.setZ(value);
    tempObj->setScale(tempScale);
}


// 被点击的时候，返回相应的信息，以及显示对应的控件
void MainWindow::onObjectItemSelect(QListWidgetItem *item) {
    if(item) {
        currentObjectID = item->data(objectDataBaseIdRole).toInt();
        int id = currentObjectID;
        Qt::CheckState displayState;
        auto temp = glManager->getTargetGameObject(id);

        if(temp->display) {
            displayState = Qt::Checked;
        } else {
            displayState = Qt::Unchecked;
        }
        nameCheckBox->setCheckState(displayState);

        positionFrame->show();
        rotationFrame->show();
        scaleFrame->show();

        setObjectTransformToSpinBox(temp, temp->getPosition(),
                                    temp->getRotation(), temp->getScale());

        qDebug() << "Select Item : " << temp->displayName;
        nameLineEdit->setText(temp->displayName);
    }
}

void MainWindow::handleEditingFinished() {
    QString currentText = nameLineEdit->text();
    nameLineEdit->clearFocus();
    if(nameLineEdit->hasFocus()) {
        return;
    }

    if(currentObjectID != -1) {
        qDebug() << "LineEdit has lost focus with text: " << currentText;
        auto tempObj = glManager->getTargetGameObject(currentObjectID);
        auto tempItem = getItemById(objectList, currentObjectID);
        if(tempItem != nullptr && tempObj != nullptr) {
            tempItem->setText(currentText);
            tempObj->displayName = currentText;
        }
    }

}

// 辅助函数：
void MainWindow::setObjectTransformToSpinBox(const std::shared_ptr<GameObject>& ptr ,QVector3D pos, QVector3D rot, QVector3D sca) {
    xPosSpinBox->setValue(pos.x());
    yPosSpinBox->setValue(pos.y());
    zPosSpinBox->setValue(pos.z());

    objXRotSpinBox->setValue(rot.x());
    objYRotSpinBox->setValue(rot.y());
    objZRotSpinBox->setValue(rot.z());

    objXScaleSpinBox->setValue(sca.x());
    objYScaleSpinBox->setValue(sca.y());
    objZScaleSpinBox->setValue(sca.z());
}

QListWidgetItem* MainWindow::getItemById(QListWidget* listWidget, int id) const {
    for(int i = 0; i < listWidget->count(); i++) {
        QListWidgetItem* item = listWidget->item(i);
        if(item->data(objectDataBaseIdRole).toInt() == id) {
            return item;
        }
    }

    return nullptr;  // 如果找不到对应ID的item则返回nullptr
}



// filter functions
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::MouseButtonPress) {
        QWidget *focusedWidget = QApplication::focusWidget();  // 获取当前拥有焦点的部件
        QWidget *clickedWidget = qApp->widgetAt(QCursor::pos());  // 获取鼠标点击的部件

        // 如果当前焦点在 QLineEdit 上
        if (qobject_cast<QLineEdit*>(focusedWidget)) {
            if (clickedWidget != focusedWidget) {
                focusedWidget->clearFocus();  // 如果点击的不是 QLineEdit，清除焦点
            }
        }

        // 如果点击的部件不是 objectList 或其子部件，则隐藏 posFrame
        if (!objectList->isAncestorOf(clickedWidget)) {
            if(clickedWidget != focusedWidget) {
                if(currentObjectID != -1) {
                    qDebug() << "Release Item : "
                             << glManager->getTargetGameObject(currentObjectID)->displayName;
                    currentObjectID = -1;
                }

                positionFrame->hide();
                rotationFrame->hide();
                scaleFrame->hide();
                directionFrame->hide();

                objectList->clearSelection();
                objectList->clearFocus();
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}
