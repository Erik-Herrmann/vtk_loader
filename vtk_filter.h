#ifndef VTK_FILTER_H
#define VTK_FILTER_H

#include <QObject>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include "datastruct.h"
#include "vtk_loader.h"


struct ValueCompLay{
    ValueCompLay(){
        lbCompName = 0;
        cobOperator = 0;
        ledCompValue = 0;
    }
    ~ValueCompLay(){
        myLayout->removeWidget(ledCompValue);
        myLayout->removeWidget(cobOperator);
        myLayout->removeWidget(lbCompName);
        myLayout->deleteLater();
        lbCompName->deleteLater();
        cobOperator->deleteLater();
        ledCompValue->deleteLater();
    }
    QHBoxLayout *myLayout;
    QLabel *lbCompName;
    QComboBox *cobOperator;
    QLineEdit *ledCompValue;
};

struct FilterLayout{
    FilterLayout(){
        chbEnable = 0;
        lbFilterName = 0;
        cobNumComp = 0;
        compLayout = 0;
    }
    ~FilterLayout(){
        foreach (ValueCompLay* compLay, compairs){
            compLayout->removeItem(compLay->myLayout);
            delete compLay;
        }
        compairs.clear();

        myLayout->removeItem(compLayout);
        myLayout->removeWidget(chbEnable);
        myLayout->removeWidget(lbFilterName);
        myLayout->removeWidget(cobNumComp);
        chbEnable->deleteLater();
        lbFilterName->deleteLater();
        cobNumComp->deleteLater();
        compLayout->deleteLater();
        myLayout->deleteLater();
    }

    QHBoxLayout *myLayout;
    QCheckBox *chbEnable;
    QLabel *lbFilterName;
    QComboBox *cobNumComp;

    QVBoxLayout *compLayout;
    QList<ValueCompLay*> compairs;
};

class vtk_filter : public QObject
{
    Q_OBJECT
public:
    explicit vtk_filter(vtk_loader *loader, QObject *parent = 0);
    ~vtk_filter();

    cFileData* getFilteredData();

private:
    vtk_loader *m_LoaderPtr;
    cFileData *m_unfiltered;
    cFileData *m_filtered;

    QScrollArea *mainFrame;
    QVBoxLayout *mainLayout;
    QPushButton *applyButton;
    QList<FilterLayout*> filterLayouts;

    void createNewCompLayout(FilterLayout *filterLay);

signals:

private slots:
    void compValNumChanged(int idx);
    void applyFilters();

public slots:
    void filteringData();

};

#endif // VTK_FILTER_H
