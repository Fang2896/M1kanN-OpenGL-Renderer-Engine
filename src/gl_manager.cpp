//
// Created by fangl on 2023/9/19.
//

#include "gl_manager.hpp"

const QVector3D CAMERA_POSITION(0.0f, 0.5f, 3.0f);


GLManager::GLManager(QWidget* parent, int width, int height)
    : QOpenGLWidget(parent)
{
    this->setGeometry(10, 20, width, height);
}

GLManager::~GLManager() = default;

/********* OpenGL Functions *********/
void GLManager::initializeGL() {
    initConfigureVariables();
    initOpenGLSettings();
    initShaders();    // shader

    // TODO: 这里可以从coordinate改成各种绘制精灵？
    initShaderValue();

    // member mangers
    // object manager, resource manager...
    m_camera = std::make_unique<Camera>(CAMERA_POSITION, defaultCameraMoveSpeed);
    coordinate = std::make_unique<Coordinate>();
    coordinate->initCoordinate();

    auto tempCubePtr = std::make_shared<GameObject>();
    objectMap[tempCubePtr->getObjectID()] = tempCubePtr;
    tempCubePtr->setPosition({-3,3,-3});
    tempCubePtr->loadDiffuseTexture(":textures/assets/textures/box_diffuse.png");
    tempCubePtr->loadSpecularTexture(":textures/assets/textures/box_specular.png");
    tempCubePtr->displayName = "Wooden Cube";

//    objectMap.push_back(std::make_shared<GameObject>(modelDirectory + "/tiger/tiger.obj"));    // 默认是正方形
//    objectMap[1]->setPosition({-6.0f,0.0f,0.0f});
//    objectMap[1]->setScale(0.005);
//    objectMap[1]->displayName = "Tiger";
//
//    objectMap.push_back(std::make_shared<GameObject>(modelDirectory + "/buddha/buddha.obj"));    // 默认是正方形
//    objectMap[2]->setPosition({-3,0,0});
//    objectMap[2]->setRotation({-90,0,0});
//    objectMap[2]->setScale(0.005);
//    objectMap[2]->displayName = "Buddha";
//
//    objectMap.push_back(std::make_shared<GameObject>(modelDirectory + "/bunny/bunny.obj"));    // 默认是正方形
//    objectMap[3]->displayName = "Bunny";
//    objectMap[3]->setScale(0.005);
//    objectMap[3]->setPosition({0,3,0});
//
//    objectMap.push_back(std::make_shared<GameObject>(modelDirectory + "/cat/cat.obj"));    // 默认是正方形
//    objectMap[4]->displayName = "Cat";
//    objectMap[4]->setScale(0.005f);
//    objectMap[4]->setPosition({3,0,0});
//
//    objectMap.push_back(std::make_shared<GameObject>(modelDirectory + "/nanosuit/nanosuit.obj"));    // 默认是正方形
//    objectMap[5]->displayName = "NanoSuit";
//    objectMap[5]->setScale(0.2f);
//    objectMap[5]->setPosition({6,0,0});

    // start timer
    eTimer.start();
}

void GLManager::resizeGL(int w, int h) {
    glFunc->glViewport(0, 0, w, h);
}

void GLManager::paintGL() {
    // time and position data
    GLfloat currentFrame = (GLfloat)eTimer.elapsed() / 100;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->handleInput(deltaTime);
    this->updateRenderData();

    ResourceManager::getShader("coordShader")->use();
    coordinate->drawCoordinate();
    ResourceManager::getShader("coordShader")->release();

    drawObjects();
}

void GLManager::initShaders() {
    ResourceManager::loadShader("coordShader",
                                ":/shaders/assets/shaders/coordShader.vert",
                                ":/shaders/assets/shaders/coordShader.frag");

    qDebug() << "======= Done Init Coordinate Shaders ========";
}

