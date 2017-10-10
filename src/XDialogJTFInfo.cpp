
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Jun 13 16:26:02 2017
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

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

#include "xante_builder.hpp"

XDialogJTFInfo::XDialogJTFInfo(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("JTF file main informations"));
    createWidgets();
    fillWidgetsWithProjectData();
}

QHBoxLayout *XDialogJTFInfo::createMainMenuChooser(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QLabel *l = new QLabel(tr("Main menu:"));
    cbMainMenu = new QComboBox;
    l->setBuddy(cbMainMenu);
    h->addWidget(l);
    h->addWidget(cbMainMenu);

    return h;
}

QHBoxLayout *XDialogJTFInfo::createButtons(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QPushButton *btnOk = new QPushButton(tr("Ok"));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(confirmOk()));

    QPushButton *btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(confirmCancel()));

    h->addWidget(btnOk);
    h->addWidget(btnCancel);

    return h;
}

QHBoxLayout *XDialogJTFInfo::createBetaChooser(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QLabel *l = new QLabel(tr("Beta:"));
    cbBeta = new QComboBox;
    l->setBuddy(cbBeta);
    cbBeta->addItem(tr("Yes"));
    cbBeta->addItem(tr("No"));
    h->addWidget(l);
    h->addWidget(cbBeta);

    return h;
}

QGroupBox *XDialogJTFInfo::createInformationWidgets(void)
{
    enum validatorType {
        VLD_STRING,
        VLD_INT,
        VLD_FLOAT
    };

    struct item {
        QString             label;
        enum validatorType validator;
    };

    struct item *p, pageItems[] = {
        { tr("Application name:"),      VLD_STRING },
        { tr("Description:"),           VLD_STRING },
        { tr("Company name:"),          VLD_STRING },
        { tr("Plugin name:"),           VLD_STRING },
        { tr("Config file directoty:"), VLD_STRING },
        { tr("Log file directory:"),    VLD_STRING },
        { tr("Version:"),               VLD_FLOAT  },
        { tr("Revision:"),              VLD_INT    },
        { tr("Build:"),                 VLD_INT    }
    };

    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *gb = new QGroupBox(tr("JTF Informations"));
    int i, t;

    t = sizeof(pageItems) / sizeof(pageItems[0]);

    for (i = 0; i < t; i++) {
        p = &pageItems[i];
        QLabel *l = new QLabel(p->label);
        QLineEdit *le = new QLineEdit;
        l->setBuddy(le);

        if (p->validator == VLD_INT)
            le->setValidator(new QIntValidator(le));
        else if (p->validator == VLD_FLOAT)
            le->setValidator(new QDoubleValidator(le));

        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(l);
        h->addWidget(le);

        v->addLayout(h);
        edit.append(le);
    }

    v->addLayout(createBetaChooser());
    gb->setLayout(v);

    return gb;
}

void XDialogJTFInfo::createWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;

    v->addWidget(createInformationWidgets());
    v->addLayout(createMainMenuChooser());
    v->addLayout(createButtons());

    setLayout(v);
}

void XDialogJTFInfo::confirmOk(void)
{
    /*
     * TODO
     *
     * 1 - Validate all widgets content.
     * 2 - Update the JTF object.
     */

    done(0);
}

void XDialogJTFInfo::confirmCancel(void)
{
    done(-1);
}

void XDialogJTFInfo::reject(void)
{
    confirmCancel();
}

void XDialogJTFInfo::fillWidgetsWithProjectData(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF jtf = project.getJtf();

    /* LineEdit */
    edit.at(XDialogJTFInfo::ApplicationName)->setText(jtf.getApplicationName());
    edit.at(XDialogJTFInfo::Description)->setText(jtf.getDescription());;
    edit.at(XDialogJTFInfo::Company)->setText(jtf.getCompany());
    edit.at(XDialogJTFInfo::Plugin)->setText(jtf.getPlugin());
    edit.at(XDialogJTFInfo::CfgPathname)->setText(jtf.getCfgPathname());
    edit.at(XDialogJTFInfo::LogPathname)->setText(jtf.getLogPathname());
    edit.at(XDialogJTFInfo::Version)->setText(jtf.getVersion());
    edit.at(XDialogJTFInfo::Revision)->setText(QString("%1").arg(jtf.getRevision()));
    edit.at(XDialogJTFInfo::Build)->setText(QString("%1").arg(jtf.getBuild()));

    /* Beta ComboBox */
    cbBeta->setCurrentIndex(jtf.getBeta() == true ? 0 : 1);

    /* Main menu chooser */
    prepareMainMenuChooser();
}

void XDialogJTFInfo::prepareMainMenuChooser(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF jtf = project.getJtf();
    int i;

    /* Insert all available menus */
    for (i = 0; i < jtf.totalMenus(); i++) {
        XanteMenu menu = jtf.menuAt(i);
        cbMainMenu->addItem(menu.getName());
    }

    /* Set current option to the current main menu */
    XanteMenu menu = jtf.getMenu(jtf.getMainMenu());
    cbMainMenu->setCurrentIndex(cbMainMenu->findText(menu.getName()));
}

