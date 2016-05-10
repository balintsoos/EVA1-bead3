#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#include <QSqlRecord>
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSqlRelationalTableModel>

#include "movietablemodel.h"
#include "userdialog.h"

class EditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditorDialog(QWidget *parent = 0);
    ~EditorDialog();

private slots:
    void addButton_Clicked();
    void removeButton_Clicked();
    void rentButton_Clicked();
    void returnButton_Clicked();
    void submitButton_Clicked();

private:
    void setupModel();
    void setupUi();

    // Model
    MovieTableModel* _model;

    // View
    QTableView* _tableView;

    // User dialog
    UserDialog* _userDialog;

    // Button box
    QDialogButtonBox* _buttonBox;

    // Buttons
    QPushButton* _addButton;
    QPushButton* _removeButton;
    QPushButton* _rentButton;
    QPushButton* _returnButton;
    QPushButton* _submitButton;
    QPushButton* _revertButton;

    // Layout
    QVBoxLayout* _mainLayout;
};

#endif // EDITORDIALOG_H
