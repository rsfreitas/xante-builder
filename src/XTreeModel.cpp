
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

XTreeItem::XTreeItem(const QList<QVariant> &data, XTreeItem *parent)
{
    parentItem = parent;
    itemData = data;
}

XTreeItem::~XTreeItem()
{
    qDeleteAll(childItems);
}

void XTreeItem::appendChild(XTreeItem *item)
{
    childItems.append(item);
}

XTreeItem *XTreeItem::child(int row)
{
    return childItems.value(row);
}

int XTreeItem::childCount() const
{
    return childItems.count();
}

int XTreeItem::columnCount() const
{
    return itemData.count();
}

QVariant XTreeItem::data(int column) const
{
    return itemData.value(column);
}

XTreeItem *XTreeItem::parent()
{
    return parentItem;
}

int XTreeItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<XTreeItem *>(this));

    return 0;
}

/*
 *
 * XTreeModel
 *
 */

XTreeModel::XTreeModel(QWidget *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> root_data;
    root_data << tr("File");

    root_item = new XTreeItem(root_data);
}

XTreeModel::XTreeModel(XanteProject *project, QWidget *parent)
    : QAbstractItemModel(parent), project(project)
{
    QList<QVariant> root_data;
    root_data << tr("File");

    root_item = new XTreeItem(root_data);
    setup_model_data(root_item);
}

XTreeModel::~XTreeModel()
{
    delete root_item;
}

int XTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<XTreeItem *>(parent.internalPointer())->columnCount();
    else
        return root_item->columnCount();
}

QVariant XTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    XTreeItem *item = static_cast<XTreeItem *>(index.internalPointer());
    return item->data(index.column());
}

QModelIndex XTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    XTreeItem *parent_item;

    if (!parent.isValid())
        parent_item = root_item;
    else
        parent_item = static_cast<XTreeItem *>(parent.internalPointer());

    XTreeItem *child_item = parent_item->child(row);

    if (child_item)
        return createIndex(row, column, child_item);
    else
        return QModelIndex();
}

QModelIndex XTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    XTreeItem *child_item = static_cast<XTreeItem *>(index.internalPointer());
    XTreeItem *parent_item = child_item->parent();

    if (parent_item == root_item)
        return QModelIndex();

    return createIndex(parent_item->row (), 0, parent_item);
}

int XTreeModel::rowCount(const QModelIndex &parent) const
{
    XTreeItem *parent_item;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parent_item = root_item;
    else
        parent_item = static_cast<XTreeItem *>(parent.internalPointer());

    return parent_item->childCount();
}

QVariant XTreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return root_item->data(section);

    return QVariant();
}

void XTreeModel::setup_model_data(XTreeItem *parent)
{
    QList<XTreeItem *> parents;
    XanteJTF jtf = project->get_jtf();
    int i, j;

    parents << parent;

    for (i = 0; i < jtf.total_menus(); i++) {
        XanteMenu menu = jtf.menu_at(i);

        QList<QVariant> child_data;
        child_data << menu.get_name();
        parents.last()->appendChild(new XTreeItem(child_data, parents.last()));

        for (j = 0; j < menu.total_items(); j++) {
            XanteItem item = menu.item_at(j);
            int child_index = parents.last()->childCount() - 1;

            QList<QVariant> it_child_data;
            it_child_data << item.get_name();
            parents.last()->child(child_index)
                          ->appendChild(new XTreeItem(it_child_data,
                                                      parents.last()->child(child_index)));
        }
    }
}

