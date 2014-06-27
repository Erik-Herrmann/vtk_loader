#include "cdatafieldt.h"
#include "helpers.h"
#include <QSet>

template<>
void cDataFieldT<double>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        if (m_DataName == "time"){
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << " = \t"
                    << Helpers::juliantimeToDatetime(m_Data[i]).toString("dddd dd.MMMM yyyy hh:mm:ss")
                    << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
            }
        }
    }
}

template<>
void cDataFieldT<float>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        if (m_DataName == "time"){
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << " = \t"
                    << Helpers::juliantimeToDatetime(m_Data[i]).toString("dddd dd.MMMM yyyy hh:mm:ss")
                    << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << m_Data[i] << "\n";
            }
        }
    }
}

template<>
void cDataFieldT<vtkPoint>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        for (int i=0; i < m_NumEntries; ++i){
            vtkPoint &p = m_Data[i];
            out << m_DataName << " " << i << ": " << p.x << " " << p.y << " " << p.z << "\n";
        }
    }
}

template<>
void cDataFieldT<vtkPoint>::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries){
        vtkPoint &p = m_Data[index];
        out << m_DataName << " " << index << ": " << p.x << " " << p.y << " " << p.z << "\n";
    }
}


template<>
void cDataFieldT<int>::filterData(std::set<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        int value = valStr.toInt();
        // create funktion pointer
        //std::function<bool(int, int)> op;
        bool (*op)(int&, int&);
        // set function to pointer by opId
        switch (opId) {
        case 0:
            op = &Helpers::equal_to;
            break;
        case 1:
            op = &Helpers::not_equal_to;
            break;
        case 2:
            op = &Helpers::greater;
            break;
        case 3:
            op = &Helpers::greater_equal;
            break;
        case 4:
            op = &Helpers::less;
            break;
        case 5:
            op = &Helpers::less_equal;
            break;
        }
        // compair and update List
        if ((m_DataName == "LINES") || (m_DataName == "POLYGONS")){
            int pos =0;
            while (pos < m_NumEntries){
                int lineSeg = m_Data[pos++];
                for (int i=0; i < lineSeg; ++i){
                    if((*op)(m_Data[pos], value)){
                        filterList->insert(m_Data[pos++]);
                    }
                    else {
                        pos++;
                    }
                }
            }
        }
        else {
            for (int i=0; i < m_NumEntries; ++i){
                if((*op)(m_Data[i], value)){
                    filterList->insert(i);
                }
            }
        }
    }
}

template<>
void cDataFieldT<vtkPoint>::filterData(std::set<int> *filterList, int opId, QString valStr){
    if (m_NumEntries){
        // get compair value
        float value = valStr.toDouble();
        // create funktion pointer
        bool (*op)(vtkPoint&, float&) = 0;
        // set funktion to pointer by opId
        switch (opId) {
        case 0:
            op = &Helpers::equal_to;
            break;
        case 1:
            op = &Helpers::not_equal_to;
            break;
        case 2:
            op = &Helpers::greater;
            break;
        case 3:
            op = &Helpers::greater_equal;
            break;
        case 4:
            op = &Helpers::less;
            break;
        case 5:
            op = &Helpers::less_equal;
            break;
        }
        // compair and update List
        for (int i=0; i < m_NumEntries; ++i){
            if(op(m_Data[i], value)){
                filterList->insert(i);
            }
        }
    }
}


template<>
caDataField* cDataFieldT<int>::getDatafieldOfListedIndices(std::set<int> &indices){
    if (m_NumEntries){
        if ((m_DataName == "LINES") || (m_DataName == "POLYGONS")){
            int pos =0;
            QList<QList<int> > lines;
            while (pos < m_NumEntries){
                int lineSeg = m_Data[pos++];
                QList<int> line;
                for (int i=0; i < lineSeg; ++i){
                    std::set<int>::iterator elemIt = indices.find(m_Data[pos]);
                    if (elemIt != indices.end()){
                        line.push_back(std::distance(indices.begin(), elemIt));
                    }
                    pos++;
                }
                lines.push_back(line);
            }
            int sumEntries = 0;
            foreach(QList<int> line, lines){
                if (!line.isEmpty())
                    sumEntries += line.size()+1;
            }
            int *newData = new int[sumEntries];
            pos = 0;
            foreach(QList<int> line, lines){
                if (!line.isEmpty()){
                    newData[pos++] = line.size();
                    foreach(int l, line){
                        newData[pos++] = l;
                    }
                }
            }
            cDataFieldT<int> *newDf = new cDataFieldT<int>;
            newDf->setName(m_DataName);
            newDf->setData(newData, sumEntries);

            return static_cast<caDataField*>(newDf);
        }
        else {
            int *newData = new int[indices.size()];
            int pos = 0;
            for (int i : indices) {
                newData[pos++] = m_Data[i];
            }
            cDataFieldT<int> *newDf = new cDataFieldT<int>;
            newDf->setName(m_DataName);
            newDf->setData(newData, indices.size());

            return static_cast<caDataField*>(newDf);
        }
    }
    return 0;
}
