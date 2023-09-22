//
// Created by fangl on 2023/9/22.
//

#include "resource_manager.hpp"


// Global variables to store Shaders and Textures
std::map<QString, Shader> ResourceManager::map_Shaders;
std::map<QString, Texture2D> ResourceManager::map_Textures;

Shader ResourceManager::loadShader(const QString& name,
                                          const QString& vShaderFile,
                                          const QString& fShaderFile,
                                          const QString& gShaderfile) {
    Shader Shader{};
    Shader.compile(vShaderFile,
                          fShaderFile,
                          gShaderfile == nullptr ? nullptr : gShaderfile);
    map_Shaders[name] = Shader;
    return map_Shaders[name];
}

Shader ResourceManager::getShader(const QString& name){
    return map_Shaders[name];
}

Texture2D ResourceManager::loadTexture(const QString& name, const QString& file, GLboolean alpha){
    Texture2D texture;

    if(alpha){
        texture.internal_format = QOpenGLTexture::RGBAFormat;
        texture.wrap_s = QOpenGLTexture::ClampToBorder;
        texture.wrap_t = QOpenGLTexture::ClampToBorder;
    }

    texture.generate(file);
    map_Textures[name] = texture;
    return texture;
}

Texture2D ResourceManager::getTexture(const QString& name){
    return map_Textures[name];
}

void ResourceManager::clear(){
    map_Shaders.clear();
    map_Textures.clear();
}

void ResourceManager::clearTextures() {
    map_Shaders.clear();
    map_Textures.clear();
}

