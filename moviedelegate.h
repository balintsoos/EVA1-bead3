#ifndef MOVIEDELEGATE_H
#define MOVIEDELEGATE_H

#include <QSqlRelationalDelegate>

class MovieDelegate : public QSqlRelationalDelegate
{
    Q_OBJECT
public:
    MovieDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    QStringList statusValueList() const;
    QStringList rentValueList() const;
};

#endif // MOVIEDELEGATE_H
