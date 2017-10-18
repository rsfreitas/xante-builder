
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Wed Oct 18 16:17:27 2017
 * Project: XDialogItemList.hpp
 *
 * Copyright (c) 2017 All rights reserved
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

#ifndef _XDIALOGITEMLIST_HPP
#define _XDIALOGITEMLIST_HPP          1

#include <QInputDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QGroupBox>

class XanteMenu;

class XDialogItemList : public QDialog
{
    Q_OBJECT

    public:
        XDialogItemList(XanteMenu &menu, QWidget *parent = 0);
        ~XDialogItemList() {}

    private slots:
        void confirmOk(void);
        void confirmCancel(void);
        void itemUp(void);
        void itemDown(void);

    private:
        XanteMenu &m_menu;
        QListWidget *m_itemsList;

        QHBoxLayout *createButtons(void);
        QGroupBox *createItemsList(void);
        void createWidgets(void);
        void fillWidgetsWithProjectData(void);
};

#endif

