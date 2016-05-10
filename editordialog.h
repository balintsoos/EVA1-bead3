#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#include <QDateTime>
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

    void filterAll();
    void filterRent();
    void filterStock();

private:
    void setupModel();
    void setupUi();

    void returnMovie(int movie);
    void seconds_to_DHMS(quint32 duration);

    // Model
    MovieTableModel* _model;

    // View
    QTableView* _tableView;

    // User dialog
    UserDialog* _userDialog;

    // Button box
    QDialogButtonBox* _buttonBox;
    QDialogButtonBox* _filterBox;

    // Buttons
    QPushButton* _addButton;
    QPushButton* _removeButton;
    QPushButton* _rentButton;
    QPushButton* _returnButton;
    QPushButton* _submitButton;
    QPushButton* _revertButton;

    QPushButton* _filterAllButton;
    QPushButton* _filterRentButton;
    QPushButton* _filterStockButton;

    // Layout
    QVBoxLayout* _mainLayout;
};

#endif // EDITORDIALOG_H
