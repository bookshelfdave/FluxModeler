#include "FluxPythonWindow.h"

#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "FluxPythonTextEditor.h"

PythonWindow::PythonWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout();
    editor = new PythonTextEditor(this);
    QPushButton *exec = new QPushButton("Exec");
    layout->addWidget(editor);
    layout->addWidget(exec);
    this->setLayout(layout);
}


PythonWindow::~PythonWindow() {
    delete editor;
}

