#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vtk_filter.h"
#include <QFileDialog>
#include <QThread>
#include <QMimeData>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    glWid = new render_widget();
    connect(ui->actionLoad_vtk, SIGNAL(triggered()), this, SLOT(loadVTKFile()));
    connect(ui->actionClear_List, SIGNAL(triggered()), this, SLOT(clearList()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->action_show, SIGNAL(triggered()), glWid, SLOT(show()));
}

MainWindow::~MainWindow()
{
    clearList();
    delete ui;
//    for (QList<vtk_loader*>::Iterator i = loaderList.begin(); i != loaderList.end(); ++i){
//        delete *i;
//    }
//    for (QList<QThread*>::Iterator i = threadList.begin(); i !=  threadList.end(); ++i){
//        (*i)->quit();
//        (*i)->deleteLater();
//    }
//    for (QList<loader_widget*>::Iterator i = widgetList.begin(); i != widgetList.end(); ++i){
//        delete *i;
//    }
}

void MainWindow::dropEvent(QDropEvent *event){
    QList<QUrl> files = event->mimeData()->urls();
    foreach(QUrl url, files){
        if (url.toString().right(3) == "vtk")
            creatNewLoader(url.toString().remove(0, 8));
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event){
    event->accept();
}

void MainWindow::clearList(){
    foreach (loader_widget* wid, widgetList){
        deleteLoader(wid);
    }
}

void MainWindow::closeProgram(){
    clearList();
    close();
}

void MainWindow::loadVTKFile(){
    // open FileDialog, get Filenames
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                tr("Open File"),
                                "C:/Users/Slip742617/Documents/Uni/Interaktive Visualisierung nicht skalarer Daten/Praktikum/Daten/",
                                tr("VTK_Legacy (*.vtk)"));

    if (filenames.isEmpty())
        return;

    // for all Files ....
    for (QStringList::Iterator filename = filenames.begin();
         filename != filenames.end();
         ++filename){
        creatNewLoader(*filename);
    }
}

void MainWindow::creatNewLoader(QString filename){
    // create new progressWidget, Thread, Loader and insert to Lists
    loader_widget *newWidget = new loader_widget(this);
    widgetList.push_back(newWidget);
    vtk_loader * newLoader = new vtk_loader;
    loaderList.push_back(newLoader);
    QThread *newThread = new QThread;
    threadList.push_back(newThread);
    newLoader->moveToThread(newThread);
    newThread->start();

    // connect Signals: Loader <-> Widget
    connect(newLoader, SIGNAL(setProgressingFilename(QString)), newWidget, SLOT(setFileName(QString)));
    connect(newLoader, SIGNAL(setProgress(QString)), newWidget, SLOT(setProgressStat(QString)));
    connect(newLoader, SIGNAL(addProgress(int)), newWidget, SLOT(addProgressPercent(int)));
    connect(newLoader, SIGNAL(resetProgress()), newWidget, SLOT(resetProgress()));
    connect(newLoader, SIGNAL(enableWidgetControls(bool)), newWidget, SLOT(enableControls(bool)));
    connect(newWidget, SIGNAL(deleteLoader(loader_widget*)), this, SLOT(deleteLoader(loader_widget*)));
    connect(newThread, SIGNAL(finished()), newThread, SLOT(deleteLater()));

    // Add Widget to QListWidget
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
    item->setSizeHint(QSize(0, 50));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, newWidget);
    newWidget->show();

    // emit filename to Loader
    QMetaObject::invokeMethod(newLoader, "loadData", Q_ARG(QString, filename));
}

