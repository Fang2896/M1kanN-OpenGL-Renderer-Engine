#include <QFile>
#include <QApplication>

#include "ui/mainwindow.hpp"

void setGLVersion(int major, int minor) {
    QSurfaceFormat format;
    format.setVersion(major, minor);
    format.setProfile(QSurfaceFormat::CoreProfile); // 使用核心配置文件
    QSurfaceFormat::setDefaultFormat(format);
}

void setStyle(const QString& styleName) {
    QFile file(":qss/assets/qss/" + styleName + ".css");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    setStyle("flatwhite");
    setGLVersion(4, 3); // Mac: 4.1, Win: 4.3 (with compute shader)

    MainWindow w;
    w.show();
    return QApplication::exec();
}

