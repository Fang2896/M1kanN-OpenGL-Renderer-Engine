//
// Created by fangl on 2023/9/22.
//

#ifndef TEXTURE_2_D_HPP
#define TEXTURE_2_D_HPP

#include <memory>
#include <QOpenGLTexture>


class Texture2D
{
    friend class ResourceManager;
   public:
    Texture2D();
    ~Texture2D();
    void generate(const QString& file);
    void bind() const;
    GLuint getTextureID();

    GLuint id;
    QString type;
    QString path;

    QOpenGLTexture::TextureFormat internal_format;//Format of texture object
    QOpenGLTexture::WrapMode wrap_s;
    QOpenGLTexture::WrapMode wrap_t;
    QOpenGLTexture::Filter filter_min;
    QOpenGLTexture::Filter filter_max;

   private:
    std::shared_ptr<QOpenGLTexture> texture;
};

#endif  //TEXTURE_2_D_HPP