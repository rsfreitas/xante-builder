
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:14:48 2017
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

#ifndef _XDIALOGITEM_HPP
#define _XDIALOGITEM_HPP          1

#include <QtWidgets>
#include <xante/libxante.h>

class TabSpreadsheet;
class TabMixedform;
class XanteProject;
class XanteBuilderConfig;

class XDialogItem : public QWidget
{
    Q_OBJECT

    public:
        XDialogItem(const XanteBuilderConfig &config, QWidget *parent = 0);
        ~XDialogItem() {}
        void setCurrentProject(int selectedMenuIndex,
                               int selectedItemIndex);

        void setSelection(int selectedMenuIndex, int selectedItemIndex);
        void clear(void);
        bool saveCurrentState(void);

    private slots:
        void prepareWidgetsForCurrentItem(int type);
        void dataChanged(void);
        void handleNewSettings(void);

    signals:
        void treeViewNeedsUpdate();
        void projectHasChanges();
        void newSettings(void);

    private:
        const XanteBuilderConfig &config;
        int currentMenuIndex = -1, currentItemIndex = -1, spreadsheetTabKey = -1,
            mixedformTabKey = -1;

        /* UI */
        QTabWidget *tabItem;
        TabSpreadsheet *spreadsheet;
        TabMixedform *mixedform;
        QMap<int, QWidget *> tabs;

        bool updateXanteItem(void);
        XanteItem createXanteItemFromWidgets(XanteJTF &jtf, const XanteMenu &menu);
        void updateSpreadsheetTab(enum XanteItem::Type type);
};

#endif

