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

FileData* vtk_loader::getFileData(){
    return filedata;
}

void vtk_loader::detectFiletype(){
    if(filedata)
        if (filedata->filename != "NOFILE"){
            if (filedata->filename.contains("mipas", Qt::CaseInsensitive))
                filedata->fileType = MIPAS;
            if (filedata->filename.contains("airs", Qt::CaseInsensitive) ||
                filedata->filename.contains("volcano", Qt::CaseInsensitive))
                filedata->fileType = AIRS;
            if (filedata->filename.contains("clams", Qt::CaseInsensitive))
                filedata->fileType = CLAMS;
            if (filedata->filename.contains("tropo", Qt::CaseInsensitive))
                filedata->fileType = TROPO;
        }
}

void vtk_loader::loadData(QString filename){
    // disable Widget Controls
    emit enableWidgetControls(false);

    // create File objects
    QFile* inFile = new QFile;
    filedata = new FileData;

    // set Filenames
    inFile->setFileName(filename);
    filedata->filename = filename;
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
                vtkPoint* data = new vtkPoint[values];
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
                    data[i] = myPoint;
                    if (!(i%progressUpdate))
                        emit addProgress(1);
                }
                cDataFieldVtkPoint *df = new cDataFieldVtkPoint;
                df->setName(strings.at(0));
                df->setData(data, values);
                filedata->dataFields.push_back(df);
                filedata->numDataFields++;
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

    T* data = new T[numVal];
    if (lines > 1){
        emit setProgress(QString("Loading %1 (%2) ...")
                         .arg(fieldName).arg(lines));
        emit resetProgress();
        int progressUpdate = lines/100;
        T value0, value1;
        int k = 0;
        for (int i=0; i < lines; ++i){
            inFile->read((char*)&value0, sizeof(T));
            value0 = reverse<T>(value0);
            /*datafield->*/data[k++] = value0;
            for (int j=0; j < value0; ++j){
                inFile->read((char*)&value1, sizeof(T));
                value1 = reverse<T>(value1);
                /*datafield->*/data[k++] = value1;
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
            /*datafield->*/data[i] = value;
            if (!(i%progressUpdate))
                emit addProgress(1);
        }
    }

    switch (checkType<T>()) {
    case INT_T:
        {
            cDataFieldInt* df = new cDataFieldInt;
            df->setName(fieldName);
            df->setData((int*)data, numVal);
            filedata->dataFields.push_back(df);
            filedata->numDataFields++;
        }
        break;
    case UINT_T:
        {
            cDataFieldUInt* df = new cDataFieldUInt;
            df->setName(fieldName);
            df->setData((unsigned int*)data, numVal);
            filedata->dataFields.push_back(df);
            filedata->numDataFields++;
        }
        break;
    case FLOAT_T:
    {
        cDataFieldFloat* df = new cDataFieldFloat;
        df->setName(fieldName);
        df->setData((float*)data, numVal);
        filedata->dataFields.push_back(df);
        filedata->numDataFields++;
    }
        break;
    case DOUBLE_T:
    {
        cDataFieldDouble* df = new cDataFieldDouble;
        df->setName(fieldName);
        df->setData((double*)data, numVal);
        filedata->dataFields.push_back(df);
        filedata->numDataFields++;
    }
        break;
    case UCHAR_T:
    {
        cDataFieldUChar* df = new cDataFieldUChar;
        df->setName(fieldName);
        df->setData((unsigned char*)data, numVal);
        filedata->dataFields.push_back(df);
        filedata->numDataFields++;
    }
        break;
    case VTKPOINT_T:
    {
        cDataFieldVtkPoint* df = new cDataFieldVtkPoint;
        df->setName(fieldName);
        df->setData((vtkPoint*)data, numVal);
        filedata->dataFields.push_back(df);
        filedata->numDataFields++;
    }
        break;
    default:
        break;
    }
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
        QString outfilename = filedata->filename;
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
    int progUpPerField = 100/filedata->dataFields.size();
    emit setProgress("Saving to File ...");
    emit resetProgress();


    for (QList<caDataField*>::Iterator it = filedata->dataFields.begin();
         it != filedata->dataFields.end(); ++it)
    {
        (*it)->writeToOut(out);
        emit addProgress(progUpPerField);
    }
    outFile->close();
    delete outFile;

    emit setProgress(" >> DONE! <<");

    // enable Widget Controls
    emit enableWidgetControls(true);
}


QList<QList<PolyLine> > vtk_loader::createPolyLines(){
    QList<QList<PolyLine> > lineList;
    if (filedata->fileType != CLAMS)
        return lineList;

    cDataFieldInt *dfLines = static_cast<cDataFieldInt*>(filedata->getDatafield("LINES"));
    cDataFieldVtkPoint *dfPoints = static_cast<cDataFieldVtkPoint*>(filedata->getDatafield("POINTS"));
    cDataFieldDouble *dfTime = static_cast<cDataFieldDouble*>(filedata->getDatafield("time"));
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
