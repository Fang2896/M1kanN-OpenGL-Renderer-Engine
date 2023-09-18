#include <QFile>
#include <QApplication>

#include "include/mainwindow.hpp"


int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

//    QFile file(":qss/assets/qss/flatwhite.css");
//    file.open(QFile::ReadOnly);
//    QString styleSheet = QLatin1String(file.readAll());
//    qApp->setStyleSheet(styleSheet);

    MainWindow w;
    w.show();
    return QApplication::exec();
}
