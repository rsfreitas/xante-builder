
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
    create_widgets();
    fill_widgets_with_project_data();
}

QHBoxLayout *XDialogJTFInfo::create_main_menu_chooser(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QLabel *l = new QLabel(tr("Main menu:"));
    cb_main_menu = new QComboBox;
    l->setBuddy(cb_main_menu);
    h->addWidget(l);
    h->addWidget(cb_main_menu);

    return h;
}

QHBoxLayout *XDialogJTFInfo::create_buttons(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QPushButton *btn_ok = new QPushButton(tr("Ok"));
    connect(btn_ok, SIGNAL(clicked()), this, SLOT(confirm_ok()));

    QPushButton *btn_cancel = new QPushButton(tr("Cancel"));
    connect(btn_cancel, SIGNAL(clicked()), this, SLOT(confirm_cancel()));

    h->addWidget(btn_ok);
    h->addWidget(btn_cancel);

    return h;
}

QHBoxLayout *XDialogJTFInfo::create_beta_chooser(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QLabel *l = new QLabel(tr("Beta:"));
    cb_beta = new QComboBox;
    l->setBuddy(cb_beta);
    cb_beta->addItem(tr("Yes"));
    cb_beta->addItem(tr("No"));
    h->addWidget(l);
    h->addWidget(cb_beta);

    return h;
}

QGroupBox *XDialogJTFInfo::create_information_widgets(void)
{
    enum validator_type {
        VLD_STRING,
        VLD_INT,
        VLD_FLOAT
    };

    struct item {
        QString             label;
        enum validator_type validator;
    };

    struct item *p, page_items[] = {
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

    t = sizeof(page_items) / sizeof(page_items[0]);

    for (i = 0; i < t; i++) {
        p = &page_items[i];
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

    v->addLayout(create_beta_chooser());
    gb->setLayout(v);

    return gb;
}

void XDialogJTFInfo::create_widgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;

    v->addWidget(create_information_widgets());
    v->addLayout(create_main_menu_chooser());
    v->addLayout(create_buttons());

    setLayout(v);
}

void XDialogJTFInfo::confirm_ok(void)
{
    /*
     * TODO
     *
     * 1 - Validate all widgets content.
     * 2 - Update the JTF object.
     */

    done(0);
}

void XDialogJTFInfo::confirm_cancel(void)
{
    done(-1);
}

void XDialogJTFInfo::reject(void)
{
    confirm_cancel();
}

void XDialogJTFInfo::fill_widgets_with_project_data(void)
{
    XanteProject &project = XMainWindow::get_project();
    XanteJTF jtf = project.get_jtf();

    /* LineEdit */
    edit.at(XDialogJTFInfo::ApplicationName)->setText(jtf.get_application_name());
    edit.at(XDialogJTFInfo::Description)->setText(jtf.get_description());;
    edit.at(XDialogJTFInfo::Company)->setText(jtf.get_company());
    edit.at(XDialogJTFInfo::Plugin)->setText(jtf.get_plugin());
    edit.at(XDialogJTFInfo::CfgPathname)->setText(jtf.get_cfg_pathname());
    edit.at(XDialogJTFInfo::LogPathname)->setText(jtf.get_log_pathname());
    edit.at(XDialogJTFInfo::Version)->setText(jtf.get_version());
    edit.at(XDialogJTFInfo::Revision)->setText(QString("%1").arg(jtf.get_revision()));
    edit.at(XDialogJTFInfo::Build)->setText(QString("%1").arg(jtf.get_build()));

    /* Beta ComboBox */
    cb_beta->setCurrentIndex(jtf.get_beta() == true ? 0 : 1);

    /* Main menu chooser */
    prepare_main_menu_chooser();
}

void XDialogJTFInfo::prepare_main_menu_chooser(void)
{
    XanteProject &project = XMainWindow::get_project();
    XanteJTF jtf = project.get_jtf();
    int i;

    /* Insert all available menus */
    for (i = 0; i < jtf.total_menus(); i++) {
        XanteMenu menu = jtf.menu_at(i);
        cb_main_menu->addItem(menu.get_name());
    }

    /* Set current option to the current main menu */
    XanteMenu menu = jtf.get_menu(jtf.get_main_menu());
    cb_main_menu->setCurrentIndex(cb_main_menu->findText(menu.get_name()));
}

