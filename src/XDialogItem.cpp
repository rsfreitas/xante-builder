
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:13:42 2017
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

XDialogItem::XDialogItem(const XanteBuilderConfig &config, QWidget *parent)
    : QWidget(parent), config(config)
{
    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *t;
    int index;
    tabItem = new QTabWidget;

    connect(parent, SIGNAL(newSettings()), this, SLOT(handleNewSettings()));

    /* Details */
    t = new TabDetails(config, this);
    connect(t, SIGNAL(itemTypeChanged(int)), this,
            SLOT(prepareWidgetsForCurrentItem(int)));

    connect(t, SIGNAL(dataChanged()), this,
            SLOT(dataChanged()));

    index = tabItem->addTab(t, tr("Details"));
    tabs.insert(index, t);

    /* Content */
    t = new TabContent(config, this);
    connect(t, SIGNAL(dataChanged()), this,
            SLOT(dataChanged()));

    index = tabItem->addTab(t, tr("Content"));
    tabs.insert(index, t);

    /* Ui */
    t = new TabUi(config, this);
    connect(t, SIGNAL(dataChanged()), this,
            SLOT(dataChanged()));

    index = tabItem->addTab(t, tr("Interface"));
    tabs.insert(index, t);

    /* Events */
    t = new TabEvents(config, this);
    connect(t, SIGNAL(dataChanged()), this,
            SLOT(dataChanged()));

    index = tabItem->addTab(t, tr("Events"));
    tabs.insert(index, t);

    layout->addWidget(tabItem);
    setLayout(layout);
}

/*
 * Clears all data inside the widgets, so a new item may be displayed or to
 * leave it clear when a project is closed.
 */
void XDialogItem::clear(void)
{
    QMapIterator<int, QWidget *> it(tabs);

    while (it.hasNext()) {
        it.next();
        dynamic_cast<TabBase *>(it.value())->clearCurrentData();
    }

    currentMenuIndex = -1;
    currentItemIndex = -1;
}

/*
 * Sets data by using the @selectedMenuIndex to get its corresponding
 * XanteItem.
 */
void XDialogItem::setSelection(int selectedMenuIndex, int selectedItemIndex)
{
    QMapIterator<int, QWidget *> it(tabs);
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();

    clear();
    currentMenuIndex = selectedMenuIndex;
    currentItemIndex = selectedItemIndex;

    try {
        XanteMenu menu = jtf.menuAt(currentMenuIndex);
        XanteItem &item = menu.itemAt(currentItemIndex);

        while (it.hasNext()) {
            it.next();
            dynamic_cast<TabBase *>(it.value())->prepareWidgets(item.type());
            dynamic_cast<TabBase *>(it.value())->setSelectedItem(item);
        }
    } catch (std::exception &e) {
        /* Ok, something went wrong. Return the index to the first object. */
        currentMenuIndex = 0;
        currentItemIndex = 0;
    }
}

/*
 * Sets the current project that is been edited, so all other information
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteItem to the @selectedMenuIndex and the
 * @selectedItemIndex inside it.
 */
void XDialogItem::setCurrentProject(int selectedMenuIndex,
    int selectedItemIndex)
{
    setSelection(selectedMenuIndex, selectedItemIndex);
}

/*
 * Creates a XanteItem object from the widgets current content. If a tab
 * encounters an error it must throw an exception, and here we propagate it.
 */
XanteItem XDialogItem::createXanteItemFromWidgets(XanteJTF &jtf,
    const XanteMenu &menu)
{
    QMapIterator<int, QWidget *> it(tabs);

    /*
     * We leave the item with a "fake" name until the real value is read. Then
     * it will be updated.
     */
    XanteItem item(jtf.applicationName(), menu.name(), "fake");

    while (it.hasNext()) {
        it.next();

        try {
            dynamic_cast<TabBase *>(it.value())->updateSelectedItem(item);
        } catch (std::exception &e) {
            throw std::runtime_error(e.what());
        }
    }

    return item;
}

/**
 * @name updateXanteItem
 * @brief Updates the current edited XanteItem data.
 *
 * This function is responsible to update the current edited XanteItem
 * information with data from the widgets.
 *
 * @return If an invalid change has been made returns false, otherwise returns
 *         true, even if no change has been made.
 */
bool XDialogItem::updateXanteItem(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    bool new_item_name = false;

    try {
        XanteMenu &menu = jtf.menuAt(currentMenuIndex);
        XanteItem &item = menu.itemAt(currentItemIndex),
                  newItem = createXanteItemFromWidgets(jtf, menu);

        qDebug() << item.debug();
        qDebug() << newItem.debug();

        if (item != newItem) {
            if (item.name() != newItem.name())
                new_item_name = true;

            item = newItem;

            /* The TreeView must be updated when the names are different. */
            if (new_item_name)
                emit treeViewNeedsUpdate();

            /* The MainWindow must know that we have a change to be saved. */
            emit projectHasChanges();
        }
    } catch (std::exception &e) {
        /* Do we have all the required fields filled? */
        QMessageBox::warning(this, tr("Update error"),
                             QString("The following fields are missing or have wrong value: <ul> %1 </ul>")
                                     .arg(e.what()));
        return false;
    }

    return true;
}

bool XDialogItem::saveCurrentState(void)
{
    return updateXanteItem();
}

void XDialogItem::prepareWidgetsForCurrentItem(int type)
{
    QMapIterator<int, QWidget *> it(tabs);

    while (it.hasNext()) {
        it.next();
        dynamic_cast<TabBase *>(it.value())->prepareWidgets((enum XanteItem::Type)type);
    }
}

void XDialogItem::dataChanged(void)
{
    emit projectHasChanges();
}

void XDialogItem::handleNewSettings(void)
{
    emit newSettings();
}

