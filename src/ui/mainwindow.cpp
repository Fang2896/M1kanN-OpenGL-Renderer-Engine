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

    objectLoadShapeUnitCubeAction = objectShapeSelectMenu->addAction("Unit Cube");
    objectLoadShapeCubeAction = objectShapeSelectMenu->addAction("Cube");
    objectLoadShapeQuadAction = objectShapeSelectMenu->addAction("Quad");
    objectLoadShapePlaneAction = objectShapeSelectMenu->addAction("Plane");
    objectLoadShapeSphereAction = objectShapeSelectMenu->addAction("Sphere");
    objectLoadShapeCapsuleAction = objectShapeSelectMenu->addAction("Capsule");

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
    skyboxComboBox = ui->skyboxComboBox;

    enableLightingCheckBox = ui->enableLightingCheckBox;
    enableLineModeCheckBox = ui->enableLineModeCheckBox;
    enableDepthMapCheckBox = ui->enableDepthMapCheckBox;
    postProcessingComboBox = ui->postProcessingComboBox;

    cullModeComboBox = ui->cullModeComboBox;

    // 操作时隐藏或者显示：
    positionFrame = ui->positionFrame;
    rotationFrame = ui->rotationFrame;
    scaleFrame = ui->scaleFrame;
    directionFrame = ui->directionFrame;
    materialFrame = ui->materialFrame;

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

    enableMatCheckBox   = ui->enableMatCheckBox;
    shaderComboBox      = ui->shaderComboBox;
    matShininessLabel   = ui->matShininessLabel;    matShininessBar = ui->matShininessBar;
    matAmbientLabel     = ui->matAmbientLabel;      matAmbientChooseButton = ui->matAmbientChooseButton;
    matDiffuseLabel     = ui->matDiffuseLabel;      matDiffuseChooseButton = ui->matDiffuseChooseButton;
    matSpecularLabel    = ui->matSpecularLabel;     matSpecularChooseButton = ui->matSpecularChooseButton;
    matDiffuseTexLabel  = ui->matDiffuseTexLabel;   matDiffuseTexChooseButton = ui->matDiffuseTexChooseButton;
    matSpecularTexLabel = ui->matSpecularTexLabel;  matSpecularTexChooseButton = ui->matSpecularTexChooseButton;

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
    auto *comboEnvLayout = new QHBoxLayout;
    comboEnvLayout->addWidget(cullModeComboBox);
    comboEnvLayout->addWidget(skyboxComboBox);

    auto *vDashLayout = new QVBoxLayout;
    vDashLayout->addLayout(comboEnvLayout);
    vDashLayout->addWidget(enableLineModeCheckBox);
    vDashLayout->addWidget(enableLightingCheckBox);
    envTab->setLayout(vDashLayout);

    auto *vPostProcessingLayout = new QVBoxLayout;
    vPostProcessingLayout->addWidget(postProcessingComboBox);
    vPostProcessingLayout->addWidget(enableDepthMapCheckBox);
    postProcessingTab->setLayout(vPostProcessingLayout);

    // name and display Layout
    auto *hDisplayLayout = new QHBoxLayout;
    hDisplayLayout->addWidget(nameCheckBox);
    hDisplayLayout->addWidget(nameLineEdit);

    auto *posLayout = new QHBoxLayout;
    posLayout->addWidget(xPosLabel);    posLayout->addWidget(xPosSpinBox,1);
    posLayout->addWidget(yPosLabel);    posLayout->addWidget(yPosSpinBox,1);
    posLayout->addWidget(zPosLabel);    posLayout->addWidget(zPosSpinBox,1);

    // for objects:
    // rotation
    auto *rotLayout = new QHBoxLayout;
    rotLayout->addWidget(objXRotLabel); rotLayout->addWidget(objXRotSpinBox, 1);
    rotLayout->addWidget(objYRotLabel); rotLayout->addWidget(objYRotSpinBox, 1);
    rotLayout->addWidget(objZRotLabel); rotLayout->addWidget(objZRotSpinBox, 1);

    // scale
    auto *scaleLayout = new QHBoxLayout;
    scaleLayout->addWidget(objXScaleLabel); scaleLayout->addWidget(objXScaleSpinBox, 1);
    scaleLayout->addWidget(objYScaleLabel); scaleLayout->addWidget(objYScaleSpinBox, 1);
    scaleLayout->addWidget(objZScaleLabel); scaleLayout->addWidget(objZScaleSpinBox, 1);

    // direct
    auto *directionLayout = new QHBoxLayout;
    directionLayout->addWidget(lightXDirLabel); directionLayout->addWidget(lightXDirSpinBox, 1);
    directionLayout->addWidget(lightYDirLabel); directionLayout->addWidget(lightYDirSpinBox, 1);
    directionLayout->addWidget(lightZDirLabel); directionLayout->addWidget(lightZDirSpinBox, 1);

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

    // material frame
    auto *matShininessLayout = new QHBoxLayout;
    matShininessLayout->addWidget(matShininessLabel);
    matShininessLayout->addWidget(matShininessBar);

    auto *matAmbientColorLayout = new QHBoxLayout;
    matAmbientColorLayout->addWidget(matAmbientLabel);
    matAmbientColorLayout->addWidget(matAmbientChooseButton);

    auto *matDiffuseColorLayout = new QHBoxLayout;
    matDiffuseColorLayout->addWidget(matDiffuseLabel);
    matDiffuseColorLayout->addWidget(matDiffuseChooseButton);

    auto *matSpecularColorLayout = new QHBoxLayout;
    matSpecularColorLayout->addWidget(matSpecularLabel);
    matSpecularColorLayout->addWidget(matSpecularChooseButton);

    auto *matDiffuseTexLayout = new QHBoxLayout;
    matDiffuseTexLayout->addWidget(matDiffuseTexLabel);
    matDiffuseTexLayout->addWidget(matDiffuseTexChooseButton);

    auto *matSpecularTexLayout = new QHBoxLayout;
    matSpecularTexLayout->addWidget(matSpecularTexLabel);
    matSpecularTexLayout->addWidget(matSpecularTexChooseButton);

    // enable and shader changed
    auto *topMaterialLayout = new QHBoxLayout;
    topMaterialLayout->addWidget(enableMatCheckBox);
    topMaterialLayout->addWidget(shaderComboBox);

    auto *matFrameLayout = new QVBoxLayout;
    matFrameLayout->addLayout(topMaterialLayout);
    matFrameLayout->addLayout(matShininessLayout);
    matFrameLayout->addLayout(matAmbientColorLayout);
    matFrameLayout->addLayout(matDiffuseColorLayout);
    matFrameLayout->addLayout(matSpecularColorLayout);
    matFrameLayout->addLayout(matDiffuseTexLayout);
    matFrameLayout->addLayout(matSpecularTexLayout);
    materialFrame->setLayout(matFrameLayout);

    // final transform
    auto *transformLayout = new QVBoxLayout;
    transformLayout->addWidget(positionFrame);
    transformLayout->addWidget(rotationFrame);
    transformLayout->addWidget(scaleFrame);
    transformLayout->addWidget(directionFrame);
    transformLayout->addWidget(materialFrame);
    transformLayout->addStretch(1);

    // 暂时全部隐藏，按照选择的QListWidget的item来显示
    positionFrame->hide();
    rotationFrame->hide();
    scaleFrame->hide();
    directionFrame->hide();
    materialFrame->hide();

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
    connect(objectLoadShapeUnitCubeAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectUnitCube);
    connect(objectLoadShapeCubeAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectCube);
    connect(objectLoadShapeQuadAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectQuad);
    connect(objectLoadShapePlaneAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectPlane);
    connect(objectLoadShapeSphereAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectSphere);
    connect(objectLoadShapeCapsuleAction, &QAction::triggered,
            this, &MainWindow::onLoadGameObjectCapsule);

    connect(objectLoadModelAction, &QAction::triggered,
            this, &MainWindow::onLoadModel);
    connect(objectDeleteButton, &QPushButton::clicked,
            this, &MainWindow::onObjectDeleteButtonClicked);

    // QList
    connect(objectList, &QListWidget::itemClicked,
            this, &MainWindow::onObjectItemSelect);
    connect(objectList, &QListWidget::currentItemChanged,
            this, &MainWindow::handleObjectItemChanged);

    // Dash Configure
    connect(enableLightingCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onEnableLightingCheckBox);
    connect(enableLineModeCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onEnableLineModeCheckBox);
    connect(enableDepthMapCheckBox, &QCheckBox::stateChanged,
            this, &MainWindow::onEnableDepthModeCheckBox);

    connect(cullModeComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::onCullModeComboBoxChanged);
    connect(postProcessingComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::onPostProcessingModeComboBoxChanged);
    connect(skyboxComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::onSkyboxComboBoxChanged);

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

    // material
    connect(matShininessBar, &QSlider::valueChanged, this,
            &MainWindow::onShininessBarChange);

    connect(shaderComboBox, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::onShaderComboBoxChanged);

    connect(matAmbientChooseButton, &QPushButton::clicked,
            this, &MainWindow::onAmbientColorButtonClicked);
    connect(matDiffuseChooseButton, &QPushButton::clicked,
            this, &MainWindow::onDiffuseColorButtonClicked);
    connect(matSpecularChooseButton, &QPushButton::clicked,
            this, &MainWindow::onSpecularColorButtonClicked);
    connect(matDiffuseTexChooseButton, &QPushButton::clicked,
            this, &MainWindow::onLoadDiffuseTextureButtonClicked);
    connect(matSpecularTexChooseButton, &QPushButton::clicked,
            this, &MainWindow::onLoadSpecularTextureButtonClicked);

    // QLineEdit
    connect(nameLineEdit, &QLineEdit::editingFinished,
            this, &MainWindow::handleEditingFinished);

}