// handle the Context-Menu
void MainWindow::showCustomContext(QPoint pos){
    // get Pointer of the Sender Widget and Loader
    loader_widget *sendWidget = (loader_widget*)sender();
    vtk_loader *sendLoader = loaderList.at(widgetList.indexOf(sendWidget));
    // get global right-click position
    QPoint globalPos = sendWidget->mapToGlobal(pos);

    // create Menu and Actions
    QMenu myMenu;
    QAction* filterAction = myMenu.addAction("apply Filter ...");
    QAction* saveTxtAction = myMenu.addAction("save *.txt");
    QAction* saveAsAction = myMenu.addAction("save as ..");
    QAction* createPolyLineAction = 0;
    if (sendLoader->getFileData()->fileType() == CLAMS){
        myMenu.addSeparator();
        createPolyLineAction = myMenu.addAction("create Polyline");
    }
    QAction* addToDrawAction = myMenu.addAction("add to Drawlist");
    myMenu.addSeparator();
    QAction* removeAction = myMenu.addAction("remove");
    // ...

    if (ui->listWidget->selectedItems().size() > 1)
        saveAsAction->setEnabled(false);

    // execute Menu
    QAction* selectedAction = myMenu.exec(globalPos);
    // check for Actions and execute
    if (selectedAction)
    {
        if (selectedAction == filterAction){
            vtk_filter *newFilter = new vtk_filter(sendLoader, this);
            //newFilter->moveToThread(threadList.at(widgetList.indexOf(sendWidget)));
            QMetaObject::invokeMethod(newFilter, "filteringData");
        }
        // delete selected Items
        if (selectedAction == removeAction){
            QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
            foreach(QListWidgetItem *item, selected){
                deleteLoader((loader_widget*)ui->listWidget->itemWidget(item));
            }
        }
        // save selected Items to TXT-File (same Path+Name)
        else if (selectedAction == saveTxtAction){
            QList<QListWidgetItem*> selected = ui->listWidget->selectedItems();
            if (selected.size() > 1)
                foreach(QListWidgetItem *item, selected){
                    QMetaObject::invokeMethod(loaderList.at(widgetList.indexOf(
                                                            (loader_widget*)ui->listWidget->itemWidget(item))),
                                               "saveAsFile");
                }
            else
               QMetaObject::invokeMethod(loaderList.at(widgetList.indexOf(
                                                           (loader_widget*)ui->listWidget->itemWidget(selected.at(0)))),
                                           "saveAsFile");
        }
        // save selected Items as choosen File
        else if (selectedAction == saveAsAction){
            saveAsFile(widgetList.indexOf(sendWidget));
        }
        else if(selectedAction == createPolyLineAction){
            loaderList.at(widgetList.indexOf(sendWidget))->createPolyLines();
        }
        else if (selectedAction == addToDrawAction){
            glWid->addToDrawlist(loaderList.at(widgetList.indexOf(sendWidget))->getFileData());
        }
    }
    else
    {
        // nothing was chosen
    }

    delete filterAction;
    delete saveAsAction;
    delete saveTxtAction;
    if (createPolyLineAction){
        delete createPolyLineAction;
    }
    delete addToDrawAction;
    delete removeAction;
}

void MainWindow::saveAsFile(int loaderId){
    QString saveFilename = QFileDialog::getSaveFileName(this, "Save File As ...", "/");
    if (saveFilename.isEmpty())
        return;
    QMetaObject::invokeMethod(loaderList.at(loaderId), "saveAsFile", Q_ARG(QString, saveFilename));
}

void MainWindow::deleteLoader(loader_widget* widgetPtr){
    int i = widgetList.indexOf(widgetPtr);

    ui->listWidget->removeItemWidget(ui->listWidget->item(i));
    delete ui->listWidget->item(i);

    // remove from drawList (QGLWidget)
    glWid->removeFromDrawlist(loaderList.at(i)->getFileData());

    widgetList.at(i)->deleteLater();
    loaderList.at(i)->deleteLater();

    threadList.at(i)->quit();
    //threadList.at(i)->terminate();
    //threadList.at(i)->deleteLater();

    widgetList.removeAt(i);
    loaderList.removeAt(i);
    threadList.removeAt(i);
}
