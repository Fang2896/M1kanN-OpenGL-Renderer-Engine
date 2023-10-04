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
    initFrameBufferSettings();
    initSkyBoxSettings();   // must init before initShader
    initShaders();    // shader

    // TODO: 这里可以从coordinate改成各种绘制精灵？
    initShaderValue();

    // member mangers
    // object manager, resource manager...
    m_camera = std::make_unique<Camera>(CAMERA_POSITION, defaultCameraMoveSpeed);
    coordinate = std::make_unique<Coordinate>();
    coordinate->initCoordinate();

    // start timer
    eTimer.start();
}

void GLManager::resizeGL(int w, int h) {
    glFunc->glViewport(0, 0, w, h);

    // postProcessing的texture也需要重新生成
    fbo->release();
    delete fbo;
    fbo = nullptr;

    fbo = new QOpenGLFramebufferObject(QSize(w, h),
                                       QOpenGLFramebufferObject::CombinedDepthStencil,
                                       GL_TEXTURE_2D, GL_RGB);
}

void GLManager::paintGL() {
    // time and position data
    GLfloat currentFrame = (GLfloat)eTimer.elapsed() / 100;
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->handleInput(deltaTime);
    this->updateRenderData();

    if(postProcessingType == PostProcessingType::NORMAL) {
        drawObjects();
    } else {
        drawObjectsWithPostProcessing();
    }
}

// for coordinate and stencil testing
void GLManager::initShaders() {
    ResourceManager::loadShader("defaultShader",
                                ":/shaders/assets/shaders/defaultShader.vert",
                                ":/shaders/assets/shaders/defaultShader.frag");

    // coordinate
    ResourceManager::loadShader("coordShader",
                                ":/shaders/assets/shaders/baseShader.vert",
                                ":/shaders/assets/shaders/coordShader.frag");

    // outline
    ResourceManager::loadShader("outlineShader",
                                ":/shaders/assets/shaders/baseShader.vert",
                                ":/shaders/assets/shaders/outlineShader.frag");

    // post processing
    ResourceManager::loadShader("postProcessingShader",
                                ":/shaders/assets/shaders/post_processing/postProcessing.vert",
                                ":/shaders/assets/shaders/post_processing/postProcessing.frag");
    ResourceManager::getShader("postProcessingShader")->use().setInteger("screenTexture", 0);

    // reflection & refraction
//    ResourceManager::loadShader("reflectionShader",
//                                ":/shaders/assets/shaders/defaultShader.vert",
//                                ":/shaders/assets/shaders/reflectionShader.frag");
//    ResourceManager::loadShader("refractionShader",
//                                ":/shaders/assets/shaders/defaultShader.vert",
//                                ":/shaders/assets/shaders/refractionShader.frag");
//    ResourceManager::getShader("reflectionShader")->use().setInteger("skybox", 0);
//    ResourceManager::getShader("refractionShader")->use().setInteger("skybox", 0);

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

    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    projection.setToIdentity();
    projection.perspective(m_camera->zoom, (GLfloat)width() / (GLfloat)height(), 0.1f, 200.f);
    view = m_camera->getViewMatrix();

    ResourceManager::updateProjViewViewPosMatrixInShader(projection, view, m_camera->position);
    ResourceManager::updateRenderConfigure(depthMode);

    // TODO：灯光管理太烂了。等后面来优化。光没准可以定义成全局变量
    ResourceManager::updateDirectLightInShader(isLighting, directLight);

    // coordinate
    QMatrix4x4 tempM;
    tempM.setToIdentity();
    ResourceManager::getShader("coordShader")->use().setMatrix4f("model", tempM);

    // skybox (这个要单独设置)
    QMatrix4x4 skyboxView;
    skyboxView.setRow(0, QVector4D(view(0, 0), view(0, 1), view(0, 2), 0.0f));
    skyboxView.setRow(1, QVector4D(view(1, 0), view(1, 1), view(1, 2), 0.0f));
    skyboxView.setRow(2, QVector4D(view(2, 0), view(2, 1), view(2, 2), 0.0f));
    skyboxView.setRow(3, QVector4D(0.0f,0.0f, .0f, .0f)); //这个去掉位移的4x4矩阵，使天空盒vertices的尺寸的改变，不再影响渲染效果
    ResourceManager::getShader("skybox")->use().setMatrix4f("view", skyboxView);
    ResourceManager::getShader("skybox")->use().setMatrix4f("projection", projection);

    // 为管理的objects设置model
    for(auto & i : objectMap) {
        auto& tempShader = ResourceManager::getShader(i.second->getShaderName())->use();
        tempShader.setMatrix4f("model", i.second->getTransform());

        // 为了cullMode change的一个bug
        ResourceManager::updateMaterialInShader(i.second->getShaderName(), i.second->getMaterial());
        tempShader.release();
    }
}