/************ slot functions ************/
void MainWindow::updateGLManager() {
    glManager->update();
}

void MainWindow::onLoadGameObjectUnitCube() {
    int id = glManager->addObject(ObjectType::UnitCube);
    if(id == -1) {
        return;
    }
    qDebug() << "Load Shape : Unit Cube";
    auto *item = new QListWidgetItem("Unit Cube - id:" + QString::number(id), objectList);
    item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
    objectList->addItem(item);
}

void MainWindow::onLoadGameObjectCube() {
    bool ok;
    int width = QInputDialog::getInt(this, tr("Enter Width"), tr("Width:"), 1, 1, 1500, 1, &ok);

    if (ok) {
        qDebug() << "Load Shape : Cube : " << "Width:" << width;
        int id = glManager->addObject(ObjectType::Cube, static_cast<float>(width));
        if(id == -1) {
            return;
        }

        auto *item = new QListWidgetItem("Cube " + QString::number(width) + " - id:" + QString::number(id), objectList);
        item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
        objectList->addItem(item);
    }
}

void MainWindow::onLoadGameObjectQuad() {
    int id = glManager->addObject(ObjectType::Quad);
    if(id == -1) {
        return;
    }
    qDebug() << "Load Shape : Quad";
    auto *item = new QListWidgetItem("Quad " + QString::number(id), objectList);
    item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
    objectList->addItem(item);
}

