
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 11:05:40 2017
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

#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QApplication>

#include "xante_builder.hpp"

XTreeView::XTreeView(QWidget *parent)
    : QTreeView(parent)
{
    createRightClickMenu();
}

XTreeView::~XTreeView()
{
}

void XTreeView::createRightClickMenu(void)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu;
    acAddMenu = menu->addAction(tr("Add new menu"), this,
                                &XTreeView::addMenu);

    acAddMenu->setStatusTip(tr("Add a new menu into the JTF file."));
    acRenameMenu = menu->addAction(tr("Rename menu"), this,
                                   &XTreeView::renameMenu);

    acRenameMenu->setStatusTip(tr("Rename the selected menu."));
    acAddItem = menu->addAction(tr("Add new item"), this,
                                &XTreeView::addItem);

    acAddItem->setStatusTip(tr("Add a new item into the selected menu."));
    menu->addSeparator();
    acCopy = menu->addAction(tr("Copy"), this, &XTreeView::copyItem);
    acCopy->setStatusTip(tr("Copy the selected item to memory."));
    acPaste = menu->addAction(tr("Paste"), this, &XTreeView::pasteItem);
    acPaste->setStatusTip(tr("Paste the item holded in memory."));
    acCut = menu->addAction(tr("Cut"), this, &XTreeView::cutItem);
    acCut->setStatusTip(tr("Removes the selected item and put it into the memory."));
    acRemove = menu->addAction(tr("Remove"), this, &XTreeView::removeItem);
    acRemove->setStatusTip(tr("Removes the selected item."));
    menu->addSeparator();
    acChange = menu->addAction(tr("Change items' position"), this,
                               &XTreeView::changeItemsPosition);

    acChange->setStatusTip(tr("Change all items' position inside the current menu."));
}

void XTreeView::controlDialogActions(bool enable)
{
    acAddMenu->setEnabled(enable);
    acRenameMenu->setEnabled(enable);
    acAddItem->setEnabled(enable);
    acCopy->setEnabled(enable);
    acPaste->setEnabled(enable);
    acCut->setEnabled(enable);
    acRemove->setEnabled(enable);
    acChange->setEnabled(enable);
}

void XTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    displaySelectedItem(index);
}

void XTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        menu->exec(event->globalPos());
    else {
        QModelIndex index = indexAt(event->pos()),
                    parentIndex = index.parent();

        if (parentIndex.isValid() || index.isValid())
            selectedLine = true;
        else
            selectedLine = false;

        QTreeView::mousePressEvent(event);
    }
}

void XTreeView::displaySelectedItem(QModelIndex index)
{
    QModelIndex parentIndex = index.parent();

    /* Did we select an item? */
    if (parentIndex.isValid()) {
        currentSelectedMenu = parentIndex.row();
        currentSelectedItem = index.row();
        emit itemSelected();
    } else if (index.isValid()) { /* Or a menu? */
        currentSelectedMenu = index.row();
        currentSelectedItem = -1;
        emit menuSelected();
    }
}

void XTreeView::addMenu()
{
}

void XTreeView::renameMenu()
{
}

void XTreeView::addItem()
{
}

void XTreeView::copyItem()
{
}

void XTreeView::pasteItem()
{
}

void XTreeView::cutItem()
{
}

void XTreeView::removeItem()
{
}

void XTreeView::changeItemsPosition()
{
}

