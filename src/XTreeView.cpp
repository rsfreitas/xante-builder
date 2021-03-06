
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
    acChange = menu->addAction(tr("Change items position"), this,
                               &XTreeView::changeItemsPosition);

    acChange->setStatusTip(tr("Change all items position inside the current menu."));
}

void XTreeView::controlDialogActions(bool enable)
{
    acAddMenu->setEnabled(enable);
    acAddItem->setEnabled(enable);
    acCopy->setEnabled(enable);
    acPaste->setEnabled(enable);
    acCut->setEnabled(enable);
    acRemove->setEnabled(enable);
    acChange->setEnabled(enable);
}

void XTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QModelIndex selectedIndex = indexAt(event->pos());
    displaySelectedItem(selectedIndex);
}

void XTreeView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
        menu->exec(event->globalPos());
    else
        QTreeView::mousePressEvent(event);
}

void XTreeView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Return:
        case Qt::Key_Space:
            displaySelectedItem(currentIndex());
            break;
    }

    QTreeView::keyPressEvent(event);
}

void XTreeView::displaySelectedItem(QModelIndex index)
{
    QModelIndex parentIndex = index.parent();
    int lastSelectedMenu = currentSelectedMenu,
        lastSelectedItem = currentSelectedItem;

    /* Did we select an item? */
    if (parentIndex.isValid()) {
        currentSelectedMenu = parentIndex.row();
        currentSelectedItem = index.row();
        selectedLine = XTreeView::SelectedLine::ItemLine;

        if ((currentSelectedMenu == lastSelectedMenu) &&
            (currentSelectedItem == lastSelectedItem))
        {
            return;
        }

        emit itemSelected();
    } else if (index.isValid()) { /* Or a menu? */
        currentSelectedMenu = index.row();
        currentSelectedItem = -1;
        selectedLine = XTreeView::SelectedLine::MenuLine;

        if ((currentSelectedMenu == lastSelectedMenu) &&
            (currentSelectedItem == lastSelectedItem))
        {
            return;
        }

        emit menuSelected();
    } else
        selectedLine = XTreeView::SelectedLine::None;
}

void XTreeView::addMenu()
{
    if (selectedLine == XTreeView::SelectedLine::None) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a line before inserting a new "
                                "menu."));

        return;
    }

    bool ok;
    QString menuName = QInputDialog::getText(this, tr("Menu name"),
                                             tr("Enter the new menu name:"),
                                             QLineEdit::Normal, QString(""),
                                             &ok);

    if (ok && (menuName.isEmpty() == false)) {
        XanteProject &project = XMainWindow::getProject();
        XanteJTF &jtf = project.getJtf();
        XanteMenu menu(jtf.applicationName(), menuName);
        jtf.addMenu(menu);

        emitSignalToUpdate();
    }
}

void XTreeView::addItem()
{
    if (selectedLine != XTreeView::SelectedLine::MenuLine) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a menu line before inserting a "
                                "new item."));

        return;
    }

    bool ok;
    QString itemName = QInputDialog::getText(this, tr("Item name"),
                                             tr("Enter the new item name:"),
                                             QLineEdit::Normal, QString(""),
                                             &ok);

    if (ok && (itemName.isEmpty() == false)) {
        XanteProject &project = XMainWindow::getProject();
        XanteJTF &jtf = project.getJtf();
        XanteMenu &menu = jtf.menuAt(currentSelectedMenu);
        XanteItem item(jtf.applicationName(), menu.name(), itemName);

        item.referencedMenu(menu.objectId());
        menu.addItem(item);
        emitSignalToUpdate();
    }
}

void XTreeView::copyItem()
{
    if (selectedLine == XTreeView::SelectedLine::None) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a line before copying a menu "
                                "or an item."));

        return;
    }
}

void XTreeView::pasteItem()
{
    if (selectedLine == XTreeView::SelectedLine::None) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a line before pasting a menu "
                                "or an item."));

        return;
    }
}

void XTreeView::cutItem()
{
    if (selectedLine == XTreeView::SelectedLine::None) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a line before cutting a menu "
                                "or an item."));

        return;
    }
}

void XTreeView::removeItem()
{
    if (selectedLine == XTreeView::SelectedLine::None) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a line before removing an menu "
                                "or item."));

        return;
    }

    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();

    if (jtf.totalMenus() == 1) {
        QMessageBox::critical(this, tr("Error"),
                              tr("You can't remove the last menu."));

        return;
    }

    if (QMessageBox::question(this, tr("Removing"),
                              QString(tr("Are you sure you want to remove the %1?"))
                                      .arg((selectedLine == XTreeView::SelectedLine::MenuLine)
                                                ? "menu" : "item")) == QMessageBox::No)
    {
        return;
    }

    if (selectedLine == XTreeView::SelectedLine::MenuLine)
        jtf.removeMenu(currentSelectedMenu);
    else {
        XanteMenu &menu = jtf.menuAt(currentSelectedMenu);
        menu.removeItem(currentSelectedItem);
    }

    emitSignalToUpdate();
}

void XTreeView::changeItemsPosition()
{
    if (selectedLine != XTreeView::SelectedLine::MenuLine) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("You must select a menu line before editting "
                                "its items order."));

        return;
    }

    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    XanteMenu &menu = jtf.menuAt(currentSelectedMenu);

    XDialogItemList dlg(menu, this);

    if (dlg.exec() == 0)
        emitSignalToUpdate();
}

void XTreeView::emitSignalToUpdate(void)
{
    emit treeViewNeedsUpdate();
    emit projectHasChanges();
}

void XTreeView::redraw(QAbstractItemModel *model)
{
    QStringList st = saveState();

    setModel(model);
    restoreState(st);
}

QStringList XTreeView::saveState(void)
{
    XTreeModel *m = dynamic_cast<XTreeModel *>(model());
    QStringList state;

    foreach (QModelIndex index, m->getPersistentIndexList()) {
        if (isExpanded(index))
            state << index.data(Qt::DisplayRole).toString();
    }

    return state;
}

void XTreeView::restoreState(QStringList state)
{
    QAbstractItemModel *m = model();

    foreach (QString item, state) {
        QModelIndexList Items = m->match(m->index(0, 0),
                                         Qt::DisplayRole,
                                         QVariant::fromValue(item));

        if (!Items.isEmpty())
            setExpanded(Items.first(), true);
    }
}

