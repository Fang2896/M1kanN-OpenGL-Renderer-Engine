//
// Created by fangl on 2023/9/22.
//

#ifndef TEXTURE_2_D_HPP
#define TEXTURE_2_D_HPP

#include <memory>
#include <QOpenGLTexture>

#include "m_type.hpp"


class Texture2D
{
    friend class ResourceManager;
   public:
    Texture2D();
    ~Texture2D();
    void generate(const QString& file);
    void bind() const;
    GLuint getTextureID();

    void setTextureFormat(QOpenGLTexture::TextureFormat format);
    void setWrapMode(QOpenGLTexture::WrapMode s, QOpenGLTexture::WrapMode t);
    void setFilter(QOpenGLTexture::Filter min, QOpenGLTexture::Filter max);

    // 方便期间，直接裸露
    GLuint id;
    TextureType type;
    GLboolean transparent;
    QString path;

   private:
    QOpenGLTexture::TextureFormat internal_format;//Format of texture object
    QOpenGLTexture::WrapMode wrap_s;
    QOpenGLTexture::WrapMode wrap_t;
    QOpenGLTexture::Filter filter_min;
    QOpenGLTexture::Filter filter_max;

    std::shared_ptr<QOpenGLTexture> texture;

    static GLboolean checkTransparency(const QImage& image);
};


#endif  //TEXTURE_2_D_HPP
