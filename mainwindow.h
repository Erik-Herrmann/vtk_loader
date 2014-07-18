#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QThread>
#include <QDropEvent>
#include "loader_widget.h"
#include "vtk_loader.h"
#include "renderwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

private:
    Ui::MainWindow *ui;
    QList<loader_widget*> widgetList;
    QList<vtk_loader*> loaderList;
    QList<QThread*> threadList;
    vtk_loader *myLoader;
    int progessbarPercent;

    RenderWindow *renderWindow;
    render_widget *glWid;

    void creatNewLoader(QString filename);

signals:
    void loader_loadFile(QString filename);

public slots:
    void loadVTKFile();
    void saveAsFile(int loaderId);
    void deleteLoader(loader_widget* widgetPtr);
    void showCustomContext(QPoint pos);
    void clearList();
    void closeProgram();
};

#endif // MAINWINDOW_H
