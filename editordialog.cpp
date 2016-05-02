#include "editordialog.h"

EditorDialog::EditorDialog(QWidget *parent) : QDialog(parent)
{
    setupModel();
    setupUi();

    setWindowTitle("Movies");
}

EditorDialog::~EditorDialog()
{
    delete _model;
}

void EditorDialog::addButton_Clicked()
{

}

void EditorDialog::removeButton_Clicked()
{

}

void EditorDialog::submitButton_Clicked()
{

}

void EditorDialog::setupModel()
{

}

void EditorDialog::setupUi()
{

}
