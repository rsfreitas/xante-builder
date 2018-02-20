
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Oct 18 16:18:53 2017
 * Project: XDialogItemList.cpp
 *
 * Copyright (C) 2017 Rodrigo Freitas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#include <QStyle>
#include <QGroupBox>

#include "xante_builder.hpp"

XDialogItemList::XDialogItemList(XanteMenu &menu, QWidget *parent)
    : QDialog(parent), m_menu(menu)
{
    setWindowTitle(tr("Menu items adjustment"));
    createWidgets();
    fillWidgetsWithProjectData();
}

void XDialogItemList::confirmOk(void)
{
    done(0);
}

void XDialogItemList::confirmCancel(void)
{
    done(-1);
}

void XDialogItemList::itemUp(void)
{
    int currentIndex = m_itemsList->currentRow();

    if (currentIndex == 0)
        return;

    /* We move the item on the UI */
	QListWidgetItem *currentItem = m_itemsList->takeItem(currentIndex);
    m_itemsList->insertItem(currentIndex - 1, currentItem);
	m_itemsList->setCurrentRow(currentIndex - 1);

    /* And into the real list */
    m_menu.itemMove(currentIndex, currentIndex - 1);
}

void XDialogItemList::itemDown(void)
{
    int currentIndex = m_itemsList->currentRow();

    if (currentIndex == (m_itemsList->count() - 1))
        return;

    /* We move the item on the UI */
	QListWidgetItem *currentItem = m_itemsList->takeItem(currentIndex);
    m_itemsList->insertItem(currentIndex + 1, currentItem);
	m_itemsList->setCurrentRow(currentIndex + 1);

    /* And into the real list */
    m_menu.itemMove(currentIndex, currentIndex + 1);
}

QHBoxLayout *XDialogItemList::createButtons(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QPushButton *btnOk = new QPushButton(tr("Ok"));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(confirmOk()));

    QPushButton *btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(confirmCancel()));

    h->addWidget(btnOk);
    h->addWidget(btnCancel);

    return h;
}

QGroupBox *XDialogItemList::createItemsList(void)
{
    QGroupBox *g = new QGroupBox(m_menu.name());
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;

    QPushButton *up =
        new QPushButton(style()->standardIcon(QStyle::SP_ArrowUp), tr("Up"));

    connect(up, SIGNAL(clicked()), this, SLOT(itemUp()));
    QPushButton *down =
        new QPushButton(style()->standardIcon(QStyle::SP_ArrowDown), tr("Down"));

    connect(down, SIGNAL(clicked()), this, SLOT(itemDown()));

    v->addWidget(up);
    v->addWidget(down);

    m_itemsList = new QListWidget;
    h->addWidget(m_itemsList);
    h->addLayout(v);
    g->setLayout(h);

    return g;
}

void XDialogItemList::createWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;

    v->addWidget(createItemsList());
    v->addLayout(createButtons());

    setLayout(v);
}

void XDialogItemList::fillWidgetsWithProjectData(void)
{
    QStringList items;
    int total = m_menu.totalItems();

    for (int i = 0; i < total; i++) {
        XanteItem it = m_menu.itemAt(i);
        items.append(it.name());
    }

    m_itemsList->addItems(items);
}

