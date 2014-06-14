#include "vtk_filter.h"

vtk_filter::vtk_filter(FileData *source, QObject *parent) :
    QObject(parent),
    unfiltered(source)
{
    mainFrame = new QScrollArea;
    mainFrame->setWindowTitle("Select Filters ...");
    mainLayout = new QVBoxLayout;
    QHBoxLayout *header = new QHBoxLayout;
    header->addWidget(new QLabel("Enable"), 2);
    header->addWidget(new QLabel("DataField"), 4);
    header->addWidget(new QLabel("NumFilter"), 2);
    header->addWidget(new QLabel("Compairs"), 6);
    mainLayout->addLayout(header, 14);

    int row = 1;
    for (QList<DataField<void>*>::Iterator curFieldIt = unfiltered->dataFields.begin();
         curFieldIt != unfiltered->dataFields.end(); ++curFieldIt)
    {
        FilterLayout *newFilterLay = new FilterLayout;
        filterLayouts.push_back(newFilterLay);

        newFilterLay->chbEnable = new QCheckBox("");
        newFilterLay->lbFilterName = new QLabel((*curFieldIt)->dataname);

        newFilterLay->cobNumComp = new QComboBox;
        newFilterLay->cobNumComp->addItem("1");
        newFilterLay->cobNumComp->addItem("2");
        newFilterLay->cobNumComp->addItem("3");

        connect(newFilterLay->cobNumComp, SIGNAL(currentIndexChanged(int)), this, SLOT(compValNumChanged(int)));

        newFilterLay->myLayout = new QHBoxLayout;
        newFilterLay->myLayout->addWidget(newFilterLay->chbEnable, 2);
//        newFilterLay->myLayout->addSpacerItem(new QSpacerItem(2, 10));
        newFilterLay->myLayout->addWidget(newFilterLay->lbFilterName, 4);
//        newFilterLay->myLayout->addSpacerItem(new QSpacerItem(2, 10));
        newFilterLay->myLayout->addWidget(newFilterLay->cobNumComp, 2);

        newFilterLay->compLayout = new QVBoxLayout;

        for (int i=0; i < newFilterLay->cobNumComp->currentText().toInt(); ++i){
            createNewCompLayout(newFilterLay);
        }

        newFilterLay->myLayout->addLayout(newFilterLay->compLayout, 6);

        mainLayout->addLayout(newFilterLay->myLayout, 14);
        row++;
    }

    mainFrame->setLayout(mainLayout);

}

vtk_filter::~vtk_filter(){
    foreach (FilterLayout *filterLay, filterLayouts){
        mainLayout->removeItem(filterLay->myLayout);
        delete filterLay;
    }
    filterLayouts.clear();
    mainFrame->deleteLater();
    unfiltered = 0;

}

void vtk_filter::compValNumChanged(int idx){
    QComboBox *sendBox = (QComboBox*)sender();
    FilterLayout *filterLay;
    foreach (FilterLayout* lay, filterLayouts){
        if (lay->cobNumComp == sendBox){
            filterLay = lay;
            break;
        }
    }

    int newNumber = sendBox->currentText().toInt();

    if (newNumber > filterLay->compairs.size()){
        while (newNumber != filterLay->compairs.size())
            createNewCompLayout(filterLay);
    }
    else if (newNumber < filterLay->cobNumComp->count()){
        foreach (ValueCompLay* compLay, filterLay->compairs) {
            delete compLay;
        }
        filterLay->myLayout->removeItem(filterLay->compLayout);
        delete filterLay->compLayout;
        filterLay->compairs.clear();

        filterLay->compLayout = new QVBoxLayout;
        for (int i=0; i < filterLay->cobNumComp->currentText().toInt(); ++i){
            createNewCompLayout(filterLay);
        }
        filterLay->myLayout->addLayout(filterLay->compLayout, 6);
    }
}

void vtk_filter::createNewCompLayout(FilterLayout *filterLay){
    ValueCompLay* newValComp = new ValueCompLay;

    newValComp->lbCompName = new QLabel("Comp_" + QString::number(filterLay->compairs.size()));

    newValComp->cobOperator = new QComboBox;
    newValComp->cobOperator->addItem("==");
    newValComp->cobOperator->addItem(">");
    newValComp->cobOperator->addItem("<");
    newValComp->cobOperator->addItem(">=");
    newValComp->cobOperator->addItem("<=");
    newValComp->cobOperator->addItem("!=");

    newValComp->ledCompValue = new QLineEdit;

    newValComp->myLayout = new QHBoxLayout;
    newValComp->myLayout->addWidget(newValComp->lbCompName);
    newValComp->myLayout->addWidget(newValComp->cobOperator);
    newValComp->myLayout->addWidget(newValComp->ledCompValue);

    filterLay->compairs.push_back(newValComp);
    filterLay->compLayout->addLayout(newValComp->myLayout);
}

void vtk_filter::showFilters(){
    mainFrame->show();
}

void vtk_filter::applyFilters(){
    int dfIdx = 0;
    filtered = new FileData;
    filtered->filename = unfiltered->filename;
    foreach (FilterLayout* filter, filterLayouts)
    {
        if (filter->chbEnable->isChecked()){

            switch (unfiltered->dataFields.at(dfIdx)->typeId){
            case INT_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                    for (int i=0; i < df->numEntries; ++i){
                        //...
                    }
                }
                break;
            case UINT_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                }
                break;
            case FLOAT_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                }
                break;
            case DOUBLE_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                }
                break;
            case UCHAR_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                }
                break;
            case VTKPOINT_T:
                if (unfiltered->dataFields.at(dfIdx)){
                    DataField<int>* df = (DataField<int>*)unfiltered->dataFields.at(dfIdx);
                }
                break;
            default:
                break;
            }
        }
        dfIdx++;
    }
}
