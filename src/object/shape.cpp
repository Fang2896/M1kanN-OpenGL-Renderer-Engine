//
// Created by fangl on 2023/9/22.
//



#include "object/shape.hpp"


Shape::Shape(QString  geometryType)
    : Object("shape", "defaultShape", "defaultShapeShader"),
      geometryType(std::move(geometryType)), material(ShapeMaterial())
{
    glFunc = QOpenGLContext::currentContext()->versionFunctions<GLFunctions_Core>();
    if(!glFunc)
        qFatal("Require GLFunctions_Core to setUp mesh");
}

Shape::~Shape() = default;

void Shape::init(QString objName) {
    setObjectName(objName);

    if(geometryType.toLower() == "cube") {
        initShapeData(ShapeData::getCubeVertices(), ShapeData::getCubeIndices());
    } else if (geometryType.toLower() == "plane") {
        initShapeData(ShapeData::getPlaneVertices(), ShapeData::getPlaneIndices());
    }

    qDebug() << "======= Done Init Test Shape ========";
}

void Shape::init() {
    setObjectName("defaultShape");

    if(geometryType.toLower() == "cube") {
        initShapeData(ShapeData::getCubeVertices(), ShapeData::getCubeIndices());
    } else if (geometryType.toLower() == "plane") {
        initShapeData(ShapeData::getPlaneVertices(), ShapeData::getPlaneIndices());
    }

    qDebug() << "======= Done Init Shape: " << getObjectName() << "Shader: "
        << getShaderName() << "========";
}

// 这里有参数的原因是，可能需要在类里面设置shader的一些参数
void Shape::draw() {
    if(VAO == 0) {
        qFatal("Shape's VAO is NULL!");
    }

    // TODO：暂且让所有shape共用shader，然后在draw上改变参数
    updateShapeMaterial();

    glFunc->glBindVertexArray(VAO);
    glFunc->glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glFunc->glBindVertexArray(0);
}

void Shape::updateShapeData(const QVector<float>& posData,
                     const QVector<unsigned int>& indexData) {
    if(VAO == 0 || VBO == 0 || EBO == 0) {
        qFatal("VAO & VBO & EBO empty!");
    }

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                         &vertices[0], GL_STATIC_DRAW);

    glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                         &indices[0], GL_STATIC_DRAW);
}

void Shape::updateShapeMaterial(ShapeMaterial mat) {
    this->material = mat;
    if(getShaderName().isEmpty()) {
        qFatal("Setting Material Shader is Empty!");
    }

    const Shader& tempShader = ResourceManager::getShader(getShaderName());
    tempShader.setFloat("material.shininess" ,mat.shininess);
    tempShader.setVector3f("material.ambientColor" ,mat.ambientColor);
    tempShader.setVector3f("material.diffuseColor" ,mat.diffuseColor);
    tempShader.setVector3f("material.specularColor" ,mat.specularColor);
    tempShader.setFloat("material.ambientOcclusion" ,mat.ambientOcclusion);
}

void Shape::initShapeData(const QVector<float>& posData, const QVector<unsigned int>& indexData) {
    convertDataFloatToVertex(posData);

    this->indices = indexData;

    glFunc->glGenVertexArrays(1, &VAO);
    glFunc->glGenBuffers(1, &VBO);
    glFunc->glGenBuffers(1, &EBO);

    glFunc->glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glFunc->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                         &vertices[0], GL_STATIC_DRAW);

    glFunc->glBindVertexArray(VAO);
    glFunc->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glFunc->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                         &indices[0], GL_STATIC_DRAW);


    // vertex position
    glFunc->glEnableVertexAttribArray(0);
    glFunc->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), (void*)0);
    // vertex normal
    glFunc->glEnableVertexAttribArray(1);
    glFunc->glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,
                                  sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex uv
    glFunc->glEnableVertexAttribArray(2);
    glFunc->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                                  sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    glFunc->glBindVertexArray(0);
}

void Shape::convertDataFloatToVertex(const QVector<float>& data) {
    vertices.clear();
    int stride = 8;

    for (int i = 0; i < data.size(); i += stride) {
        Vertex v;

        v.position = QVector3D(data[i], data[i + 1], data[i + 2]);
        v.normal = QVector3D(data[i + 3], data[i + 4], data[i + 5]);
        v.texCoord = QVector2D(data[i + 6], data[i + 7]);

        vertices.push_back(v);
    }
}
