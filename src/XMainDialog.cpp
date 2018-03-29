
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
    dialogItem = new XDialogItem(this);
    dialogItem->setEnabled(false);
    dialogMenu = new XDialogMenu(this);
    dialogMenu->setEnabled(false);

    tree = new XTreeView(this);
    connect(tree, SIGNAL(itemSelected()), this, SLOT(dialogItemSelected()));
    connect(tree, SIGNAL(menuSelected()), this, SLOT(dialogMenuSelected()));
    connect(tree, SIGNAL(treeViewNeedsUpdate()), this,
            SLOT(updateTreeView()));

    connect(dialogMenu, SIGNAL(treeViewNeedsUpdate()), this,
            SLOT(updateTreeView()));

    connect(dialogItem, SIGNAL(treeViewNeedsUpdate()), this,
            SLOT(updateTreeView()));

    connect(tree, SIGNAL(projectHasChanges()), this,
            SLOT(projectChanged()));

    connect(dialogMenu, SIGNAL(projectHasChanges()), this,
            SLOT(projectChanged()));

    connect(dialogItem, SIGNAL(projectHasChanges()), this,
            SLOT(projectChanged()));

    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(tree);
    splitter->addWidget(dialogMenu);
    splitter->addWidget(dialogItem);
    dialogItem->hide();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(splitter);
    setLayout(layout);
}

/*
 * Here we fill every widget data with project content.
 */
void XMainDialog::activeProject(bool active)
{
    if (active) {
        dialogMenu->setCurrentProject(tree->currentSelectedMenu);
        dialogItem->setCurrentProject(tree->currentSelectedMenu,
                                      tree->currentSelectedItem);
    } else {
        dialogMenu->clear();
        dialogItem->clear();
    }
}

void XMainDialog::setTreeContent(XTreeModel *model, bool enableMenu)
{
    tree->setModel(model);
    tree->controlDialogActions(enableMenu);
    tree->expandAll();
}

void XMainDialog::dialogItemSelected()
{
    dialogMenu->hide();
    dialogItem->hide();
    dialogItem->show();
    dialogItem->setSelection(tree->currentSelectedMenu,
                             tree->currentSelectedItem);
}

void XMainDialog::dialogMenuSelected()
{
    dialogItem->hide();
    dialogMenu->hide();
    dialogMenu->show();
    dialogMenu->setSelection(tree->currentSelectedMenu);
}

void XMainDialog::controlProjectWidgets(bool enable)
{
    dialogMenu->setEnabled(enable);
    dialogItem->setEnabled(enable);
}

void XMainDialog::updateTreeView()
{
    XTreeModel *model = new XTreeModel(true, this);
    tree->setModel(model);
    tree->expandAll();
}

void XMainDialog::projectChanged()
{
    emit projectHasChanges();
}

void XMainDialog::saveCurrentState(void)
{
    dialogItem->saveCurrentState();
    dialogMenu->saveCurrentState();
}

