#include "FluxPythonTextEditor.h"

PythonTextEditor::PythonTextEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    this->appendPlainText(">>");
}

void PythonTextEditor::keyReleaseEvent(QKeyEvent *e) {
    if(e->key() == Qt::Key_Return) {
        this->appendPlainText("Eval Python...");
        this->appendPlainText(">>");
    }
}
