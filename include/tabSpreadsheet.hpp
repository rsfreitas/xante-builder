
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Apr  3 14:04:03 -03 2018
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

#ifndef _TABSPREADSHEET_HPP
#define _TABSPREADSHEET_HPP          1

#include <QWidget>
#include "tabBase.hpp"

class XanteItem;

class TabSpreadsheet : public QWidget, public TabBase
{
    Q_OBJECT

    public:
        TabSpreadsheet(QWidget *parent = 0);
        ~TabSpreadsheet() {}

        void setSelectedItem(const XanteItem &item);
        void updateSelectedItem(XanteItem &item);
        void clearCurrentData(void);
        void prepareWidgets(int type);
};

#endif