// 在物体初始化后，或者增加物体，改变这里边的参数后调用！
void GLManager::initShaderValue() {
    initLightInfo();    // 要在shaderInit之前

    // coordinate matrix configuration （因为坐标位置是不变的）
    QMatrix4x4 model;
    model.setToIdentity();
    model.scale(10.0f);
    ResourceManager::getShader("coordShader")->use().setMatrix4f("model", model);
}

void GLManager::updateRenderData() {
    if(this->isLineMode)
        glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // 这个是可有可无？
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);  // 例如：清除为黑色

    projection.setToIdentity();
    projection.perspective(m_camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.f);
    view = m_camera->getViewMatrix();

    ResourceManager::updateProjViewMatrixInShader(projection, view);
    ResourceManager::updateViewPosInShader(m_camera->position);
    // TODO：灯光管理太烂了。等后面来优化。光没准可以定义成全局变量
    ResourceManager::updateDirectLightInShader(directLight);

    QMatrix4x4 tempM;
    tempM.setToIdentity();
    ResourceManager::getShader("coordShader")->use().setMatrix4f("model", tempM);
    // 为管理的objects设置model
    for(auto & i : objectMap) {
        auto& tempShader = ResourceManager::getShader(i.second->getShaderName())->use();
        tempShader.setMatrix4f("model", i.second->getTransform());
        tempShader.release();
    }
}

/********* Object Manager Functions *********/
void GLManager::drawObjects() {
    for(auto & i : objectMap) {
        i.second->draw();
    }
}

// TODO: 注意这个clear，由于用的是shared_ptr，没准会导致这里删除到其他地方还在用？
void GLManager::clearObjects() {
    objectMap.clear();
    qDebug() << "Clear ALL Objects";
}

int GLManager::addObject(const QString& mPath) {
    this->makeCurrent();

    if(mPath.isEmpty()) {
        qDebug() << "Please Give Model Type a Model Path!";
        return -1;
    }

    std::shared_ptr<GameObject> tempPtr;
    tempPtr = std::make_shared<GameObject>(mPath);
    GLuint tempID = tempPtr->getObjectID();
    objectMap[tempID] = tempPtr;

    qDebug() << "Add Model Object, Path: " << mPath;
    this->doneCurrent();

    return (int)tempID;
}

int GLManager::addObject(ObjectType objType) {
    this->makeCurrent();
    if(objType == ObjectType::Model) {
        qDebug() << "If you want to add a model, please directly give the model path!";
        return -1;
    }

    std::shared_ptr<GameObject> tempPtr = std::make_shared<GameObject>(objType);
    GLuint tempID = tempPtr->getObjectID();
    objectMap[tempID] = tempPtr;
    qDebug() << "Add Shape Object, Shader: " << objectTypeToString(objType);

    this->doneCurrent();

    return (int)tempID;
}

void GLManager::deleteObject(GLuint id) {
    this->makeCurrent();
    if(objectMap.find(id) == objectMap.end()) {
        qDebug() << "Not Found Object to Delete, ID: " << id;
        return;
    }

    objectMap.erase(id);
    qDebug() << "Delete Object, ID: " << id;
    this->doneCurrent();
}

const std::map<GLuint, std::shared_ptr<GameObject>>& GLManager::getAllGameObjectMap() const {
    return objectMap;
}

std::shared_ptr<GameObject> GLManager::getTargetGameObject(GLuint id) {
    if(objectMap.find(id) == objectMap.end()) {
        qDebug() << "Not Found Object to Get, ID: " << id;
        return nullptr;
    }

    return objectMap[id];
}

void GLManager::checkGLVersion() {
    QOpenGLContext* context = QOpenGLContext::currentContext();
    if (context) {
        QSurfaceFormat format = context->format();
        int majorVersion = format.majorVersion();
        int minorVersion = format.minorVersion();
        qDebug() << "OpenGL Version:" << majorVersion << "." << minorVersion;
    } else {
        qDebug() << "OpenGL Context Empty.";
    }
}

