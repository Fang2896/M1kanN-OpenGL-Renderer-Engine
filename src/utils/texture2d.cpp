//
// Created by fangl on 2023/9/22.
//

#include "utils/texture2d.hpp"

Texture2D::Texture2D()
    : texture(nullptr), id(0), type(TextureType::UNKNOWN), transparent(GL_FALSE),
      path(""),
      internal_format(QOpenGLTexture::RGBAFormat),
      wrap_s(QOpenGLTexture::Repeat), wrap_t(QOpenGLTexture::Repeat),
      filter_min(QOpenGLTexture::Linear), filter_max(QOpenGLTexture::Linear)
{ }

Texture2D::~Texture2D() = default;

void Texture2D::generate(const QString &file) {
    texture = std::make_shared<QOpenGLTexture>(QOpenGLTexture::Target2D);
    texture->setFormat(internal_format);

    // 透明的话，用clampToEdge
    QImage image = QImage(file);
    GLboolean isTransparency = checkTransparency(image);
    if(isTransparency) {
        wrap_s = QOpenGLTexture::ClampToEdge;
        wrap_t = QOpenGLTexture::ClampToEdge;
        transparent = GL_TRUE;
    }

    texture->setData(image, QOpenGLTexture::GenerateMipMaps);

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);

    texture->setMinificationFilter(filter_min);
    texture->setMagnificationFilter(filter_max);

    this->id = texture->textureId();
}

void Texture2D::bind() const {
    texture->bind();
}

GLuint Texture2D::getTextureID() {
    if(!texture)
        qFatal("Texture is NULL");

    return texture->textureId();
}

void Texture2D::setTextureFormat(QOpenGLTexture::TextureFormat format) {
    internal_format = format;
    texture->setFormat(internal_format);
}

void Texture2D::setWrapMode(QOpenGLTexture::WrapMode s, QOpenGLTexture::WrapMode t) {
    wrap_s = s;
    wrap_t = t;

    texture->setWrapMode(QOpenGLTexture::DirectionS, wrap_s);
    texture->setWrapMode(QOpenGLTexture::DirectionT, wrap_t);
}

void Texture2D::setFilter(QOpenGLTexture::Filter min, QOpenGLTexture::Filter max) {
    filter_min = min;
    filter_max = max;

    texture->setMinificationFilter(filter_min);
    texture->setMagnificationFilter(filter_max);
}

GLboolean Texture2D::checkTransparency(const QImage& image) {
    if (image.hasAlphaChannel()) {
        int width = image.width();
        int height = image.height();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                QColor pixelColor = image.pixelColor(x, y);

                if (pixelColor.alpha() < 255) {
                    return true; // 图像包含至少一个半透明或透明像素
                }
            }
        }
    }

    return false; // 图像没有透明度或所有像素都是不透明的
}