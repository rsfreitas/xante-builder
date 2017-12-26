
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 14:46:31 2017
 * Project: xante-builder
 *
 * Copyright (C) 2017 Rodrigo Freitas
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "xante_builder.hpp"

/*
 *
 * XTreeItem
 *
 */

XTreeItem::XTreeItem(const QList<QVariant> &data, int itemType,
    XTreeItem *parent) : m_itemData(data), m_parentItem(parent)
{
/*    if (itemType != XanteItem::Type::Unknown) {
        QPixmap pixmap = QPixmap(QString(""));
        m_icon = pixmap.scaled(18, 18);
    }*/
}

XTreeItem::~XTreeItem()
{
    qDeleteAll(m_childItems);
}

void XTreeItem::appendChild(XTreeItem *item)
{
    m_childItems.append(item);
}

XTreeItem *XTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int XTreeItem::childCount() const
{
    return m_childItems.count();
}

int XTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant XTreeItem::data(int column) const
{
    return m_itemData.value(column);
}

XTreeItem *XTreeItem::parent()
{
    return m_parentItem;
}

int XTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<XTreeItem *>(this));

    return 0;
}

QPixmap XTreeItem::icon() const
{
    return m_icon;
}

/*
 *
 * XTreeModel
 *
 */

XTreeModel::XTreeModel(bool enable, QWidget *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << tr("File");

    rootItem = new XTreeItem(rootData);

    if (enable)
        setupModelData(rootItem);
}

XTreeModel::~XTreeModel()
{
    delete rootItem;
}

int XTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<XTreeItem *>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant XTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    XTreeItem *item = static_cast<XTreeItem *>(index.internalPointer());
    return item->data(index.column());

    //XTreeItem *item = static_cast<XTreeItem *>(index.internalPointer());

//        printf("1 %s\n", __FUNCTION__);
//    if (role == Qt::DecorationRole) {
/*        {
            QList<QByteArray> l = QImageReader::supportedImageFormats();
            printf("%s: %d\n", __FUNCTION__, l.size());
        }
        QPixmap pixmap;
        bool x = pixmap.load("/home/rodrigo/git/github/personal/xante-builder/resource/item.jpg");
        printf("2 %s: %d - %d, %d\n", __FUNCTION__, pixmap.isNull(), x,
                QFileInfo::exists("/home/rodrigo/git/github/personal/xante-builder/resource/item.jpg"));
        return QVariant::fromValue(pixmap);
//        return pixmap;*/
/*        return QVariant::fromValue(item->icon());
    } else if (role != Qt::DisplayRole)
        return QVariant();

    return item->data(index.column());*/
}

QModelIndex XTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    XTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<XTreeItem *>(parent.internalPointer());

    XTreeItem *childItem = parentItem->child(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex XTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    XTreeItem *childItem = static_cast<XTreeItem *>(index.internalPointer());
    XTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row (), 0, parentItem);
}

int XTreeModel::rowCount(const QModelIndex &parent) const
{
    XTreeItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<XTreeItem *>(parent.internalPointer());

    return parentItem->childCount();
}

QVariant XTreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

void XTreeModel::setupModelData(XTreeItem *parent)
{
    QList<XTreeItem *> parents;
    XanteProject &project = XMainWindow::getProject();
    XanteJTF jtf = project.getJtf();
    int i, j;

    parents << parent;

    for (i = 0; i < jtf.totalMenus(); i++) {
        XanteMenu menu = jtf.menuAt(i);

        QList<QVariant> childData;
        childData << menu.name();
        parents.last()->appendChild(new XTreeItem(childData,
                                                  XanteItem::Type::Unknown,
                                                  parents.last()));

        for (j = 0; j < menu.totalItems(); j++) {
            XanteItem item = menu.itemAt(j);
            int childIndex = parents.last()->childCount() - 1;

            QList<QVariant> itChildData;
            itChildData << item.name();
            parents.last()->child(childIndex)
                          ->appendChild(new XTreeItem(itChildData, item.type(),
                                                      parents.last()->child(childIndex)));
        }
    }
}

