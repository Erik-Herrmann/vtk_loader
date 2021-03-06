#ifndef VTK_LOADER_H
#define VTK_LOADER_H

#include <QObject>
#include <QFile>
#include "datastruct.h"
#include "cfiledata.h"
#include "datafield/datafieldincludes.h"

class vtk_loader : public QObject
{

    Q_OBJECT
public:
    explicit vtk_loader(QObject *parent = 0);
    ~vtk_loader();

    cFileData* getFileData();
    void switchFileData(cFileData *newData);
    QList<QList<PolyLine> > createPolyLines();

private:
    cFileData *filedata;

    template <typename T>
    void readDataField(QFile *inFile, QString *line,
                       int lines, int numVal);

    // switch between Little Endian <-> Big Endian
    template <typename T>
    T reverse(T inVal);

    template<typename T>
    MyTypes checkType();

    void detectFiletype();

signals:
    //>> Signals to control the Widget
    // set Name of File
    void setProgressingFilename(QString name);
    // set Status of Progress
    void setProgress(QString progress);
    // adds percent to progressbar
    void addProgress(int percent);
    // set progressbar to 0
    void resetProgress();
    // enable Controls if progress finished
    void enableWidgetControls(bool on);

public slots:
    void loadData(QString filename);
    void saveAsFile();
    void saveAsFile(QString filename);
};

#endif // VTK_LOADER_H
