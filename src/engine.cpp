#include <QFile>
#include <QApplication>

#include "include/mainwindow.hpp"


void setGLVersion(int major, int minor) {
    QSurfaceFormat format;
    format.setVersion(major, minor); // 设置为 OpenGL 4.5
    format.setProfile(QSurfaceFormat::CoreProfile); // 使用核心配置文件
    QSurfaceFormat::setDefaultFormat(format);
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // set styles
//    QFile file(":qss/assets/qss/flatwhite.css");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//    qApp->setStyleSheet(styleSheet);

    setGLVersion(4, 3);
    MainWindow w;
    w.show();
    return QApplication::exec();
}