/********* Object Manager Functions *********/
void GLManager::drawObjects() {
    ResourceManager::getShader("coordShader")->use();
    coordinate->drawCoordinate();
    ResourceManager::getShader("coordShader")->release();

    // 先绘制不透明物体
    if(enableSkybox == GL_TRUE) {
        glFunc->glDepthFunc(GL_LEQUAL);
        ResourceManager::getShader("skybox")->use();
        skybox->draw();
        glFunc->glDepthFunc(GL_LESS);
    }

    for(auto & i : objectMap) {
        if(!i.second->containTransparencyTexture)
            i.second->draw();
    }

    std::vector<std::pair<float, unsigned int>> distVec;
    distVec.clear();
    for(auto &i : objectMap) {
        if(i.second->containTransparencyTexture) {
            float dist = m_camera->position.distanceToPoint(i.second->getPosition());
            distVec.emplace_back(dist, i.first);
        }
    }

    // 从远到近排序
    std::sort(distVec.begin(), distVec.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.first > rhs.first;
              });

    // 从远到近绘制透明物体
    for(auto & it : distVec) {
        objectMap[it.second]->draw();
    }
}

void GLManager::drawObjectsWithPostProcessing() {
    // 1st pass
    fbo->bind();
    glFunc->glEnable(GL_DEPTH_TEST);
    glFunc->glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawObjects();

    fbo->release();

    // 2nd pass
    glFunc->glDisable(GL_DEPTH_TEST);
    glFunc->glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    glFunc->glClear(GL_COLOR_BUFFER_BIT);

    const Shader &tempShader = ResourceManager::getShader("postProcessingShader")->use();
    switch (postProcessingType) {
        case PostProcessingType::NORMAL:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::NORMAL);
            break;
        case PostProcessingType::INVERSION :
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::INVERSION);
            break;
        case PostProcessingType::GRAY:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::GRAY);
            break;
        case PostProcessingType::SHARPEN:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::SHARPEN);
            break;
        case PostProcessingType::BLUR:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::BLUR);
            break;
        case PostProcessingType::EDGE:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::EDGE);
            break;
        case PostProcessingType::SCAN:
            tempShader.setInteger("postProcessingType", (int)PostProcessingType::SCAN);
            break;
    }

    glFunc->glBindTexture(GL_TEXTURE_2D, fbo->texture()); //绑定fbo缓冲所生成的纹理ID
    postProcessingScreen->draw();
    glFunc->glBindTexture(GL_TEXTURE_2D, 0);
}

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

int GLManager::addObject(ObjectType objType, float width, float height) {
    this->makeCurrent();
    if(objType == ObjectType::Model) {
        qDebug() << "If you want to add a model, please directly give the model path!";
        return -1;
    }

    std::shared_ptr<GameObject> tempPtr = std::make_shared<GameObject>(objType, width, height);
    GLuint tempID = tempPtr->getObjectID();

    tempPtr->displayName = objectTypeToString(objType) + " - " + QString::number(tempID);
    objectMap[tempID] = tempPtr;

    this->doneCurrent();
    return (int)tempID;
}

