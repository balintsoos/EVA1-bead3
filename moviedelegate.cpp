#include <QStyleOptionViewItem>
#include <QByteArray>
#include <QVector>
#include <QtGui>

#include "moviedelegate.h"

MovieDelegate::MovieDelegate(QObject *parent) :
    QSqlRelationalDelegate(parent)
{

}

void MovieDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    if (index.column() == 6)
    {
        QString text = statusValueList().at(index.data().toInt());

        QStyleOptionViewItem optionViewItem = option;
        optionViewItem.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

        drawDisplay(painter, optionViewItem, optionViewItem.rect, text);
        drawFocus(painter, optionViewItem, optionViewItem.rect);
    }
    else if (index.column() == 7)
    {
        QString text = rentValueList().at(index.data().toInt());

        QStyleOptionViewItem optionViewItem = option;
        optionViewItem.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

        drawDisplay(painter, optionViewItem, optionViewItem.rect, text);
        drawFocus(painter, optionViewItem, optionViewItem.rect);
    }
    else
    {
        QSqlRelationalDelegate::paint(painter, option, index);
    }
}

QWidget *MovieDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() == 6)
    {
        QComboBox *availableComboBox = new QComboBox(parent);
        availableComboBox->addItems(statusValueList());
        return availableComboBox;
    }
    else
    {
        return QSqlRelationalDelegate::createEditor(parent, option, index);
    }
}

void MovieDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.column() == 6)
    {
        int i = index.data().toInt();
        QComboBox *availableComboBox = qobject_cast<QComboBox *>(editor);
        availableComboBox->setCurrentIndex(i);
    }
    else
    {
        QSqlRelationalDelegate::setEditorData(editor, index);
    }
}

void MovieDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.column() == 6)
    {
        QComboBox *availableComboBox = qobject_cast<QComboBox *>(editor);
        model->setData(index, availableComboBox->currentIndex ());
    }
    else
    {
        QSqlRelationalDelegate::setModelData(editor, model, index);
    }
}

QStringList MovieDelegate::statusValueList() const
{
    QStringList list;

    list.append(trUtf8("Locked"));
    list.append(trUtf8("Free"));

    return list;
}

QStringList MovieDelegate::rentValueList() const
{
    QStringList list;

    list.append(trUtf8("In stock"));
    list.append(trUtf8("Rented"));

    return list;
}

