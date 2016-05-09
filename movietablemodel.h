#ifndef MOVIETABLEMODEL_H
#define MOVIETABLEMODEL_H

#include <QSqlRelationalTableModel>

class MovieTableModel : public QSqlRelationalTableModel
{
    Q_OBJECT
public:
    MovieTableModel(QObject *parent = 0);

    void insertRow(int row);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
};

#endif // MOVIETABLEMODEL_H
