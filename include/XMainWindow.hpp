
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

#include <XanteCommon.hpp>

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

        static bool activeProject(void) { return (project != nullptr); }
        static XanteProject &getProject(void) { return *project; }

    protected:
        void closeEvent(QCloseEvent *event) override;

    private slots:
        void newProject();
        void openProject();
        void saveProject();
        void closeProject();
        void editJtfInfo();
        void jtfTest();
        void aboutUs();
        void openRecentFile();
        void projectChanged();

    private:
        static XanteProject *project;
        XanteConfig &config;
        bool editingProject = false;

        /* UI */
        QAction *acOpen, *acClose, *acSave, *acJtfMainInfo, *acTestJtf,
                *acNewProject, *acRecentFiles[MaxRecentFiles];

        XMainDialog *dialog;

        void createMenuActions(void);
        void createMenu(void);
        void loadFile(const QString &filename);
        void setWindowWidgetsEnabled(bool enable);
        void setCurrentFile(const QString &filename);
};

#endif