void GLManager::deleteObject(GLuint id) {
    this->makeCurrent();
    if(objectMap.find(id) == objectMap.end()) {
        qDebug() << "Not Found Object to Delete, ID: " << id;
        return;
    }

    auto tempObj = objectMap[id];
    ResourceManager::map_Shaders.erase("shader_" + QString::number(id));
    qDebug() << "Delete Object, ID: " << id << ", Name: " << tempObj->displayName;
    objectMap.erase(id);
    tempObj = nullptr;

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

void GLManager::setEnableLighting(GLboolean enableLighting) {
    isLighting = enableLighting;
}

void GLManager::setLineMode(GLboolean enableLineMode) {
    this->isLineMode = enableLineMode;
}

void GLManager::setDepthMode(GLboolean depMode) {
    this->depthMode = depMode;
}

void GLManager::setCullMode(CullModeType type) {
    makeCurrent();
    if(type == CullModeType::Disable) {
        glFunc->glDisable(GL_CULL_FACE);
    } else if (type == CullModeType::Front) {
        glFunc->glEnable(GL_CULL_FACE);
        glFunc->glCullFace(GL_FRONT);
    } else if (type == CullModeType::Back) {
        glFunc->glEnable(GL_CULL_FACE);
        glFunc->glCullFace(GL_BACK);
    } else if (type == CullModeType::Front_Back) {
        glFunc->glEnable(GL_CULL_FACE);
        glFunc->glCullFace(GL_FRONT_AND_BACK);
    }
    doneCurrent();
}

void GLManager::setPostProcessingType(PostProcessingType type) {
    this->postProcessingType = type;
}

void GLManager::setSkyboxPath(SkyboxType type) {
    if(type == SkyboxType::Disable) {
        enableSkybox = GL_FALSE;
    } else if(type == SkyboxType::Mountain){    //山水
        enableSkybox = GL_TRUE;
        skybox->setCubePath(":/textures/assets/textures/skybox/1/right.jpg",
                            ":/textures/assets/textures/skybox/1/top.jpg",
                            ":/textures/assets/textures/skybox/1/front.jpg",
                            ":/textures/assets/textures/skybox/1/left.jpg",
                            ":/textures/assets/textures/skybox/1/bottom.jpg",
                            ":/textures/assets/textures/skybox/1/back.jpg");
    }else if(type == SkyboxType::Earth){
        enableSkybox = GL_TRUE;
        skybox->setCubePath(":/textures/assets/textures/skybox/2/right.jpg",
                            ":/textures/assets/textures/skybox/2/top.jpg",
                            ":/textures/assets/textures/skybox/2/front.jpg",
                            ":/textures/assets/textures/skybox/2/left.jpg",
                            ":/textures/assets/textures/skybox/2/bottom.jpg",
                            ":/textures/assets/textures/skybox/2/back.jpg");
    }else if(type == SkyboxType::Sunset){
        enableSkybox = GL_TRUE;
        skybox->setCubePath(":/textures/assets/textures/skybox/3/right.jpg",
                            ":/textures/assets/textures/skybox/3/top.jpg",
                            ":/textures/assets/textures/skybox/3/front.jpg",
                            ":/textures/assets/textures/skybox/3/left.jpg",
                            ":/textures/assets/textures/skybox/3/bottom.jpg",
                            ":/textures/assets/textures/skybox/3/back.jpg");
    }
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
    isLighting = GL_TRUE;
    depthMode = GL_FALSE;
    cullType = CullModeType::Disable;
    backGroundColor = QVector3D(0.6f, 0.6f, 0.6f);

    // post processing
    postProcessingType = PostProcessingType::NORMAL;

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

    glFunc->glEnable(GL_BLEND); // 半透明
    glFunc->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glFunc->glEnable(GL_STENCIL_TEST);
    glFunc->glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glFunc->glClearStencil(0);

    glFunc->glDisable(GL_CULL_FACE);

    GLfloat lineWidthRange[2];
    glFunc->glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    qDebug() << "Support Line Width Range: " << lineWidthRange[0] << "~" << lineWidthRange[1];

    glFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glFunc->glClearColor(backGroundColor.x(),
                         backGroundColor.y(),
                         backGroundColor.z(), 1.0f);

    qDebug() << "======= Done Init OpenGL Settings ========";
}

void GLManager::initFrameBufferSettings() {
    fbo = new QOpenGLFramebufferObject(QSize(width(), height()),
                                       QOpenGLFramebufferObject::CombinedDepthStencil, GL_TEXTURE_2D, GL_RGB);

    postProcessingScreen = std::make_shared<PostProcessScreen>();
    postProcessingScreen->init();

    maxNumOfTextureUnits = 0;
    glFunc->glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxNumOfTextureUnits);
    qDebug() << "Max Texture Unit Support Number : " << maxNumOfTextureUnits;
    qDebug() << "======= Done Init Frame Buffer Settings ========";
}

void GLManager::initSkyBoxSettings() {
    enableSkybox = GL_FALSE;

    skybox = std::make_shared<SkyBox>();

    skybox->setPosXPath(":/textures/assets/textures/skybox/1/right.jpg");
    skybox->setPosYPath(":/textures/assets/textures/skybox/1/top.jpg");
    skybox->setPosZPath(":/textures/assets/textures/skybox/1/front.jpg");
    skybox->setNegXPath(":/textures/assets/textures/skybox/1/left.jpg");
    skybox->setNegYPath(":/textures/assets/textures/skybox/1/bottom.jpg");
    skybox->setNegZPath(":/textures/assets/textures/skybox/1/back.jpg");
    skybox->init();

    ResourceManager::loadShader("skybox", ":/shaders/assets/shaders/skybox/skybox.vert",
                                ":/shaders/assets/shaders/skybox/skybox.frag");
    ResourceManager::getShader("skybox")->use().setInteger("skybox", 31);
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

