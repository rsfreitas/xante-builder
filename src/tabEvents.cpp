
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

TabEvents::TabEvents(const XanteBuilderConfig &config, QWidget *parent)
    : QWidget(parent), config(config)
{
    rowLabels << QString("Selected")
              << QString("Confirm value")
              << QString("Value changed")
              << QString("Exit")
              << QString("Extra button pressed")
              << QString("Custom function")
              << QString("Update routine")
              << QString("Custom data")
              << QString("Sync routine")
              << QString("Value strlen")
              << QString("Value check");

    QVBoxLayout *layout = new QVBoxLayout;
    tbEvents = new QTableWidget(XanteItem::Event::MaxEvents, 1);
    connect(tbEvents, SIGNAL(itemChanged(QTableWidgetItem *)), this,
            SLOT(contentChanged(QTableWidgetItem *)));

    connect(tbEvents, SIGNAL(itemSelectionChanged()), this,
            SLOT(selectionChanged()));

    tbEvents->setHorizontalHeaderLabels(QStringList() << tr("Function name"));
    tbEvents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbEvents->setVerticalHeaderLabels(rowLabels);

    layout->addWidget(tbEvents);
    setLayout(layout);
}

void TabEvents::setSelectedItem(const XanteItem &item)
{
    oldItem = item;
    mayNotify = false;

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

        while (i.hasNext()) {
            int event = i.next();
            QString functionName = item.event((enum XanteItem::Event)event);

            if (functionName.isEmpty())
                continue;

            QTableWidgetItem *it = new QTableWidgetItem(functionName);
            tbEvents->setItem(event, 0, it);
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
}

void TabEvents::adjustRowFlags(int row, bool editEnable)
{
    QTableWidgetItem *old, *it;
    QString cnt;

    it = new QTableWidgetItem(cnt);
    it->setFlags((editEnable == false) ? it->flags() & ~Qt::ItemIsEditable
                                       : it->flags());

    old = tbEvents->item(row, 0);
    cnt = (old != NULL) ? old->text() : QString("");
    tbEvents->setItem(row, 0, it);
}

void TabEvents::prepareWidgets(enum XanteItem::Type type)
{
    /* Disable rows after XANTE_ITEM_DEFAULT_EVENTS */
    for (int i = XANTE_ITEM_DEFAULT_EVENTS; i < tbEvents->rowCount(); i++)
        adjustRowFlags(i, false);

    /* Enable the ones that the item is able to use */
    switch (type) {
        case XanteItem::Type::Inputscroll:
            adjustRowFlags(XanteItem::Event::ValueStrlen, true);
            adjustRowFlags(XanteItem::Event::ValueCheck, true);
            break;

        case XanteItem::Type::SpinnerSync:
        case XanteItem::Type::DotsSync:
            adjustRowFlags(XanteItem::Event::SyncRoutine, true);
            break;

        case XanteItem::Type::UpdateObject:
        case XanteItem::Type::Progress:
            adjustRowFlags(XanteItem::Event::UpdateRoutine, true);
            break;

        case XanteItem::Type::Custom:
            adjustRowFlags(XanteItem::Event::CustomFunction, true);
            break;

        default:
            break;
    }
}

void TabEvents::notifyChange(void)
{
    emit dataChanged();
}

void TabEvents::contentChanged(QTableWidgetItem *item)
{
    Q_UNUSED(item);

    if (mayNotify) {
        QString event = oldItem.event((enum XanteItem::Event)item->row());

        if (event != item->text())
            notifyChange();
    }
}

void TabEvents::selectionChanged(void)
{
    mayNotify = true;
}

