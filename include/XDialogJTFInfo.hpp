
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Jun 13 16:27:19 2017
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

#ifndef _XDIALOGJTFINFO_HPP
#define _XDIALOGJTFINFO_HPP          1

#include <QDialog>
#include <QWidget>
#include <QList>

class QComboBox;
class QLineEdit;
class QGroupBox;
class QHBoxLayout;
class XanteProject;

class XDialogJTFInfo : public QDialog
{
    Q_OBJECT

    public:
        XDialogJTFInfo(QWidget *parent = 0);
        ~XDialogJTFInfo() {};

    private slots:
        void confirmOk(void);
        void confirmCancel(void);

    private:
        enum editWidget {
            ApplicationName,
            Description,
            Company,
            Plugin,
            CfgPathname,
            LogPathname,
            Version,
            Revision,
            Build
        };

        /* UI */
        QComboBox *cbMainMenu, *cbBeta;
        QList<QLineEdit *> edit;

        void createWidgets(void);
        void reject(void);
        void fillWidgetsWithProjectData(void);
        void prepareMainMenuChooser(void);
        QHBoxLayout *createMainMenuChooser(void);
        QHBoxLayout *createBetaChooser(void);
        QHBoxLayout *createButtons(void);
        QGroupBox *createInformationWidgets(void);
};

#endif

