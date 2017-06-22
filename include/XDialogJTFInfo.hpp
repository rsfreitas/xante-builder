
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

class QComboBox;
class QLineEdit;
class QGroupBox;
class QHBoxLayout;
class XanteProject;

class XDialogJTFInfo : public QDialog
{
    Q_OBJECT

    public:
        XDialogJTFInfo(XanteProject *project, QWidget *parent = 0);
        ~XDialogJTFInfo();

    private slots:
        void confirm_ok(void);
        void confirm_cancel(void);

    private:
        XanteProject *project = nullptr;

        /* UI */
        QComboBox *cb_main_menu, *cb_beta;
        QLineEdit *le_application_name, *le_description, *le_company,
                  *le_plugin, *le_cfg_pathname, *le_log_pathname,
                  *le_version, *le_revision, *le_build;

        void create_widgets(void);
        void reject(void);
        QHBoxLayout *create_main_menu_chooser(void);
        QHBoxLayout *create_beta_chooser(void);
        QHBoxLayout *create_buttons(void);
        QGroupBox *create_information_widgets(void);
};

#endif

