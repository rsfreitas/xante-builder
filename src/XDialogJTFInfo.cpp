
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

XDialogJTFInfo::XDialogJTFInfo(XanteProject *project, QWidget *parent)
    : QDialog(parent), project(project)
{
    setWindowTitle(tr("JTF file main informations"));
    create_widgets();
}

XDialogJTFInfo::~XDialogJTFInfo()
{
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
    QPushButton *btn_cancel = new QPushButton(tr("Cancel"));
    h->addWidget(btn_ok);
    h->addWidget(btn_cancel);

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
        QLineEdit           *le;
        enum validator_type validator;
    };

    struct item *p, page_items[] = {
        { tr("Application name:"),      le_application_name,    VLD_STRING },
        { tr("Description:"),           le_description,         VLD_STRING },
        { tr("Company name:"),          le_company,             VLD_STRING },
        { tr("Plugin name:"),           le_plugin,              VLD_STRING },
        { tr("Config file directoty:"), le_cfg_pathname,        VLD_STRING },
        { tr("Log file directory:"),    le_log_pathname,        VLD_STRING },
        { tr("Version:"),               le_version,             VLD_FLOAT  },
        { tr("Revision:"),              le_revision,            VLD_INT    },
        { tr("Build:"),                 le_build,               VLD_INT    }
    };

    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *gb = new QGroupBox(tr("JTF Informations"));
    int i, t;

    t = sizeof(page_items) / sizeof(page_items[0]);

    for (i = 0; i < t; i++) {
        p = &page_items[i];
        QLabel *l = new QLabel(p->label);
        p->le = new QLineEdit;
        l->setBuddy(p->le);

        if (p->validator == VLD_INT)
            p->le->setValidator(new QIntValidator(p->le));
        else if (p->validator == VLD_FLOAT)
            p->le->setValidator(new QDoubleValidator(p->le));

        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(l);
        h->addWidget(p->le);

        v->addLayout(h);
    }

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

