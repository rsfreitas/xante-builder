
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Jun 12 23:13:42 2017
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

static const char *cb_item_event_name[] = {
    "selected",
    "value_confirmed",
    "value_changed",
    "exit"
};

#define ITEM_EVENT      \
    (sizeof(cb_item_event_name) / sizeof(cb_item_event_name[0]))

static const char *cb_access_mode_name[] = {
    "hidden",
    "view",
    "edit"
};

#define ACCESS_MODE     \
    (sizeof(cb_access_mode_name) / sizeof(cb_access_mode_name[0]))

static const char *cb_item_type_name[] = {
    NULL,
    "menu",
    "input-int",
    "input-float",
    "input-date",
    "input-string",
    "input-passwd",
    "input-time",
    "calendar",
    "timebox",
    "radio-checklist",
    "checklist",
    "yesno",
    "dynamic-menu",
    "delete-dynamic-menu",
    "add-dynamic-menu"
};

#define ITEM_TYPE       \
    (sizeof(cb_item_type_name) / sizeof(cb_item_type_name[0]))

QHBoxLayout *XDialogItem::create_identification_widgets(void)
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

QHBoxLayout *XDialogItem::create_type_widgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    QLabel *ltype = new QLabel(tr("Item type:"));
    cb_type = new QComboBox;

    for (unsigned int i = 1; i < ITEM_TYPE; i++)
        cb_type->addItem(QString(cb_item_type_name[i]));

    /* mode */
    QLabel *lmode = new QLabel(tr("Access mode:"));
    cb_mode = new QComboBox;

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb_mode->addItem(QString(cb_access_mode_name[i]));

    /* menu_id */
    QLabel *lmenu = new QLabel(tr("Referenced menu:"));
    cb_menu_id = new QComboBox;

    h->addWidget(ltype);
    h->addWidget(cb_type);
    h->addWidget(lmode);
    h->addWidget(cb_mode);
    h->addWidget(lmenu);
    h->addWidget(cb_menu_id);

    return h;
}

QGroupBox *XDialogItem::create_item_configuration_widgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Configuration"));
    QHBoxLayout *h = new QHBoxLayout;

    /* default value */
    QLabel *lvalue = new QLabel(tr("Default item value:"));
    le_default_value = new QLineEdit;
    lvalue->setBuddy(le_default_value);
    h->addWidget(lvalue);
    h->addWidget(le_default_value);

    QLabel *lblock = new QLabel(tr("Block:"));
    le_cfg_block = new QLineEdit;
    lblock->setBuddy(le_cfg_block);
    h->addWidget(lblock);
    h->addWidget(le_cfg_block);

    QLabel *litem = new QLabel(tr("Item:"));
    le_cfg_item = new QLineEdit;
    litem->setBuddy(le_cfg_item);
    h->addWidget(litem);
    h->addWidget(le_cfg_item);

    g->setLayout(h);

    return g;
}

QGroupBox *XDialogItem::create_item_options_widgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Options"));
    QVBoxLayout *vbuttons = new QVBoxLayout,
                *v = new QVBoxLayout;;
    QHBoxLayout *hdescription = new QHBoxLayout,
                *hbuttons = new QHBoxLayout;

    QLabel *ldescription = new QLabel(tr("Description:"));
    le_options = new QLineEdit;
    ldescription->setBuddy(le_options);
    hdescription->addWidget(ldescription);
    hdescription->addWidget(le_options);

    QPushButton *bt_add = new QPushButton(tr("Add"));
    QPushButton *bt_del = new QPushButton(tr("Remove"));
    vbuttons->addWidget(bt_add, 0, Qt::AlignBottom);
    vbuttons->addWidget(bt_del, 0, Qt::AlignTop);

    l_options = new QListWidget;
    hbuttons->addWidget(l_options);
    hbuttons->addLayout(vbuttons);

    v->addLayout(hdescription);
    v->addLayout(hbuttons);
    g->setLayout(v);

    return g;
}

