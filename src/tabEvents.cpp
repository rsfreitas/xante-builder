
/*
 * Description: Class to handle item's events.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Apr  3 13:54:53 -03 2018
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

TabEvents::TabEvents(QWidget *parent)
    : QWidget(parent)
{
    rowLabels << tr("Selected")
              << tr("Confirm value")
              << tr("Value changed")
              << tr("Exit")
              << tr("Extra button pressed");

    QVBoxLayout *layout = new QVBoxLayout;
    tbEvents = new QTableWidget(XANTE_ITEM_DEFAULT_EVENTS, 1);
    tbEvents->setHorizontalHeaderLabels(QStringList() << tr("Function name"));
    tbEvents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbEvents->setVerticalHeaderLabels(rowLabels);

    layout->addWidget(tbEvents);
    setLayout(layout);
}

void TabEvents::setSelectedItem(const XanteItem &item)
{
    /* These must be in the same order as the rows in the tbEvents */
    events.append(XanteItem::Event::Selected);
    events.append(XanteItem::Event::ValueConfirmed);
    events.append(XanteItem::Event::ValueChanged);
    events.append(XanteItem::Event::Exit);
    events.append(XanteItem::Event::ExtraButtonPressed);

    switch (item.type()) {
        case XanteItem::Type::Inputscroll:
            events.append(XanteItem::Event::ValueStrlen);
            events.append(XanteItem::Event::ValueCheck);
            break;

        case XanteItem::Type::SpinnerSync:
        case XanteItem::Type::DotsSync:
            events.append(XanteItem::Event::SyncRoutine);
            break;

        case XanteItem::Type::UpdateObject:
        case XanteItem::Type::Progress:
            events.append(XanteItem::Event::UpdateRoutine);
            break;

        case XanteItem::Type::Custom:
            events.append(XanteItem::Event::CustomFunction);
            break;

        default:
            break;
    }

    if (item.hasEvents()) {
        QListIterator<int> i(events);
        int row = 0;

        while (i.hasNext()) {
            QString functionName = item.event((enum XanteItem::Event)i.next());

            if (functionName.isEmpty())
                continue;

            QTableWidgetItem *it = new QTableWidgetItem(functionName);
            tbEvents->setItem(row, 0, it);
            row += 1;
        }
    }
}

void TabEvents::updateSelectedItem(XanteItem &item)
{
    QListIterator<int> i(events);

    while (i.hasNext()) {
        int event = i.next();
        QTableWidgetItem *it = tbEvents->item(event, 0);

        if (NULL == it)
            continue;

        QString fname = it->text();

        if (fname.isEmpty() == false)
            item.event(fname, static_cast<enum XanteItem::Event>(event));
    }
}

void TabEvents::clearCurrentData(void)
{
    events.clear();
    tbEvents->clearContents();

    /* We also remove additional rows */
    for (int i = XANTE_ITEM_DEFAULT_EVENTS; i < tbEvents->rowCount(); i++)
        tbEvents->removeRow(i);
}

void TabEvents::prepareWidgets(int type_)
{
    enum XanteItem::Type type = (enum XanteItem::Type)type_;
    int row = tbEvents->rowCount();
    QStringList currentLabels = rowLabels;

    for (int i = XANTE_ITEM_DEFAULT_EVENTS; i < tbEvents->rowCount(); i++)
        tbEvents->removeRow(i);

    switch (type) {
        case XanteItem::Type::Inputscroll:
            tbEvents->insertRow(row++); // ValueStrlen
            currentLabels << QString("Value strlen");

            tbEvents->insertRow(row++); // ValueCheck
            currentLabels << QString("Value check");
            break;

        case XanteItem::Type::SpinnerSync:
        case XanteItem::Type::DotsSync:
            tbEvents->insertRow(row++); // SyncRoutine
            currentLabels << QString("Sync routine");
            break;

        case XanteItem::Type::UpdateObject:
        case XanteItem::Type::Progress:
            tbEvents->insertRow(row++); // UpdateRoutine
            currentLabels << QString("Update routine");
            break;

        case XanteItem::Type::Custom:
            tbEvents->insertRow(row++); // CustomFunction
            currentLabels << QString("Custom function");
            break;

        default:
            break;
    }

    tbEvents->setVerticalHeaderLabels(currentLabels);
}