void GLManager::initConfigureVariables() {
    isLineMode = GL_FALSE;
    backGroundColor = QVector3D(0.8f, 0.84f, 0.8f);

    defaultCameraMoveSpeed = 0.2f;
    shiftDown = GL_FALSE;
    isFirstMouse = GL_TRUE;
    isRightMousePress = GL_FALSE;

    deltaTime = 0.0f;
    lastFrame = 0.0f;
    lastX = (int)((float)width() / 2.0f);
    lastY = (int)((float)height() / 2.0f);

    for (auto& key : keys) {
        key = GL_FALSE;
    }
}

void GLManager::initLightInfo() {
    directLight = DirectLight();

}

void GLManager::initOpenGLSettings() {
    checkGLVersion();
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if (!glFunc) {
        qFatal("Requires OpenGL >= 4.3");
    }
    glFunc->initializeOpenGLFunctions();

    glFunc->glEnable(GL_DEPTH_TEST);
    glFunc->glEnable(GL_LINE_SMOOTH);

    GLfloat lineWidthRange[2];
    glFunc->glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    qDebug() << "Support Line Width Range: " << lineWidthRange[0] << "~" << lineWidthRange[1];
    glFunc->glLineWidth(10.0f);

    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    qDebug() << "======= Done Init OpenGL Settings ========";
}

/********* Event Functions *********/
void GLManager::keyPressEvent(QKeyEvent *event) {
    if(isFirstMouse)
        return;

    GLuint key = event->key();
    if(key < 1024)
        this->keys[key] = GL_TRUE;
    else if(key == Qt::Key_Shift)
        shiftDown = GL_TRUE;
}

void GLManager::keyReleaseEvent(QKeyEvent *event) {
    GLuint key = event->key();
    if(key < 1024)
        this->keys[key] = GL_FALSE;
    else if(key == Qt::Key_Shift)
        shiftDown = GL_FALSE;
}

void GLManager::mouseMoveEvent(QMouseEvent *event) {
    if(!isRightMousePress)
        return;

    GLint xPos = event->pos().x();
    GLint yPos = event->pos().y();
    if (isFirstMouse){
        qDebug() << "First Mouse Click : " << xPos << ", " << yPos;
        lastX = xPos;
        lastY = yPos;
        isFirstMouse = GL_FALSE;
    }

    GLint xOffset = xPos - lastX;
    GLint yOffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
    lastX = xPos;
    lastY = yPos;
    m_camera->handleMouseMovement((GLfloat)xOffset, (GLfloat)yOffset);
}

void GLManager::wheelEvent(QWheelEvent *event) {
    QPoint offset = event->angleDelta();
    m_camera->handleMouseScroll((float)offset.y() / 20.0f);
}

void GLManager::mousePressEvent(QMouseEvent *event) {
    setFocus();
    if(event->button() == Qt::RightButton) {
        m_camera->movementSpeed *= 3.0f;
        isRightMousePress = GL_TRUE;;
    }
}

void GLManager::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() == Qt::RightButton){
        isRightMousePress = GL_FALSE;
        isFirstMouse = GL_TRUE;
    }
}

void GLManager::handleInput(GLfloat dt) {
    if (keys[Qt::Key_W])
        m_camera->handleKeyboard(CameraMove::FORWARD, dt);
    if (keys[Qt::Key_S])
        m_camera->handleKeyboard(CameraMove::BACKWARD, dt);
    if (keys[Qt::Key_A])
        m_camera->handleKeyboard(CameraMove::LEFT, dt);
    if (keys[Qt::Key_D])
        m_camera->handleKeyboard(CameraMove::RIGHT, dt);
    if (keys[Qt::Key_E])
        m_camera->handleKeyboard(CameraMove::UP, dt);
    if (keys[Qt::Key_Q])
        m_camera->handleKeyboard(CameraMove::DOWN, dt);

    if (shiftDown)
        m_camera->movementSpeed = 2.5f * defaultCameraMoveSpeed;
    else
        m_camera->movementSpeed = defaultCameraMoveSpeed;
}

