//
// Created by fangl on 2023/9/19.
//

#include "gl_manager.hpp"

const QVector3D CAMERA_POSITION(0.0f, 0.5f, 3.0f);


GLManager::GLManager(QWidget* parent, int width, int height)
    : QOpenGLWidget(parent), coordData(ShapeData::getCoordinateVertices()) {
    this->setGeometry(10, 20, width, height);
}

GLManager::~GLManager() = default;

/********* OpenGL Functions *********/
void GLManager::initializeGL() {
    initConfigureVariables();
    initOpenGLSettings();

    // member mangers
    m_camera = std::make_unique<Camera>(CAMERA_POSITION, defaultCameraMoveSpeed);
    m_testModel = std::make_unique<Model>("../assets/models/nanosuit/nanosuit.obj");

    // object manager, resource manager...
    initObjects();
    initResources();

    // start timer
    eTimer.start();
}

void GLManager::resizeGL(int w, int h) {
    glFunc->glViewport(0, 0, w, h);
}

void GLManager::paintGL() {
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);  // 例如：清除为黑色

    // time and position data
    GLfloat currentFrame = (GLfloat)eTimer.elapsed() / 100;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    
    this->handleInput(deltaTime);
    this->updateRenderData();

    ResourceManager::getShader("coordinate").use();
    drawCoordinate();
    ResourceManager::getShader("baseModel").use();
    m_testModel->draw(ResourceManager::getShader("baseModel").use());
}

/********* Other Functions *********/
void GLManager::initObjects() {
    glFunc->glGenBuffers(1, &coordVBO);
    glFunc->glBindBuffer(GL_ARRAY_BUFFER, coordVBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, coordData.size() * sizeof(float), coordData.data(), GL_STATIC_DRAW);

    glFunc->glGenVertexArrays(1, &coordVAO);
    glFunc->glBindVertexArray(coordVAO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, coordVBO);

    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (GLsizei)3 * sizeof(float), (void*)nullptr);
    glFunc->glBindVertexArray(0);

    qDebug() << "Initializing Objects... ";
}

void GLManager::initResources() {
    ResourceManager::loadShader("coordinate",
                                ":/shaders/assets/shaders/coordinate.vert",
                                ":/shaders/assets/shaders/coordinate.frag");
    ResourceManager::loadShader("baseModel",
                                ":/shaders/assets/shaders/baseModel.vert",
                                ":/shaders/assets/shaders/baseModel.frag");

    // matrix configuration
    QMatrix4x4 model;
    model.setToIdentity();
    model.scale(5.0f);
    ResourceManager::getShader("coordinate").use().setMatrix4f("model", model);
    model.setToIdentity();
    model.scale(0.1f);
    ResourceManager::getShader("baseModel").use().setMatrix4f("model", model);

    qDebug() << "Initializing Resources... ";
}

void GLManager::drawCoordinate() {
    glFunc->glBindVertexArray(coordVAO);
    glFunc->glDrawArrays(GL_LINES, 0, (GLsizei)coordData.size() / 3);
    glFunc->glBindVertexArray(0);
}

void GLManager::updateRenderData() {
    if(this->isLineMode)
        glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glFunc->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    projection.setToIdentity();
    projection.perspective(m_camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.f);
    view = m_camera->getViewMatrix();

    ResourceManager::getShader("coordinate").use().setMatrix4f("projection", projection);
    ResourceManager::getShader("coordinate").use().setMatrix4f("view", view);
    ResourceManager::getShader("baseModel").use().setMatrix4f("projection", projection);
    ResourceManager::getShader("baseModel").use().setMatrix4f("view", view);
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
    backGroundColor = QVector3D(0.35f, 0.35f, 0.35f);

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

void GLManager::initOpenGLSettings() {
    checkGLVersion();
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if (!glFunc) {
        qFatal("Requires OpenGL >= 4.3");
    }
    glFunc->initializeOpenGLFunctions();
    glFunc->glEnable(GL_DEPTH_TEST);
    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

