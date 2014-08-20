#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include "render_widget.h"
#include "cfiledata.h"

class RenderWindow : public QWidget
{
    Q_OBJECT
public:
    explicit RenderWindow(render_widget *renderWid, QWidget *parent = 0);

private:
    render_widget *m_RenderWidget;
    QSlider *m_Slider;
    QVBoxLayout *m_MainLayout;

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void newTimeVal(double time);
    void keyForward(QKeyEvent *event);

public slots:
    void setSliderRange(cFileData *fileData);
    void setSliderMinMax(int min, int max);

};

#endif // RENDERWINDOW_H