QGroupBox *XDialogItem::create_item_help_widgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Help details"));
    QVBoxLayout *v = new QVBoxLayout,
                *vbuttons = new QVBoxLayout;
    QHBoxLayout *hbuttons = new QHBoxLayout,
                *hdescription = new QHBoxLayout,
                *hbrief = new QHBoxLayout;

    QLabel *lbrief = new QLabel(tr("Brief:"));
    le_help_brief = new QLineEdit;
    lbrief->setBuddy(le_help_brief);
    hbrief->addWidget(lbrief);
    hbrief->addWidget(le_help_brief);

    QLabel *ldescription = new QLabel(tr("Description:"));
    le_help_description = new QLineEdit;
    ldescription->setBuddy(le_help_description);
    hdescription->addWidget(ldescription);
    hdescription->addWidget(le_help_description);

    QPushButton *bt_add = new QPushButton(tr("Add"));
    QPushButton *bt_del = new QPushButton(tr("Remove"));
    vbuttons->addWidget(bt_add, 0, Qt::AlignBottom);
    vbuttons->addWidget(bt_del, 0, Qt::AlignTop);

    l_help_options = new QListWidget;
    hbuttons->addWidget(l_help_options);
    hbuttons->addLayout(vbuttons);

    v->addLayout(hbrief);
    v->addLayout(hdescription);
    v->addLayout(hbuttons);
    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);

    return g;
}

QHBoxLayout *XDialogItem::create_item_details_widgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    h->addWidget(create_item_options_widgets());
    h->addWidget(create_item_help_widgets());

    return h;
}

QGroupBox *XDialogItem::create_ranges_widgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Input ranges"));
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h;

    /* String length */
    h = new QHBoxLayout;
    QLabel *lstr = new QLabel(tr("String length:"));
    le_input_str_length = new QLineEdit;
    lstr->setBuddy(le_input_str_length);
    h->addWidget(lstr);
    h->addWidget(le_input_str_length);
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    QLabel *lmin = new QLabel(tr("Min:"));
    le_input_min = new QLineEdit;
    lmin->setBuddy(le_input_min);
    h->addWidget(lmin);
    h->addWidget(le_input_min);
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    QLabel *lmax = new QLabel(tr("Max:"));
    le_input_max = new QLineEdit;
    lmax->setBuddy(le_input_min);
    h->addWidget(lmax);
    h->addWidget(le_input_max);
    v->addLayout(h);

    g->setLayout(v);

    return g;
}

QGroupBox *XDialogItem::create_events_widgets(void)
{
    struct ev_widget_detail {
        const char  *name;
        QLineEdit   *le;
        QCheckBox   *cb;
    };

    struct ev_widget_detail *ev, events[] = {
        { "Selected", le_ev_selected, chb_ev_selected },
        { "Value confirmed", le_ev_value_confirmed, chb_ev_value_confirmed },
        { "Value changed", le_ev_value_changed, chb_ev_value_changed },
        { "Exit", le_ev_exit, chb_ev_exit }
    };

    int i, t;
    QGroupBox *g = new QGroupBox(tr("Events"));
    QVBoxLayout *v = new QVBoxLayout;

    t = sizeof(events) / sizeof(events[0]);

    for (i = 0; i < t; i++) {
        ev = &events[i];

        QHBoxLayout *h = new QHBoxLayout;
        ev->cb = new QCheckBox(QString(ev->name));
        QLabel *l = new QLabel(tr("Function name:"));
        ev->le = new QLineEdit;
        l->setBuddy(ev->le);
        h->addWidget(ev->cb);
        h->addWidget(l);
        h->addWidget(ev->le);

        v->addLayout(h);
    }

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);

    return g;
}

QHBoxLayout *XDialogItem::create_ranges_and_events_widgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    h->addWidget(create_ranges_widgets());
    h->addWidget(create_events_widgets());

    return h;
}

XDialogItem::XDialogItem(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;

    layout->addLayout(create_identification_widgets());
    layout->addLayout(create_type_widgets());
    layout->addWidget(create_item_configuration_widgets());
    layout->addLayout(create_item_details_widgets());
    layout->addLayout(create_ranges_and_events_widgets());

    setLayout(layout);
}

XDialogItem::~XDialogItem()
{
}

