#include "vtk_loader.h"
#include <QTextStream>
#include <QStringList>
#include <typeinfo>
#include "datafield/datafieldincludes.h"
#include "helpers.h"


vtk_loader::vtk_loader(QObject *parent) :
    QObject(parent)
{
}

vtk_loader::~vtk_loader(){
    delete filedata;
}

cFileData *vtk_loader::getFileData(){
    return filedata;
}

void vtk_loader::switchFileData(cFileData *newData){
    filedata = newData;
}

void vtk_loader::detectFiletype(){
    if(filedata)
        if (filedata->filename() != "NOFILE"){
            if (filedata->filename().contains("mipas", Qt::CaseInsensitive))
                filedata->setFiletype(MIPAS);
            if (filedata->filename().contains("airs", Qt::CaseInsensitive) ||
                filedata->filename().contains("volcano", Qt::CaseInsensitive))
                filedata->setFiletype(AIRS);
            if (filedata->filename().contains("clams", Qt::CaseInsensitive))
                filedata->setFiletype(CLAMS);
            if (filedata->filename().contains("tropo", Qt::CaseInsensitive))
                filedata->setFiletype(TROPO);
        }
}

void vtk_loader::loadData(QString filename){
    // disable Widget Controls
    emit enableWidgetControls(false);

    // create File objects
    QFile* inFile = new QFile;
    filedata = new cFileData;

    // set Filenames
    inFile->setFileName(filename);
    filedata->setFilename(filename);
    emit setProgressingFilename(filename.right(filename.length()-filename.lastIndexOf('/')-1));

    detectFiletype();

    // open File
    inFile->open(QIODevice::ReadOnly);

    // define some variables
    QString line;

    // create TextStream to read lines
    QTextStream in(inFile);
    line = in.readLine();

    // read the whole filedata
    while (!line.isNull()) {
        inFile->seek(in.pos());
        QStringList strings = line.split(' ');
        if (strings.size() > 3){
            QString type_s = strings.at(3);
            if (type_s == "int"){
                readDataField<int>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
            else if (type_s == "unsigned_int"){
                readDataField<unsigned int>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
            else if (type_s == "float"){
                readDataField<float>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
            else if (type_s == "double"){
                readDataField<double>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
            else if(type_s == "unsigned_char"){
                readDataField<unsigned char>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
        }
        else {
            if (strings.at(0) == "POINTS"){
                int values = strings.at(1).toInt();
                cDataFieldT<vtkPoint> *df = new cDataFieldT<vtkPoint>(values);
                df->setName(strings.at(0));
                int progressUpdate = values/100;
                emit resetProgress();
                emit setProgress(QString("Loading Points (%1) ...").arg(values));
                for (int i = 0; i < values; ++i){
                    vtkPoint myPoint;
                    inFile->read((char*)&myPoint.x, sizeof(float));
                    inFile->read((char*)&myPoint.y, sizeof(float));
                    inFile->read((char*)&myPoint.z, sizeof(float));
                    myPoint.x = reverse(myPoint.x);
                    myPoint.y = reverse(myPoint.y);
                    myPoint.z = reverse(myPoint.z);
                    df->push_back(myPoint);
                    if (!(i%progressUpdate))
                        emit addProgress(1);
                }
                filedata->push_back(df);
            }
            else if ((strings.at(0) == "POLYGONS") || (strings.at(0) == "LINES")){
                readDataField<int>(inFile, &line, strings.at(1).toInt(), strings.at(2).toInt());
            }
        }
        in.seek(inFile->pos());
        line = in.readLine();
    }

    inFile->close();
    delete inFile;
    emit setProgress(" >> DONE! <<");

    // enable Controls
    emit enableWidgetControls(true);
}


template <typename T>
void vtk_loader::readDataField(QFile *inFile, QString *line,
                               int lines, int numVal)
{
    QString fieldName = line->left(line->indexOf(' '));

    cDataFieldT<T>* df = new cDataFieldT<T>(numVal);
    df->setName(fieldName);
    if (lines > 1){
        emit setProgress(QString("Loading %1 (%2) ...")
                         .arg(fieldName).arg(lines));
        emit resetProgress();
        int progressUpdate = lines/100;
        T value0, value1;
        for (int i=0; i < lines; ++i){
            inFile->read((char*)&value0, sizeof(T));
            value0 = reverse<T>(value0);
            df->push_back(value0);
            for (int j=0; j < value0; ++j){
                inFile->read((char*)&value1, sizeof(T));
                value1 = reverse<T>(value1);
                df->push_back(value1);
            }
            if (!(i%progressUpdate))
                emit addProgress(2);
        }
    }
    else {
        emit setProgress(QString("Loading %1 (%2) ...")
                         .arg(fieldName).arg(numVal));
        emit resetProgress();
        int progressUpdate = numVal/100;
        T value;
        for (int i=0; i < numVal; ++i){
            inFile->read((char*)&value, sizeof(T));
            value = reverse<T>(value);
            df->push_back(value);
            if (!(i%progressUpdate))
                emit addProgress(1);
        }
    }
    filedata->push_back(df);
}


template <typename T>
T vtk_loader::reverse(T inVal){
    T retVal;
    char *valToConvert = (char*) &inVal;
    char *returnVal = (char*) &retVal;

    int size = sizeof(T);
    for (int i=0; i < size; ++i){
        returnVal[i] = valToConvert[size-i-1];
    }
    return retVal;
}


template <typename T>
MyTypes vtk_loader::checkType(){
    if (typeid(T) == typeid(int))
        return INT_T;
    if (typeid(T) == typeid(unsigned int))
        return UINT_T;
    if (typeid(T) == typeid(float))
        return FLOAT_T;
    if (typeid(T) == typeid(double))
        return DOUBLE_T;
    if (typeid(T) == typeid(unsigned char))
        return UCHAR_T;
    if (typeid(T) == typeid(vtkPoint))
        return VTKPOINT_T;

    return NOTYPE;
}


void vtk_loader::saveAsFile(){
    if (filedata){
        QString outfilename = filedata->filename();
        outfilename.chop(3);
        outfilename.append("txt");
        saveAsFile(outfilename);
    }
}


void vtk_loader::saveAsFile(QString filename){
    if (!filedata)
        return;

    // disable Widget Controls
    emit enableWidgetControls(false);

    QFile *outFile = new QFile(filename);
    if (!outFile->open(QIODevice::WriteOnly))
        return;

    QTextStream out(outFile);
    int progUpPerField = 100/filedata->numDataFields();
    emit setProgress("Saving to File ...");
    emit resetProgress();


    foreach (caDataField* df, filedata->getDatafieldList())
    {
        df->writeToOut(out);
        emit addProgress(progUpPerField);
    }
    outFile->close();
    delete outFile;

    // Progess -> 100%
    emit addProgress(100-(filedata->numDataFields()*progUpPerField));
    emit setProgress(" >> DONE! <<");

    // enable Widget Controls
    emit enableWidgetControls(true);
}


QList<QList<PolyLine> > vtk_loader::createPolyLines(){
    QList<QList<PolyLine> > lineList;
    if (filedata->fileType() != CLAMS)
        return lineList;

    cDataFieldT<int> *dfLines = static_cast<cDataFieldT<int>*>(filedata->getDatafield("LINES"));
    cDataFieldT<vtkPoint> *dfPoints = static_cast<cDataFieldT<vtkPoint>*>(filedata->getDatafield("POINTS"));
    cDataFieldT<double> *dfTime = static_cast<cDataFieldT<double>*>(filedata->getDatafield("time"));
    // ... more datafields needed

    int pos = 0;
    while(pos < dfLines->numEntries()){
        int values = dfLines->getValueAt(pos++);
        QList<PolyLine> line;
        for(int i=0; i < values; ++i){
            int idx = dfLines->getValueAt(pos++);
            PolyLine elem;
            elem.point = dfPoints->getValueAt(idx);
            elem.time = Helpers::juliantimeToDatetime(dfTime->getValueAt(idx));
            //... add more data to struct
            line.push_back(elem);
        }
        lineList.push_back(line);
    }
    return lineList;
}
