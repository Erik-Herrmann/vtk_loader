#include "vtk_filter.h"
#include <QSet>
#include <QtAlgorithms>

vtk_filter::vtk_filter(vtk_loader *loader, QObject *parent) :
    QObject(parent),
    m_LoaderPtr(loader),
    m_unfiltered(loader->getFileData())
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
    foreach (caDataField* curField, m_unfiltered->getDatafieldList())
    {
        FilterLayout *newFilterLay = new FilterLayout;
        filterLayouts.push_back(newFilterLay);

        newFilterLay->chbEnable = new QCheckBox("");
        newFilterLay->lbFilterName = new QLabel(curField->getName());

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

    applyButton = new QPushButton("apply");
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applyFilters()));
    mainLayout->addWidget(applyButton);

    mainFrame->setLayout(mainLayout);

}

vtk_filter::~vtk_filter(){
    foreach (FilterLayout *filterLay, filterLayouts){
        mainLayout->removeItem(filterLay->myLayout);
        delete filterLay;
    }
    filterLayouts.clear();
    disconnect(applyButton, SIGNAL(clicked()), this, SLOT(applyFilters()));
    delete applyButton;
    mainFrame->deleteLater();
    m_unfiltered = 0;
}


cFileData *vtk_filter::getFilteredData(){
    return m_filtered;
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
    newValComp->cobOperator->addItem("!=");
    newValComp->cobOperator->addItem(">");
    newValComp->cobOperator->addItem(">=");
    newValComp->cobOperator->addItem("<");
    newValComp->cobOperator->addItem("<=");

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
    mainFrame->close();

    // TODO:
    // to prevent errors
    // still need some specialisation for CLAMS data
    if (m_unfiltered->fileType() == CLAMS)
        return;

    int dfIdx = 0;
    m_filtered = new cFileData;
    m_filtered->setFilename(m_unfiltered->filename());
    m_filtered->setFiletype(m_unfiltered->fileType());

    // apply choosen filters and get List of filtered entries
    // the entries are the one who match the filter constraints
    std::set<int> filterList;
    bool firstFilter = true;
    foreach (FilterLayout* filter, filterLayouts)
    {
        if (filter->chbEnable->isChecked()){
            caDataField *df = m_unfiltered->getDatafield(dfIdx);
            for (int i=0; i < filter->cobNumComp->currentIndex()+1; ++i){
                std::set<int> curFilter;
                df->filterData(&curFilter,
                               filter->compairs.at(i)->cobOperator->currentIndex(),
                               filter->compairs.at(i)->ledCompValue->text());

                if (firstFilter){
                    filterList = curFilter;
                    firstFilter = false;
                }
                else {
                    std::vector<int> tmpVec(std::min(filterList.size(), curFilter.size()));
                    std::vector<int>::iterator it;
                    it = std::set_intersection(filterList.begin(), filterList.end(), curFilter.begin(), curFilter.end(), tmpVec.begin());
                    tmpVec.resize(it-tmpVec.begin());
                    std::set<int> tmpSet(tmpVec.begin(), tmpVec.end());
                    filterList = tmpSet;
                }
            }
        }
        dfIdx++;
    }

    // get filtered data from unfiltered

    // create filtered data
    foreach (caDataField* df, m_unfiltered->getDatafieldList()){
        m_filtered->push_back(
                    df->getDatafieldOfListedIndices(filterList));
    }

    m_LoaderPtr->switchFileData(m_filtered);
    delete m_unfiltered;

    this->deleteLater();
}

