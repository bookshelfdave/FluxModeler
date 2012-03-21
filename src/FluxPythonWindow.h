#ifndef FLUXPYTHONWINDOW_H
#define FLUXPYTHONWINDOW_H

#include <QWidget>


class PythonTextEditor;
class QSize;

class PythonWindow : public QWidget        
{
    Q_OBJECT
public:
    explicit PythonWindow(QWidget *parent = 0);
    ~PythonWindow();

private:
    PythonTextEditor *editor;

};

#endif // FLUXPYTHONWINDOW_H
