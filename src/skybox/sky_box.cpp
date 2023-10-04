//
// Created by fangl on 2023/10/4.
//

#include "skybox/sky_box.hpp"


SkyBox::SkyBox() : VAO(0), VBO(0), texture(nullptr) {
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if (!glFunc) {
        qFatal("Requires OpenGL >= 4.3");
    }
    glFunc->initializeOpenGLFunctions();
}

SkyBox::~SkyBox(){
    if(VAO != 0)
        glFunc->glDeleteVertexArrays(1, &VAO);
    if(VBO != 0)
        glFunc->glDeleteBuffers(1, &VBO);
}

void SkyBox::setPosXPath(const QString &path){
    paths["posX"] = path;
}

void SkyBox::setPosYPath(const QString &path){
    paths["posY"] = path;
}

void SkyBox::setPosZPath(const QString &path){
    paths["posZ"] = path;
}

void SkyBox::setNegXPath(const QString &path){
    paths["negX"] = path;
}

void SkyBox::setNegYPath(const QString &path){
    paths["negY"] = path;
}

void SkyBox::setNegZPath(const QString &path){
    paths["negZ"] = path;
}

void SkyBox::setCubePath(const QString &posX, const QString &posY, const QString &posZ, const QString &negX, const QString &negY, const QString &negZ){
    this->setPosXPath(posX);
    this->setPosYPath(posY);
    this->setPosZPath(posZ);
    this->setNegXPath(negX);
    this->setNegYPath(negY);
    this->setNegZPath(negZ);
    this->init();
}

void SkyBox::init(){
    if(paths.size() < 6)
        qDebug() << "ERROR::SKYBOX::INIT skybox textures' size less than 6";

    QImage posX = QImage(paths["posX"]).convertToFormat(QImage::Format_RGB888); //Right，默认纹理位32位RGB，不符合要求，必须转为24位RGB。
    QImage negX = QImage(paths["negX"]).convertToFormat(QImage::Format_RGB888); //Left
    QImage posY = QImage(paths["posY"]).convertToFormat(QImage::Format_RGB888); //Top
    QImage negY = QImage(paths["negY"]).convertToFormat(QImage::Format_RGB888); //Bottom
    QImage posZ = QImage(paths["posZ"]).convertToFormat(QImage::Format_RGB888); //Front
    QImage negZ = QImage(paths["negZ"]).convertToFormat(QImage::Format_RGB888); //Back

    texture = new QOpenGLTexture(QOpenGLTexture::TargetCubeMap);

    texture->setSize(posX.width(), posX.height(), posX.depth());
    texture->setFormat(QOpenGLTexture::RGBFormat);
    texture->allocateStorage(QOpenGLTexture::RGB, QOpenGLTexture::UInt8); //分配内存

    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posX.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posY.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapPositiveZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)posZ.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeX, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negX.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeY, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negY.bits());
    texture->setData(0, 0, QOpenGLTexture::CubeMapNegativeZ, QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void*)negZ.bits());

    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::DirectionS, QOpenGLTexture::ClampToEdge);
    texture->setWrapMode(QOpenGLTexture::DirectionT, QOpenGLTexture::ClampToEdge);

    float vertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    if(VAO != 0) {
        glFunc->glDeleteVertexArrays(1, &VAO);
    }
    if(VBO != 0) {
        glFunc->glDeleteBuffers(1, &VBO);
    }

    glFunc->glGenBuffers(1, &VBO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glFunc->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glFunc->glGenVertexArrays(1, &VAO);
    glFunc->glBindVertexArray(VAO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  3 * sizeof(float), (void*)nullptr);

    glFunc->glBindVertexArray(0);
}

void SkyBox::draw(){
    glFunc->glBindVertexArray(VAO);

    glFunc->glActiveTexture(GL_TEXTURE0 + 31);  // 31作为默认的天空盒纹理单元
    texture->bind();

    glFunc->glDrawArrays(GL_TRIANGLES, 0, 36);

    glFunc->glBindVertexArray(0);
    //texture->release();
}

int SkyBox::getTextureID() {
    if(texture != nullptr)
        return (int)texture->textureId();
    return -1;
}
