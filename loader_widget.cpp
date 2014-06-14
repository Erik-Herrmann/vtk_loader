#include "loader_widget.h"
#include "ui_loaderwidget.h"

loader_widget::loader_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoaderWidget)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), parent, SLOT(showCustomContext(QPoint)));
}

loader_widget::~loader_widget(){
    delete ui;
}

void loader_widget::setFileName(QString name){
    ui->lblFileName->setText(name);
}

void loader_widget::setProgressStat(QString progress){
    ui->lblProgressName->setText(progress);
}

void loader_widget::addProgressPercent(int value){
    ui->progBar->setValue(ui->progBar->value()+value);
}

void loader_widget::resetProgress(){
    ui->progBar->setValue(0);
}

void loader_widget::enableControls(bool on){
    ui->toolButton->setEnabled(on);
    if (on){
        setContextMenuPolicy(Qt::CustomContextMenu);
    }
    else{
        setContextMenuPolicy(Qt::NoContextMenu);
    }
}

void loader_widget::on_toolButton_clicked()
{
    emit deleteLoader(this);
}
