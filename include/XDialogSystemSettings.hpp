
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Apr  2 10:45:05 -03 2018
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

#ifndef _XDIALOGSYSTEMSETTINGS_HPP
#define _XDIALOGSYSTEMSETTINGS_HPP              1

#include <QDialog>
#include <QWidget>

class QGroupBox;
class QHBoxLayout;
class QLineEdit;
class XanteBuilderConfig;

class XDialogSystemSettings : public QDialog
{
    Q_OBJECT

    public:
        XDialogSystemSettings(XanteBuilderConfig &config, QWidget *parent = 0);
        ~XDialogSystemSettings() {}

    private slots:
        void confirmOk(void);
        void confirmCancel(void);
        void chooseSourceTemplatePath(void);
        void chooseJerminusPath(void);

    private:
        QLineEdit *leSourceTemplatePath, *leJerminusPath;
        XanteBuilderConfig &config;

        void createWidgets(void);
        QHBoxLayout *createButtons(void);
        QGroupBox *createUtilitiesWidgets(void);
};

#endif