void MainWindow::onLoadGameObjectPlane() {
    bool ok;
    int width = QInputDialog::getInt(this, tr("Enter Width"), tr("Width:"),
                                     1, 1, 1500, 1, &ok);

    if (ok) {
        int height = QInputDialog::getInt(this, tr("Enter Height"), tr("Height:"),
                                          1, 1, 1500, 1, &ok);
        if (ok) {
            qDebug() << "Load Shape : Plane : " << "Width:" << width << "Height:" << height;

            int id = glManager->addObject(ObjectType::Plane, static_cast<float>(width), static_cast<float>(height));
            if(id == -1) {
                return;
            }

            auto *item = new QListWidgetItem("Plane " + QString::number(width) + "x" + QString::number(height) + " - id:" + QString::number(id), objectList);
            item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
            objectList->addItem(item);
        }
    }
}

void MainWindow::onLoadGameObjectSphere() {
    bool ok;
    double radius = QInputDialog::getDouble(this, tr("Enter Radius"), tr("Radius:"),
                                     1, 0.00001, 1000.00000, 5, &ok);

    if (ok) {
        int resolution = QInputDialog::getInt(this, tr("Enter Resolution"), tr("Resolution:"),
                                          1, 1, 1000, 1, &ok);
        if (ok) {
            qDebug() << "Load Shape : Sphere : " << "Radius:" << radius << "Resolution:" << resolution;

            int id = glManager->addObject(ObjectType::Sphere, static_cast<float>(radius), static_cast<float>(resolution));
            if(id == -1) {
                return;
            }

            auto *item = new QListWidgetItem("Sphere " + QString::number(radius) + "x" + QString::number(resolution) + " - id:" + QString::number(id), objectList);
            item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
            objectList->addItem(item);
        }
    }
}

