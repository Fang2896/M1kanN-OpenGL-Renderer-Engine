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
#include <QSplitter>
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

   protected:   // some override functions
    void showEvent(QShowEvent *event) override;

   private: // functions
    void initWidget();
    void initLayout();
    void connectConfigure();

   private slots: // slots
    void updateGLManager();

    void onLoadGameObjectCube();
    void onLoadGameObjectQuad();
    void onLoadModel();
    void onObjectDeleteButtonClicked();

    // dash configure slot functions
    void onEnableLightingCheckBox(int state);
    void onEnableLineModeCheckBox(int state);

    // inspector:
    void onDisplayCheckBox(int state);

    // spinBox:
    void onXPosSpinBoxValueChanged(double value);
    void onYPosSpinBoxValueChanged(double value);
    void onZPosSpinBoxValueChanged(double value);

    void onXRotSpinBoxValueChanged(double value);
    void onYRotSpinBoxValueChanged(double value);
    void onZRotSpinBoxValueChanged(double value);

    void onXScaSpinBoxValueChanged(double value);
    void onYScaSpinBoxValueChanged(double value);
    void onZScaSpinBoxValueChanged(double value);

    void onObjectItemSelect(QListWidgetItem *item);

    void handleEditingFinished();

   private: // 辅助函数
    void setObjectTransformToSpinBox(const std::shared_ptr<GameObject>& ptr ,QVector3D pos, QVector3D rot, QVector3D sca);
    QListWidgetItem* getItemById(QListWidget* listWidget, int id) const;

   private: // filters
    bool eventFilter(QObject *watched, QEvent *event) override;

   private: // configure variables
    const QString modelDirectory = "../assets/models";
    const QString textureDirectory = "../assets/textures";

    const int objectDataBaseIdRole = Qt::UserRole + 1;
    const int lightDataBaseIdRole = Qt::UserRole + 2;

   private: // variables
    Ui::MainWindow* ui;
    GLManager *glManager;
    QTimer *timer;

    // 当前选中的object或者light的ID
    int currentObjectID;

    // widgets
    QSplitter *contentSplitter;
    QSplitter *glDashSplitter;

    QGroupBox *sceneGroupBox;
    QLabel *lightTitleLabel;
    QLabel *objectTitleLabel;
    QToolButton *lightAddButton;
    QPushButton *lightDeleteButton;
    QToolButton *objectAddButton;
    QPushButton *objectDeleteButton;
    QListWidget *lightList;
    QListWidget *objectList;

    // Tool Button Menu
    QMenu *objectAddMenu;
    QMenu *objectShapeSelectMenu;

    // TODO: 后面会增加更多
    QAction *objectLoadShapeCubeAction;
    QAction *objectLoadShapeQuadAction;

    QAction *objectLoadModelAction;

    QMenu *lightAddMenu;
    QAction *pointLightAddAction;
    QAction *spotLightAddAction;

    QGroupBox *inspectorGroupBox;
    QCheckBox *nameCheckBox;
    QLineEdit *nameLineEdit;

    // configure dash widgets
    QTabWidget *configureDashTab;
    QWidget *envTab;
    QWidget *postProcessingTab;

    QCheckBox *enableLightingCheckBox;
    QCheckBox *enableLineModeCheckBox;

    QGroupBox *transformGroupBox;

    // 操作时增加：(需要new)
    QFrame *positionFrame;
    QFrame *rotationFrame;
    QFrame *scaleFrame;
    QFrame *directionFrame;

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
    // direct light
    QLabel *lightXDirLabel;  QDoubleSpinBox *lightXDirSpinBox;
    QLabel *lightYDirLabel;  QDoubleSpinBox *lightYDirSpinBox;
    QLabel *lightZDirLabel;  QDoubleSpinBox *lightZDirSpinBox;

    // object和light改变，就改变frame显示状态即可

    QMenuBar *mainMenuBar;
    QMenu *fileMenu;
    QAction *loadModelAction;
};

#endif  //MAINWINDOW_HPP
