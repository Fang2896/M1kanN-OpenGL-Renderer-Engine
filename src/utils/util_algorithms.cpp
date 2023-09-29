//
// Created by fangl on 2023/9/30.
//

#include "utils/util_algorithms.hpp"


QString UtilAlgorithms::getFileNameFromPath(const QString& path) {
    QFileInfo fileInfo(path);
    QString baseName = fileInfo.baseName(); // 获取不带扩展名的文件名

    return baseName;
}


