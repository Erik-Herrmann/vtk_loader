#ifndef LOADER_WIDGET_H
#define LOADER_WIDGET_H

#include <QWidget>

namespace Ui {
class LoaderWidget;
}

class loader_widget : public QWidget
{
    Q_OBJECT
public:
    explicit loader_widget(QWidget *parent = 0);
    ~loader_widget();

private:
    Ui::LoaderWidget *ui;

signals:
    void deleteLoader(loader_widget* widgetPtr);

public slots:
    void setFileName(QString name);
    void setProgressStat(QString progress);
    void addProgressPercent(int value);
    void resetProgress();
    void enableControls(bool on);

private slots:
    void on_toolButton_clicked();
};

#endif // LOADER_WIDGET_H
