//
// Created by fangl on 2023/9/22.
//

#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <map>
#include <QString>

#include "shader.hpp"
#include "texture2d.hpp"


class ResourceManager
{
   public:
    static std::map<QString, Shader> map_Shaders;
    static std::map<QString, Texture2D> map_Textures;

    static Shader loadShader(const QString& name, const QString& vShaderFile, const QString& fShaderFile, const QString& gShaderfile = NULL);
    static Shader getShader(const QString&  name);
    static Texture2D loadTexture(const QString&  name, const QString& file, GLboolean alpha = false);
    static Texture2D getTexture(const QString&  name);
    static void clear();
    static void clearTextures();

   private:
    ResourceManager() {}
};


#endif  //RESOURCE_MANAGER_HPP
