//
// Created by fangl on 2023/9/22.
//

#ifndef SHADER_HPP
#define SHADER_HPP

#include <QOpenGLShader>
#include <QOpenGLShaderProgram>


class Shader
{
    friend class ResourceManager;
   public:
    Shader() = default;
    ~Shader() = default;

    bool compile(const QString& vertexSource, const QString& fragmentSource, const QString& geometrySource = nullptr);

    Shader& use(){
        shaderProgram->bind();
        return *this;
    }

    void release() {
        shaderProgram->release();
    }

    void bind() {
        shaderProgram->bind();
    }

    void setFloat(const QString& name, const GLfloat& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setInteger(const QString& name, const GLint& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector2f(const QString& name, const GLfloat& x, const GLfloat& y) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector2D(x, y));
    }

    void setVector2f(const QString& name, const QVector2D& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector3f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector3D(x, y, z));
    }

    void setVector3f(const QString& name, const QVector3D& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setVector4f(const QString& name, const GLfloat& x, const GLfloat& y, const GLfloat& z, const GLfloat& w) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, QVector4D(x, y, z, w));
    }

    void setVector4f(const QString& name, const QVector4D& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setMatrix4f(const QString& name, const QMatrix4x4& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

    void setBool(const QString& name, const GLboolean& value) const {
        GLint loc = shaderProgram->uniformLocation(name);
        shaderProgram->setUniformValue(loc, value);
    }

   private:
    std::shared_ptr<QOpenGLShaderProgram> shaderProgram;
};

#endif  //SHADER_HPP
