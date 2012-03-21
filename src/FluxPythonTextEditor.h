#ifndef FLUXPYTHONTEXTEDITOR_H
#define FLUXPYTHONTEXTEDITOR_H

#include <QPlainTextEdit>

class PythonTextEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit PythonTextEditor(QWidget *parent = 0);

protected:
    void keyReleaseEvent(QKeyEvent *e);
signals:

public slots:

};

#endif // FLUXPYTHONTEXTEDITOR_H
