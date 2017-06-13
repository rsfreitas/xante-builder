
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:46:32 2017
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

#ifndef _XMAINWINDOW_HPP
#define _XMAINWINDOW_HPP          1

#include <QMainWindow>

class QAction;
class XanteConfig;
class XanteProject;
class XMainDialog;

class XMainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        XMainWindow(XanteConfig &config);
        ~XMainWindow() {}

    protected:
        void closeEvent(QCloseEvent *event) override;

    private slots:
        void new_project();
        void open_project();
        void save_project();
        void close_project();
        void edit_jtf_info();
        void jtf_test();
        void about_us();

    private:
        XanteConfig &config;
        XanteProject *project = nullptr;
        bool editing_project = false;

        /* UI */
        QAction *ac_open, *ac_close, *ac_save, *ac_jtf_main_info, *ac_test_jtf,
                *ac_new_project;

        XMainDialog *dialog;

        void create_menu_actions(void);
        void create_menu(void);
        void control_menu_options(bool enable);
};

#endif