void MainWindow::onLoadGameObjectCapsule() {
//    bool ok;
//    double radius = QInputDialog::getDouble(this, tr("Enter Radius"), tr("Radius:"), 1, 0.00001, 150, 5, &ok);
//
//    if (ok) {
//        double height = QInputDialog::getDouble(this, tr("Enter Height"), tr("Height:"), 1, 0.00001, 150, 5, &ok);
//        if (ok) {
//            qDebug() << "Load Shape : Capsule : " << "Radius:" << radius << "Height:" << height;
//
//            int id = glManager->addObject(ObjectType::Capsule, static_cast<float>(radius), static_cast<float>(height));
//            if(id == -1) {
//                return;
//            }
//
//            auto *item = new QListWidgetItem("Capsule " + QString::number(radius) + "x" + QString::number(height), objectList);
//            item->setData(objectDataBaseIdRole, static_cast<qulonglong>(id));  // 存储ID
//            objectList->addItem(item);
//        }
//    }
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

    currentObjectID = -1;
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

void MainWindow::onEnableDepthModeCheckBox(int state) {
    bool depthMode;
    if (state == Qt::Checked) {
        depthMode = true;
    } else {
        depthMode = false;
    }

    glManager->setDepthMode(depthMode);
}

void MainWindow::onCullModeComboBoxChanged(int index) {
    if(index == 0) {    // back
        glManager->setCullMode(CullModeType::Disable);
    } else if (index == 1) {    // disable
        glManager->setCullMode(CullModeType::Back);
    } else if (index == 2) {    // front
        glManager->setCullMode(CullModeType::Front);
    } else if (index == 3) {
        glManager->setCullMode(CullModeType::Front_Back);
    }
}

void MainWindow::onPostProcessingModeComboBoxChanged(int index) {
    auto type = static_cast<PostProcessingType>(index);
    glManager->setPostProcessingType(type);
}

void MainWindow::onSkyboxComboBoxChanged(int index) {
    auto type = static_cast<SkyboxType>(index);
    glManager->setSkyboxPath(type);
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

    tempObj->setVisible(display);
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

// material
void MainWindow::onShininessBarChange(int value) {
    if(currentObjectID == -1) {
        return;
    }

    auto tempObj = glManager->getTargetGameObject(currentObjectID);
    Material tempMat = tempObj->getMaterial();
    tempMat.shininess = static_cast<float>(value) / 128.0f;
    tempObj->setMaterial(tempMat);
}
void MainWindow::onAmbientColorButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    QColor color = QColorDialog::getColor(Qt::white, this, "Select a Ambient color");
    if(color.isValid()) {
        auto tempObj = glManager->getTargetGameObject(currentObjectID);
        qDebug() << "User selected Ambient color:" << color;
        Material tempMat = tempObj->getMaterial();
        QVector3D ambientColor = QVector3D(color.redF(), color.greenF(), color.blueF());
        tempObj->setAmbientColor(ambientColor);

        QString ambientStyle = QString("background-color: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
        matAmbientChooseButton->setStyleSheet(ambientStyle);

        QString ambientText = QString("R:%1,G:%2,B:%3").arg(color.red()).arg(color.green()).arg(color.blue());
        matAmbientChooseButton->setText(ambientText);
    }
}
void MainWindow::onDiffuseColorButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    QColor color = QColorDialog::getColor(Qt::white, this, "Select a Diffuse color");
    if(color.isValid()) {
        auto tempObj = glManager->getTargetGameObject(currentObjectID);
        qDebug() << "User selected Diffuse color:" << color;
        Material tempMat = tempObj->getMaterial();
        QVector3D diffuseColor = QVector3D(color.redF(), color.greenF(), color.blueF());
        tempObj->setDiffuseColor(diffuseColor);

        QString diffuseStyle = QString("background-color: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
        matDiffuseChooseButton->setStyleSheet(diffuseStyle);

        QString diffuseText = QString("R:%1,G:%2,B:%3").arg(color.red()).arg(color.green()).arg(color.blue());
        matDiffuseChooseButton->setText(diffuseText);
    }
}
void MainWindow::onSpecularColorButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    QColor color = QColorDialog::getColor(Qt::white, this, "Select a Specular color");
    if(color.isValid()) {
        auto tempObj = glManager->getTargetGameObject(currentObjectID);
        qDebug() << "User selected Specular color:" << color;
        Material tempMat = tempObj->getMaterial();
        QVector3D specularColor = QVector3D(color.redF(), color.greenF(), color.blueF());
        tempObj->setSpecularColor(specularColor);

        QString specularStyle = QString("background-color: rgb(%1,%2,%3);").arg(color.red()).arg(color.green()).arg(color.blue());
        matSpecularChooseButton->setStyleSheet(specularStyle);

        QString specularText = QString("R:%1,G:%2,B:%3").arg(color.red()).arg(color.green()).arg(color.blue());
        matSpecularChooseButton->setText(specularText);
    }
}
void MainWindow::onLoadDiffuseTextureButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    QString filters = "Image files (*.png *.jpg *.jpeg *.bmp );;All files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(this, "Select an image",
                                                    textureDirectory, filters);
    auto tempObj = glManager->getTargetGameObject(currentObjectID);

    if (!filePath.isEmpty()) {
        qDebug() << "User selected Diffuse Texture image path:" << filePath;
        tempObj->loadDiffuseTexture(filePath);
    }

    QString texName = UtilAlgorithms::getFileNameFromPath(filePath);
    matDiffuseTexChooseButton->setText(texName);
}
void MainWindow::onLoadSpecularTextureButtonClicked() {
    if(currentObjectID == -1) {
        return;
    }

    QString filters = "Image files (*.png *.jpg *.jpeg *.bmp );;All files (*.*)";
    QString filePath = QFileDialog::getOpenFileName(this, "Select an image",
                                                    textureDirectory, filters);
    auto tempObj = glManager->getTargetGameObject(currentObjectID);

    if (!filePath.isEmpty()) {
        qDebug() << "User selected Specular Texture image path:" << filePath;
        tempObj->loadSpecularTexture(filePath);
    }

    QString texName = UtilAlgorithms::getFileNameFromPath(filePath);
    matSpecularTexChooseButton->setText(texName);
}

