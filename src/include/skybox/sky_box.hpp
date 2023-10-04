//
// Created by fangl on 2023/10/4.
//

#ifndef SKY_BOX_HPP
#define SKY_BOX_HPP

#include <QMap>
#include <QDebug>
#include <QOpenGLTexture>

#include "gl_configure.hpp"


class SkyBox {
   public:
    SkyBox();
    ~SkyBox();

    void setPosXPath(const QString& path);
    void setPosYPath(const QString& path);
    void setPosZPath(const QString& path);
    void setNegXPath(const QString& path);
    void setNegYPath(const QString& path);
    void setNegZPath(const QString& path);
    void setCubePath(const QString& posX, const QString& posY, const QString& posZ,
                     const QString& negX, const QString& negY, const QString& negZ);

    void init();
    void draw();

    int getTextureID();
   private:
    GLFunctions_Core *glFunc;
    QOpenGLTexture *texture;

    GLuint VAO;
    GLuint VBO;

    QMap<QString, QString> paths;
};

#endif  //SKY_BOX_HPP
