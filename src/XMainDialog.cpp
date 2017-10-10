
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Sat Jun 10 10:51:30 2017
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

#include <QWidget>
#include <QSplitter>
#include <QHBoxLayout>

#include "xante_builder.hpp"

XMainDialog::XMainDialog(QWidget *parent)
    : QWidget(parent)
{
    dialog_item = new XDialogItem(this);
    dialog_item->setEnabled(false);
    dialog_menu = new XDialogMenu(this);
    dialog_menu->setEnabled(false);

    tree = new XTreeView(this);
    connect(tree, SIGNAL(item_selected()), this, SLOT(dialog_item_selected()));
    connect(tree, SIGNAL(menu_selected()), this, SLOT(dialog_menu_selected()));
    connect(tree, SIGNAL(content_changed()), this,
            SLOT(dialog_content_changed()));

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(tree);
    splitter->addWidget(dialog_menu);
    splitter->addWidget(dialog_item);
    dialog_item->hide();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter);
    setLayout(layout);
}

/*
 * Here we fill every widget data with project content.
 */
void XMainDialog::active_project(bool active)
{
    if (active) {
        dialog_menu->set_current_project(tree->currentSelectedMenu);
        dialog_item->set_current_project(tree->currentSelectedMenu,
                                         tree->currentSelectedItem);
    } else {
        dialog_menu->clear();
        dialog_item->clear();
    }
}

void XMainDialog::set_tree_content(XTreeModel *model, bool enable_menu)
{
    tree->setModel(model);
    tree->controlDialogActions(enable_menu);
}

void XMainDialog::dialog_item_selected()
{
    dialog_menu->hide();
    dialog_item->show();
    dialog_item->set_selection(tree->currentSelectedMenu,
                               tree->currentSelectedItem);
}

void XMainDialog::dialog_menu_selected()
{
    dialog_menu->show();
    dialog_menu->set_selection(tree->currentSelectedMenu);
    dialog_item->hide();
}

void XMainDialog::dialog_content_changed()
{
}

void XMainDialog::control_project_widgets(bool enable)
{
    dialog_menu->setEnabled(enable);
    dialog_item->setEnabled(enable);
}

