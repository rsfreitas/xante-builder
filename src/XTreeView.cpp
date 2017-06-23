
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
    create_menu();
}

XTreeView::~XTreeView()
{
}

void XTreeView::create_menu(void)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    menu = new QMenu;
    ac_add_menu = menu->addAction(tr("Add new menu"), this,
                                  &XTreeView::add_menu);

    ac_add_menu->setStatusTip(tr("Add a new menu into the JTF file."));
    ac_rename_menu = menu->addAction(tr("Rename menu"), this,
                                     &XTreeView::rename_menu);

    ac_rename_menu->setStatusTip(tr("Rename the selected menu."));
    ac_add_item = menu->addAction(tr("Add new item"), this,
                                  &XTreeView::add_item);

    ac_add_item->setStatusTip(tr("Add a new item into the selected menu."));
    menu->addSeparator();
    ac_copy = menu->addAction(tr("Copy"), this, &XTreeView::copy_item);
    ac_copy->setStatusTip(tr("Copy the selected item to memory."));
    ac_paste = menu->addAction(tr("Paste"), this, &XTreeView::paste_item);
    ac_paste->setStatusTip(tr("Paste the item holded in memory."));
    ac_cut = menu->addAction(tr("Cut"), this, &XTreeView::cut_item);
    ac_cut->setStatusTip(tr("Removes the selected item and put it into the memory."));
    ac_remove = menu->addAction(tr("Remove"), this, &XTreeView::remove_item);
    ac_remove->setStatusTip(tr("Removes the selected item."));
    menu->addSeparator();
    ac_change = menu->addAction(tr("Change items' position"), this,
                                &XTreeView::change_items_position);

    ac_change->setStatusTip(tr("Change all items' position inside the current menu."));
}

void XTreeView::control_actions(bool enable)
{
    ac_add_menu->setEnabled(enable);
    ac_rename_menu->setEnabled(enable);
    ac_add_item->setEnabled(enable);
    ac_copy->setEnabled(enable);
    ac_paste->setEnabled(enable);
    ac_cut->setEnabled(enable);
    ac_remove->setEnabled(enable);
    ac_change->setEnabled(enable);
}

void XTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex index = indexAt(event->pos());
    display_selected_item(index);
}

void XTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        menu->exec(event->globalPos());
    else {
        QModelIndex index = indexAt(event->pos()),
                    parent_index = index.parent();

        if (parent_index.isValid() || index.isValid())
            selected_line = true;
        else
            selected_line = false;

        QTreeView::mousePressEvent(event);
    }
}

void XTreeView::display_selected_item(QModelIndex index)
{
    QModelIndex parent_index = index.parent();

    /* Did we select an item? */
    if (parent_index.isValid()) {
        current_selected_menu = parent_index.row();
        current_selected_item = index.row();
        emit item_selected();
    } else if (index.isValid()) { /* Or a menu? */
        current_selected_menu = index.row();
        current_selected_item = -1;
        emit menu_selected();
    }
}

void XTreeView::add_menu()
{
}

void XTreeView::rename_menu()
{
}

void XTreeView::add_item()
{
}

void XTreeView::copy_item()
{
}

void XTreeView::paste_item()
{
}

void XTreeView::cut_item()
{
}

void XTreeView::remove_item()
{
}

void XTreeView::change_items_position()
{
}

