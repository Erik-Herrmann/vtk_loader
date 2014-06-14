#include "vtk_loader.h"
#include <QTextStream>
#include <QStringList>
#include <typeinfo>


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
                DataField<vtkPoint> *datafield = new DataField<vtkPoint>;
                datafield->dataname = "POINTS";
                datafield->numEntries = values;
                datafield->typeId = VTKPOINT_T;
                datafield->typeSize = datafield->getTypeSizeFromId(datafield->typeId);
                datafield->data = (vtkPoint*)malloc(values*sizeof(vtkPoint));
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
                    datafield->data[i] = myPoint;
                    if (!(i%progressUpdate))
                        emit addProgress(1);
                }
                filedata->dataFields.push_back((DataField<void>*)datafield);
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

    DataField<T> *datafield = new DataField<T>;
    datafield->dataname = fieldName;
    datafield->numEntries = numVal;
    datafield->data = (T*)malloc(numVal*sizeof(T));
    datafield->typeId = checkType<T>();
    datafield->typeSize = sizeof(T);

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
            datafield->data[k++] = value0;
            for (int j=0; j < value0; ++j){
                inFile->read((char*)&value1, sizeof(T));
                value1 = reverse<T>(value1);
                datafield->data[k++] = value1;
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
            datafield->data[i] = value;
            if (!(i%progressUpdate))
                emit addProgress(1);
        }
    }

    filedata->dataFields.push_back((DataField<void>*)datafield);
    filedata->numDataFields++;
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


//template<typename T>
//void writeToOut(void *df_v, int pos, int numArg, va_list vl){
//    if (numArg != 1)
//        return;
//    DataField<T>* df = (DataField<T>*)df_v;
//    QTextStream &out = *va_arg(vl, QTextStream*);
//    if (df->dataname == "POINTS"){
//        out << df->dataname << " " << pos << ": " << df->data[pos].x << " " << df->data[pos].y << " " << df->data[pos].z << "\n";
//    }
//    else if ((df->dataname == "LINES") || (df->dataname == "POLYGONS")){
//        //out << df->dataname << " " << j << ": " << df->data[j] << "\n";
//    }
//    else {
//        out << df->dataname << " " << pos << ": " << df->data[pos] << "\n";
//    }

//    va_end(vl);
//}

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


    for (QList<DataField<void>*>::Iterator it = filedata->dataFields.begin();
         it != filedata->dataFields.end(); ++it)
    {
        int progressUpdate = (*it)->numEntries/progUpPerField;
        out << (*it)->dataname << " " << (*it)->numEntries << "\n";
        switch((*it)->typeId){
        case INT_T:
            if (*it){
                DataField<int> *df = (DataField<int>*)(*it);
                if ((df->dataname == "LINES") || (df->dataname == "POLYGONS")){
                    int pos = 0;
                    while(pos < df->numEntries){
                        int values = df->data[pos++];
                        out << values;
                        for (int k=0; k < values; ++k){
                            out << " " << df->data[pos++];
                            if (!(pos%progressUpdate))
                                emit addProgress(1);
                        }

                        out << "\n";
                    }
                }
                else{
                    DataField<int> *df = (DataField<int>*)(*it);
                    for (int j=0; j < df->numEntries; ++j){
                        out << df->dataname << " " << j << ": " << df->data[j] << "\n";
                        if (!(j%progressUpdate))
                            emit addProgress(1);
                    }
                }
            }
            break;
        case UINT_T:
            if (*it){
                DataField<unsigned int> *df = (DataField<unsigned int>*)(*it);
                for (int j=0; j < df->numEntries; ++j){
                    out << df->dataname << " " << j << ": " << df->data[j] << "\n";
                    if (!(j%progressUpdate))
                        emit addProgress(1);
                }
            }
            break;
        case FLOAT_T:
            if (*it){
                DataField<float> *df = (DataField<float>*)(*it);
                for (int j=0; j < df->numEntries; ++j){
                    out << df->dataname << " " << j << ": " << df->data[j] << "\n";
                    if (!(j%progressUpdate))
                        emit addProgress(1);
                }
            }
            break;
        case DOUBLE_T:
            if (*it){
                DataField<double> *df = (DataField<double>*)(*it);
                for (int j=0; j < df->numEntries; ++j){
                    out << df->dataname << " " << j << ": " << df->data[j] << "\n";
                    if (!(j%progressUpdate))
                        emit addProgress(1);
                }
            }
            break;
        case UCHAR_T:
            if (*it){
                DataField<unsigned char> *df = (DataField<unsigned char>*)(*it);
                for (int j=0; j < df->numEntries; ++j){
                    out << df->dataname << " " << j << ": " << df->data[j] << "\n";
                    if (!(j%progressUpdate))
                        emit addProgress(1);
                }
            }
            break;
        case VTKPOINT_T:
            if (*it){
                DataField<vtkPoint> *df = (DataField<vtkPoint>*)(*it);
                for (int j=0; j < df->numEntries; ++j){
                    out << df->dataname << " " << j << ": " << df->data[j].x << " " << df->data[j].y << " " << df->data[j].z << "\n";
                    if (!(j%progressUpdate))
                        emit addProgress(1);
                }
            }
            break;
        default:
            break;
        }
    }
    outFile->close();
    delete outFile;
    emit setProgress(" >> DONE! <<");

    // enable Widget Controls
    emit enableWidgetControls(true);
}