//
// Created by fangl on 2023/9/18.
//

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QWidget>
#include <QLine>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "gl_manager.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT

   public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

   protected:   // some override functions?

   private: // functions
    void initWidget();
    void initLayout();
    void connectConfigure();

   private slots: // slots
    void updateGLManager();
    void handleEditingFinished();

   private: // filters
    bool eventFilter(QObject *watched, QEvent *event) override;

   private: // variables
    Ui::MainWindow* ui;
    GLManager *glManager;
    QTimer *timer;

    // widgets
    QGroupBox *sceneGroupBox;
    QLabel *lightTitleLabel;
    QLabel *objectTitleLabel;
    QToolButton *lightAddButton;
    QToolButton *objectAddButton;
    QListWidget *lightList;
    QListWidget *objectList;

    QGroupBox *inspectorGroupBox;
    QCheckBox *nameCheckBox;
    QLineEdit *nameLineEdit;

    QGroupBox *transformGroupBox;

    QTabWidget *configureDashTab;

    // 操作时增加：(需要new)
    QLabel *posTitleLabel;
    QLabel *rotationTitleLabel;
    QLabel *scaleTitleLabel;
    QLabel *directionTitleLabel;

    QLabel *xPosLabel;    QDoubleSpinBox *xPosSpinBox;
    QLabel *yPosLabel;    QDoubleSpinBox *yPosSpinBox;
    QLabel *zPosLabel;    QDoubleSpinBox *zPosSpinBox;

    // objects
    QLabel *objXRotLabel;    QDoubleSpinBox *objXRotSpinBox;
    QLabel *objYRotLabel;    QDoubleSpinBox *objYRotSpinBox;
    QLabel *objZRotLabel;    QDoubleSpinBox *objZRotSpinBox;

    QLabel *objXScaleLabel;  QDoubleSpinBox *objXScaleSpinBox;
    QLabel *objYScaleLabel;  QDoubleSpinBox *objYScaleSpinBox;
    QLabel *objZScaleLabel;  QDoubleSpinBox *objZScaleSpinBox;

    // lights
    QLabel *lightTypeLabel;
    QComboBox *lightTypeComboBox;   // 按照comboBox的种类再次出现不同的控件
    QLabel *lightIntensityLabel;    QDoubleSpinBox *lightIntensitySpinBox;

    // direct light
    QLabel *lightXDirLabel;  QDoubleSpinBox *lightXDirSpinBox;
    QLabel *lightYDirLabel;  QDoubleSpinBox *lightYDirSpinBox;
    QLabel *lightZDirLabel;  QDoubleSpinBox *lightZDirSpinBox;

    QMenuBar *mainMenuBar;
    QMenu *fileMenu;
    QAction *loadModelAction;
};

#endif  //MAINWINDOW_HPP
