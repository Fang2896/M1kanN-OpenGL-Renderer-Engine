//
// Created by fangl on 2023/9/23.
//

#ifndef GL_CONFIGURE_HPP
#define GL_CONFIGURE_HPP

#include <qglobal.h>
#if defined(Q_OS_MAC)
#include <QOpenGLFunctions_4_1_Core>  // Mac-specific version
using GLFunctions_Core = QOpenGLFunctions_4_1_Core;
#elif defined(Q_OS_WIN)
#include <QOpenGLFunctions_4_3_Core>  // Windows-specific version
using GLFunctions_Core = QOpenGLFunctions_4_3_Core;
#endif


#endif  //GL_CONFIGURE_HPP
