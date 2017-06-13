
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:13:39 2017
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

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>

#include "xante_builder.hpp"

static const char *cb_menu_type_name[] = {
    "default",
    "dynamic"
};

#define MENU_TYPE       \
    (sizeof(cb_menu_type_name) / sizeof(cb_menu_type_name[0]))

static const char *cb_menu_event_name[] = {
    "selected",
    "exit"
};

#define MENU_EVENT      \
    (sizeof(cb_menu_event_name) / sizeof(cb_menu_event_name[0]))

static const char *cb_access_mode_name[] = {
    "hidden",
    "view",
    "edit"
};

#define ACCESS_MODE     \
    (sizeof(cb_access_mode_name) / sizeof(cb_access_mode_name[0]))

QHBoxLayout *XDialogMenu::create_identification_widgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* name */
    QLabel *lname = new QLabel(tr("Name:"));
    le_name = new QLineEdit;
    lname->setBuddy(le_name);

    /* object_id */
    QLabel *lobject_id = new QLabel(tr("Object ID:"));
    le_object_id = new QLineEdit;
    lobject_id->setBuddy(le_object_id);

    h->addWidget(lname);
    h->addWidget(le_name);
    h->addWidget(lobject_id);
    h->addWidget(le_object_id);

    return h;
}

QHBoxLayout *XDialogMenu::create_type_widgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    QLabel *ltype = new QLabel(tr("Menu type:"));
    cb_type = new QComboBox;

    for (unsigned int i = 0; i < MENU_TYPE; i++)
        cb_type->addItem(QString(cb_menu_type_name[i]));

    /* mode */
    QLabel *lmode = new QLabel(tr("Access mode:"));
    cb_mode = new QComboBox;

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb_mode->addItem(QString(cb_access_mode_name[i]));

    h->addWidget(ltype);
    h->addWidget(cb_type);
    h->addWidget(lmode);
    h->addWidget(cb_mode);

    return h;
}

QGroupBox *XDialogMenu::create_events_widgets(void)
{
    QHBoxLayout *h1, *h2;
    QVBoxLayout *v = new QVBoxLayout;

    h1 = new QHBoxLayout;
    h2 = new QHBoxLayout;
    gb_events = new QGroupBox(tr("Events"));
    gb_events->setCheckable(true);
    gb_events->setChecked(false);

    chb_ev_selected = new QCheckBox(tr("Selected event"));
    QLabel *lselected = new QLabel(tr("Function name:"));
    le_ev_selected = new QLineEdit;
    lselected->setBuddy(le_ev_selected);
    h1->addWidget(chb_ev_selected);
    h1->addWidget(lselected);
    h1->addWidget(le_ev_selected);

    chb_ev_exit = new QCheckBox(tr("Exit event"));
    QLabel *lexit = new QLabel(tr("Function name:"));
    le_ev_exit = new QLineEdit;
    lexit->setBuddy(le_ev_exit);
    h2->addWidget(chb_ev_exit);
    h2->addWidget(lexit);
    h2->addWidget(le_ev_exit);

    v->addLayout(h1);
    v->addLayout(h2);
    gb_events->setLayout(v);

    return gb_events;
}

QGroupBox *XDialogMenu::create_dm_fixed_size_widgets(void)
{
    QGroupBox *gb = new QGroupBox;
    QHBoxLayout *h = new QHBoxLayout;

    QLabel *l = new QLabel(tr("Number of copies:"));
    le_dm_copies = new QLineEdit;
    l->setBuddy(le_dm_copies);
    h->addWidget(l);
    h->addWidget(le_dm_copies);
    gb->setLayout(h);

    return gb;
}

QGroupBox *XDialogMenu::create_dm_fixed_options_widgets(void)
{
    QGroupBox *gb = new QGroupBox;
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;

    QPushButton *bt_add = new QPushButton(tr("Add"));
    QPushButton *bt_del = new QPushButton(tr("Remove"));
    v->addWidget(bt_add, 0, Qt::AlignBottom);
    v->addWidget(bt_del, 0, Qt::AlignTop);

    l_dm_options = new QListWidget;
    h->addWidget(l_dm_options);
    h->addLayout(v);

    gb->setLayout(h);

    return gb;
}

QGroupBox *XDialogMenu::create_dm_options_widgets(void)
{
    QGroupBox *gb = new QGroupBox;
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h1 = new QHBoxLayout,
                *h2 = new QHBoxLayout;

    QLabel *l1 = new QLabel(tr("Block origin:"));
    le_dm_origin_block = new QLineEdit;
    l1->setBuddy(le_dm_origin_block);
    h1->addWidget(l1);
    h1->addWidget(le_dm_origin_block);

    QLabel *l2 = new QLabel(tr("Item origin:"));
    le_dm_origin_item = new QLineEdit;
    l2->setBuddy(le_dm_origin_item);
    h2->addWidget(l2);
    h2->addWidget(le_dm_origin_item);

    v->addLayout(h1);
    v->addLayout(h2);
    gb->setLayout(v);

    return gb;
}

QGroupBox *XDialogMenu::create_dynamic_details_widgets(void)
{
    QGridLayout *grid = new QGridLayout;

    static const char *option[] = {
        "Fixed size",
        "Fixed options",
        "Dynamic options"
    };

    /* Radio buttons */
    for (int i = 0; i < XDialogMenu::MaxType; i++) {
        QRadioButton *rb = new QRadioButton(QString(option[i]));
        grid->addWidget(rb, 0, i);
    }

    /* Dynamic option properties */
    grid->addWidget(create_dm_fixed_size_widgets(), 1, 0, Qt::AlignTop);
    grid->addWidget(create_dm_fixed_options_widgets(), 1, 1);
    grid->addWidget(create_dm_options_widgets(), 1, 2, Qt::AlignTop);

    gb_dynamic = new QGroupBox(tr("Dynamic properties"));
    gb_dynamic->setLayout(grid);

    return gb_dynamic;
}

XDialogMenu::XDialogMenu(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addLayout(create_identification_widgets());
    layout->addLayout(create_type_widgets());
    layout->addWidget(create_events_widgets());
    layout->addWidget(create_dynamic_details_widgets());

    setLayout(layout);
}

XDialogMenu::~XDialogMenu()
{
}

void XDialogMenu::disable_all_widgets(void)
{
}

void XDialogMenu::onDmRadioToggled(bool checked)
{
    if (checked == true) {
        QRadioButton *rb = static_cast<QRadioButton *>(sender());
    }
}

