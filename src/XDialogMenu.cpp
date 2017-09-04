
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
#include <QInputDialog>

#include "xante_builder.hpp"

static const char *cb_menu_type_name[] = {
    "default",
    "dynamic"
};

#define MENU_TYPE       \
    (sizeof(cb_menu_type_name) / sizeof(cb_menu_type_name[0]))

static const char *cb_access_mode_name[] = {
    "hidden",
    "view",
    "edit"
};

#define ACCESS_MODE     \
    (sizeof(cb_access_mode_name) / sizeof(cb_access_mode_name[0]))

QHBoxLayout *XDialogMenu::create_identification_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h = new QHBoxLayout;

    /* name */
    label = new QLabel(tr("Name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    line_edit[XDialogMenu::LineEdit::Name] = edit;
    h->addWidget(label);
    h->addWidget(line_edit[XDialogMenu::LineEdit::Name]);

    /* object_id */
    label = new QLabel(tr("Object ID:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    edit->setEnabled(false);
    line_edit[XDialogMenu::LineEdit::ObjectId] = edit;
    h->addWidget(label);
    h->addWidget(line_edit[XDialogMenu::LineEdit::ObjectId]);

    return h;
}

QHBoxLayout *XDialogMenu::create_type_widgets(void)
{
    QLabel *label;
    QComboBox *cb;
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Menu type:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < MENU_TYPE; i++)
        cb->addItem(QString(cb_menu_type_name[i]));

    h->addWidget(label);
    h->addWidget(cb);
    connect(cb, SIGNAL(activated(int)), this, SLOT(select_menu_type(int)));
    combo_box[XDialogMenu::ComboBox::Type] = cb;

    /* mode */
    label = new QLabel(tr("Access mode:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb->addItem(QString(cb_access_mode_name[i]));

    h->addWidget(label);
    h->addWidget(cb);
    combo_box[XDialogMenu::ComboBox::Mode] = cb;

    return h;
}

QGroupBox *XDialogMenu::create_events_widgets(void)
{
    QGroupBox *gb;
    QCheckBox *cb;
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h1, *h2;
    QVBoxLayout *v = new QVBoxLayout;

    h1 = new QHBoxLayout;
    h2 = new QHBoxLayout;
    gb = new QGroupBox(tr("Events"));
    gb->setCheckable(true);
    gb->setChecked(false);

    cb = new QCheckBox(tr("Selected event"));
    label = new QLabel(tr("Function name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h1->addWidget(cb);
    h1->addWidget(label);
    h1->addWidget(edit);
    check_box[XDialogMenu::CheckBox::EvSelected] = cb;
    line_edit[XDialogMenu::LineEdit::EventSelected] = edit;

    cb = new QCheckBox(tr("Exit event"));
    label = new QLabel(tr("Function name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h2->addWidget(cb);
    h2->addWidget(label);
    h2->addWidget(edit);
    check_box[XDialogMenu::CheckBox::EvExit] = cb;
    line_edit[XDialogMenu::LineEdit::EventExit] = edit;

    v->addLayout(h1);
    v->addLayout(h2);
    gb->setLayout(v);
    group_box[XDialogMenu::GroupBox::Events] = gb;

    return gb;
}

QGroupBox *XDialogMenu::create_dm_fixed_size_widgets(void)
{
    QLineEdit *edit;
    QGroupBox *gb = new QGroupBox;
    QHBoxLayout *h = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Number of copies:"));

    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    gb->setLayout(h);
    line_edit[XDialogMenu::LineEdit::DynamicNumberOfCopies] = edit;
    group_box[XDialogMenu::GroupBox::DynamicFixed] = gb;

    return gb;
}

QGroupBox *XDialogMenu::create_dm_fixed_options_widgets(void)
{
    QGroupBox *gb = new QGroupBox;
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;

    QPushButton *bt_add = new QPushButton(tr("Add"));
    connect(bt_add, SIGNAL(clicked()), this, SLOT(add_dynamic_fixed_option()));

    QPushButton *bt_del = new QPushButton(tr("Remove"));
    connect(bt_del, SIGNAL(clicked()), this, SLOT(del_dynamic_fixed_option()));

    v->addWidget(bt_add, 0, Qt::AlignBottom);
    v->addWidget(bt_del, 0, Qt::AlignTop);

    dynamic_options = new QListWidget;
    h->addWidget(dynamic_options);
    h->addLayout(v);

    gb->setLayout(h);
    group_box[XDialogMenu::GroupBox::DynamicOptions] = gb;

    return gb;
}

QGroupBox *XDialogMenu::create_dm_options_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QGroupBox *gb = new QGroupBox;
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h1 = new QHBoxLayout,
                *h2 = new QHBoxLayout;

    label = new QLabel(tr("Block origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h1->addWidget(label);
    h1->addWidget(edit);
    line_edit[XDialogMenu::LineEdit::DynamicOriginBlock] = edit;

    label = new QLabel(tr("Item origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h2->addWidget(label);
    h2->addWidget(edit);
    line_edit[XDialogMenu::LineEdit::DynamicOriginItem] = edit;

    v->addLayout(h1);
    v->addLayout(h2);
    gb->setLayout(v);
    group_box[XDialogMenu::GroupBox::DynamicConfig] = gb;

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

    /* Dynamic option properties */
    grid->addWidget(create_dm_fixed_size_widgets(), 1, 0, Qt::AlignTop);
    grid->addWidget(create_dm_fixed_options_widgets(), 1, 1);
    grid->addWidget(create_dm_options_widgets(), 1, 2, Qt::AlignTop);

    /* Radio buttons */
    for (int i = 0; i < XanteMenu::MaxDynamicMenuType; i++) {
        QRadioButton *rb = new QRadioButton(QString(option[i]));
        rb->setObjectName(QString("%1").arg(i));
        grid->addWidget(rb, 0, i);

        if (i == 0)
            rb->setChecked(true);
        else
            group_box[i]->setEnabled(false);

        connect(rb, &QRadioButton::toggled, this, &XDialogMenu::dynamic_radio_toggled);
    }

    QGroupBox *gb_dynamic = new QGroupBox(tr("Dynamic properties"));
    gb_dynamic->setLayout(grid);
    group_box[XDialogMenu::GroupBox::Dynamic] = gb_dynamic;

    return gb_dynamic;
}

XDialogMenu::XDialogMenu(QWidget *parent)
    : QWidget(parent)
{
    line_edit = QVector<QLineEdit *>(XDialogMenu::MaxLineEdit);
    check_box = QVector<QCheckBox *>(XDialogMenu::MaxCheckBox);
    combo_box = QVector<QComboBox *>(XDialogMenu::MaxComboBox);
    group_box = QVector<QGroupBox *>(XDialogMenu::MaxGroupBox);

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

void XDialogMenu::dynamic_radio_toggled(bool checked)
{
    if (checked == true) {
        QRadioButton *rb = static_cast<QRadioButton *>(sender());
        enum XanteMenu::DynamicType selected_button = (enum XanteMenu::DynamicType)rb->objectName().toInt();
        int i;

        for (i = 0; i < XanteMenu::MaxDynamicMenuType; i++)
            group_box[i]->setEnabled((selected_button == i) ? true : false);
    }
}

void XDialogMenu::set_current_project(XanteProject *project,
    int selected_menu_index)
{
    this->project = project;
    set_selection(selected_menu_index);
}

void XDialogMenu::set_selection(int selected_menu_index)
{
    current_menu_index = selected_menu_index;
    setup_widgets();
}

void XDialogMenu::setup_widgets(void)
{
    XanteJTF jtf = project->get_jtf();
    XanteMenu menu = jtf.menu_at(current_menu_index);

    setup_widgets(menu);
}

void XDialogMenu::setup_widgets(XanteMenu menu)
{
    /* Enable/Disable dynamics groupbox */
    select_menu_type((menu.get_type() == XanteMenu::Type::Default) ? 0 : 1);

    line_edit[XDialogMenu::LineEdit::Name]->setText(menu.get_name());
    line_edit[XDialogMenu::LineEdit::ObjectId]->setText(menu.get_object_id());
    combo_box[XDialogMenu::ComboBox::Type]->setCurrentIndex((int)menu.get_type());
    combo_box[XDialogMenu::ComboBox::Mode]->setCurrentIndex((int)menu.get_mode());
}

void XDialogMenu::select_menu_type(int index)
{
    group_box[XDialogMenu::GroupBox::Dynamic]->setEnabled((index == 0) ? false
                                                                       : true);
}

void XDialogMenu::add_dynamic_fixed_option(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New dynamic option"),
                                           tr("Enter the new dynamic option name:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    dynamic_options->addItem(option);
}

void XDialogMenu::del_dynamic_fixed_option(void)
{
    int row = dynamic_options->currentRow();

    if (row < 0)
        return;

    dynamic_options->takeItem(row);
}

void XDialogMenu::hideEvent(QHideEvent *event)
{
    if (event->spontaneous() == false) {
        /* TODO: Save content with current data */
        XanteJTF jtf = project->get_jtf();
        XanteMenu menu = jtf.menu_at(current_menu_index);
    }

    event->accept();
}

