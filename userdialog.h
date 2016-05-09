#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QListView>
#include <QSqlTableModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QSqlQuery>

class UserDialog : public QDialog
{
    Q_OBJECT
public:
    UserDialog(QWidget *parent = 0);

    void setMovie(int movieId);

private slots:
    void selectButton_Clicked();

private:
    void setupModel();
    void setupUi();

    int _movie;

    // Model
    QSqlTableModel* _model;

    // View
    QListView* _listView;

    // Buttons
    QPushButton* _selectButton;

    // Button box
    QDialogButtonBox* _buttonBox;

    // Layout
    QHBoxLayout* _mainLayout;
};

#endif // USERDIALOG_H
