#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include <QDialog>
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QSqlRelationalTableModel>

class EditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EditorDialog(QWidget *parent = 0);
    ~EditorDialog();

private slots:
    void addButton_Clicked();
    void removeButton_Clicked();
    void submitButton_Clicked();

private:
    void setupModel();
    void setupUi();

    // Model
    QSqlRelationalTableModel* _model;

    // View
    QTableView* _tableView;

    // Button box
    QDialogButtonBox* _buttonBox;

    // Buttons
    QPushButton* _addButton;
    QPushButton* _removeButton;
    QPushButton* _submitButton;
    QPushButton* _revertButton;

    // Layout
    QVBoxLayout* mainLayout;
};

#endif // EDITORDIALOG_H