// 控制折射和反射
void MainWindow::onShaderComboBoxChanged(int index) {
    auto type = static_cast<ShaderType>(index);
    if(currentObjectID == -1)
        return;
    auto tempObj = glManager->getTargetGameObject(currentObjectID);

    if(type == ShaderType::Default) {
        tempObj->setReflection(false);
        tempObj->setRefraction(false);
        qDebug() << "Shader Change to " << "Default";
    } else if (type == ShaderType::Reflection) {
        tempObj->setReflection(true);
        qDebug() << "Shader Change to " << "Reflection";
    } else if (type == ShaderType::Refraction) {
        tempObj->setRefraction(true);
        qDebug() << "Shader Change to " << "Refraction";
    }
}

// 被点击的时候，返回相应的信息，以及显示对应的控件
void MainWindow::onObjectItemSelect(QListWidgetItem *item) {
    if(item) {
        currentObjectID = item->data(objectDataBaseIdRole).toInt();
        int id = currentObjectID;
        Qt::CheckState displayState;
        auto temp = glManager->getTargetGameObject(id);

        if(temp->getVisible()) {
            displayState = Qt::Checked;
        } else {
            displayState = Qt::Unchecked;
        }
        nameCheckBox->setCheckState(displayState);

        positionFrame->show();
        rotationFrame->show();
        scaleFrame->show();

        // 大于2的说明可能自带texture，就不设置了
        if(temp->getMeshCount() == 1) {
            materialFrame->show();
            setObjectMaterialToFrame(temp->getMaterial());
        }

        setObjectTransformToSpinBox(temp->getPosition(),
                                    temp->getRotation(), temp->getScale());

        qDebug() << "Select Item : " << temp->displayName;
        nameLineEdit->setText(temp->displayName);

        // 绘制被选择的边框
        temp->setDrawOutline(GL_TRUE);
    }
}

