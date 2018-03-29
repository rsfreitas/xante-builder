
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jun  9 15:26:31 2017
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

#ifndef _XPROJECTWIZARD_HPP
#define _XPROJECTWIZARD_HPP          1

#include <QWizard>

class QLabel;
class QLineEdit;
class QComboBox;
class XanteProject;

class IntroPage : public QWizardPage
{
    Q_OBJECT

    public:
        IntroPage(QWidget *parent = 0);

    private:
        QLabel *label;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

    public:
        ConclusionPage(QWidget *parent = 0);

    protected:
        void initializePage();

    private:
        QLabel *label;
};

class ProjectInfoPage : public QWizardPage
{
    Q_OBJECT

    public:
        ProjectInfoPage(QWidget *parent = 0);

    protected slots:
        void chooseDir(void);

    private:
        /* UI */
        QLineEdit *leName, *leDescription, *lePathname, *leAuthor;
        QComboBox *cbLanguage;
};

class ProjectConfigPage : public QWizardPage
{
    Q_OBJECT

    public:
        ProjectConfigPage(QWidget *parent = 0);

    private:
        /* UI */
        QLineEdit *leCompany, *lePlugin, *leCfgPath, *leLogPath,
                  *leVersion, *leRevision, *leBuild;

        QComboBox *cbBeta;
};

class XProjectWizard : public QWizard
{
    Q_OBJECT

    public:
        XProjectWizard(QWidget *parent = 0);
        ~XProjectWizard();
        void accept();
        XanteProject *buildProject(void);

    private:
        bool releaseProject = true;
        XanteProject *project = nullptr;

        void callSourceTemplate();
};

#endif

