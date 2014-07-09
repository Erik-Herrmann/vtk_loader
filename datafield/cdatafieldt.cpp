#include "cdatafieldt.h"
#include "helpers.h"
#include <QSet>

template<>
void cDataFieldT<double>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        if (m_DataName == "time"){
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << at(i) << " = \t"
                    << Helpers::juliantimeToDatetime(at(i)).toString("dddd dd.MMMM yyyy hh:mm:ss")
                    << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << at(i) << "\n";
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
                out << m_DataName << " " << i << ": " << at(i) << " = \t"
                    << Helpers::juliantimeToDatetime(at(i)).toString("dddd dd.MMMM yyyy hh:mm:ss")
                    << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << at(i) << "\n";
            }
        }
    }
}

template<>
void cDataFieldT<int>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        if ((m_DataName == "LINES") || (m_DataName == "POLYGONS")){
            int pos = 0;
            while(pos < m_NumEntries){
                int values = at(pos++);
                out << values;
                for (int i=0; i < values; ++i){
                    out << " " << at(pos++);
                }
                out << "\n";
            }
        }
        else{
            for (int i=0; i < m_NumEntries; ++i){
                out << m_DataName << " " << i << ": " << at(i) << "\n";
            }
        }
    }
}


template<>
void cDataFieldT<vtkPoint>::writeToOut(QTextStream &out){
    if (m_NumEntries){
        out << m_DataName << " " << m_NumEntries << "\n";
        for (int i=0; i < m_NumEntries; ++i){
            vtkPoint &p = at(i);
            out << m_DataName << " " << i << ": " << p.x << " " << p.y << " " << p.z << "\n";
        }
    }
}

template<>
void cDataFieldT<vtkPoint>::writeValueToOut(QTextStream &out, int index){
    if (index < m_NumEntries){
        vtkPoint &p = at(index);
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
                int lineSeg = at(pos++);
                for (int i=0; i < lineSeg; ++i){
                    if((*op)(at(pos), value)){
                        filterList->insert(at(pos++));
                    }
                    else {
                        pos++;
                    }
                }
            }
        }
        else {
            for (int i=0; i < m_NumEntries; ++i){
                if((*op)(at(i), value)){
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
            if(op(at(i), value)){
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
                int lineSeg = at(pos++);
                QList<int> line;
                for (int i=0; i < lineSeg; ++i){
                    std::set<int>::iterator elemIt = indices.find(at(pos));
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
            cDataFieldT<int> *newDf = new cDataFieldT<int>(sumEntries);
            newDf->setName(m_DataName);
            pos = 0;
            foreach(QList<int> line, lines){
                if (!line.isEmpty()){
                    newDf->push_back(line.size());
                    foreach(int l, line){
                        newDf->push_back(l);
                    }
                }
            }
            return static_cast<caDataField*>(newDf);
        }
        else {
            cDataFieldT<int> *newDf = new cDataFieldT<int>(indices.size());
            newDf->setName(m_DataName);
            for (int i : indices) {
                newDf->push_back(at(i));
            }
            return static_cast<caDataField*>(newDf);
        }
    }
    return 0;
}