void MainWindow::handleObjectItemChanged(QListWidgetItem *current, QListWidgetItem *previous) {
    if(previous != nullptr) {
        int prevId = previous->data(objectDataBaseIdRole).toInt();
        auto prevObj = glManager->getTargetGameObject(prevId);

        prevObj->setDrawOutline(GL_FALSE);
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
void MainWindow::setObjectTransformToSpinBox(QVector3D pos, QVector3D rot, QVector3D sca) {
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

void MainWindow::setObjectMaterialToFrame(const Material& mat) {
    matShininessBar->setValue((int)(mat.shininess * 255));
    QColor ambientColor(static_cast<int>(mat.ambientColor.x() * 255),
                 static_cast<int>(mat.ambientColor.y() * 255),
                 static_cast<int>(mat.ambientColor.z() * 255));
    QColor diffuseColor(static_cast<int>(mat.diffuseColor.x() * 255),
                        static_cast<int>(mat.diffuseColor.y() * 255),
                        static_cast<int>(mat.diffuseColor.z() * 255));
    QColor specularColor(static_cast<int>(mat.specularColor.x() * 255),
                        static_cast<int>(mat.specularColor.y() * 255),
                        static_cast<int>(mat.specularColor.z() * 255));

    QString ambientStyle = QString("background-color: rgb(%1,%2,%3);").arg(ambientColor.red()).arg(ambientColor.green()).arg(ambientColor.blue());
    QString diffuseStyle = QString("background-color: rgb(%1,%2,%3);").arg(diffuseColor.red()).arg(diffuseColor.green()).arg(diffuseColor.blue());
    QString specularStyle = QString("background-color: rgb(%1,%2,%3);").arg(specularColor.red()).arg(specularColor.green()).arg(specularColor.blue());

    matAmbientChooseButton->setStyleSheet(ambientStyle);
    matDiffuseChooseButton->setStyleSheet(diffuseStyle);
    matSpecularChooseButton->setStyleSheet(specularStyle);

    QString ambientText = QString("R:%1,G:%2,B:%3").arg(ambientColor.red()).arg(ambientColor.green()).arg(ambientColor.blue());
    QString diffuseText = QString("R:%1,G:%2,B:%3").arg(diffuseColor.red()).arg(diffuseColor.green()).arg(diffuseColor.blue());
    QString specularText = QString("R:%1,G:%2,B:%3").arg(specularColor.red()).arg(specularColor.green()).arg(specularColor.blue());

    matAmbientChooseButton->setText(ambientText);
    matDiffuseChooseButton->setText(diffuseText);
    matSpecularChooseButton->setText(specularText);

    // TODO: clear 逻辑，以及如何处理以及有texture的物体
    if(mat.texture_diffuse1 != nullptr) {
        QString speTexName = UtilAlgorithms::getFileNameFromPath(mat.texture_diffuse1->path);
        matDiffuseTexChooseButton->setText(speTexName);
    } else {
        matDiffuseTexChooseButton->setText("load texture");    // 没有，或者有多个
    }

    if(mat.texture_specular1 != nullptr) {
        QString speTexName = UtilAlgorithms::getFileNameFromPath(mat.texture_specular1->path);
        matSpecularTexChooseButton->setText(speTexName);
    } else {
        matSpecularTexChooseButton->setText("load texture");    // 没有，或者有多个
    }
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
                    auto tempObj = glManager->getTargetGameObject(currentObjectID);
                    qDebug() << "Release Item : "
                             << tempObj->displayName;
                    tempObj->setDrawOutline(GL_FALSE);
                    currentObjectID = -1;
                }

                positionFrame->hide();
                rotationFrame->hide();
                scaleFrame->hide();
                directionFrame->hide();
                materialFrame->hide();

                objectList->clearSelection();
                objectList->clearFocus();
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}
