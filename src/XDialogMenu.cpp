
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
                *h2 = new QHBoxLayout,
                *h3 = new QHBoxLayout;

    label = new QLabel(tr("Block prefix:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h1->addWidget(label);
    h1->addWidget(edit);
    line_edit[XDialogMenu::LineEdit::BlockPrefix] = edit;

    label = new QLabel(tr("Block origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h2->addWidget(label);
    h2->addWidget(edit);
    line_edit[XDialogMenu::LineEdit::DynamicOriginBlock] = edit;

    label = new QLabel(tr("Item origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h3->addWidget(label);
    h3->addWidget(edit);
    line_edit[XDialogMenu::LineEdit::DynamicOriginItem] = edit;

    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
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
        radio_button[i] = rb;
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
    radio_button = QVector<QRadioButton *>(XanteMenu::MaxDynamicMenuType);

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

/*
 * Sets the current project that's been edited, so all other informations
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteMenu to the @selected_menu_index inside it.
 */
void XDialogMenu::set_current_project(int selected_menu_index)
{
    set_selection(selected_menu_index);
}

/*
 * Sets data by using the @selected_menu_index to get its corresponding
 * XanteMenu.
 */
void XDialogMenu::set_selection(int selected_menu_index)
{
    current_menu_index = selected_menu_index;
    setup_widgets();
}

void XDialogMenu::setup_widgets(void)
{
    XanteProject &project = XMainWindow::get_project();

    XanteJTF jtf = project.get_jtf();
    XanteMenu menu = jtf.menu_at(current_menu_index);

    setup_widgets(menu);
}

/*
 * Adjusts current selected XanteMenu dynamic informations in the UI if it
 * contains them.
 */
void XDialogMenu::setup_dynamic_info_widgets(XanteMenu menu)
{
    enum XanteMenu::DynamicType type = menu.get_dynamic_type();

    /*
     * If we're dealing with a default menu or an unsupported dynamic menu
     * type, abort.
     */
    if ((menu.get_type() != XanteMenu::Type::Dynamic) ||
        (type >= XanteMenu::DynamicType::MaxDynamicMenuType))
    {
        return;
    }

    radio_button[type]->setChecked(true);

    switch (menu.get_dynamic_type()) {
        case XanteMenu::DynamicType::FixedSize:
            line_edit[XDialogMenu::LineEdit::DynamicNumberOfCopies]->setText(
                QString("%1").arg(menu.get_dynamic_copies())
            );

            break;

        case XanteMenu::DynamicType::FixedOptions:
            dynamic_options->addItems(menu.get_dynamic_options());
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            line_edit[XDialogMenu::LineEdit::BlockPrefix]->setText(
                QString("%1").arg(menu.get_block_prefix())
            );

            line_edit[XDialogMenu::LineEdit::DynamicOriginBlock]->setText(
                QString("%1").arg(menu.get_dynamic_origin_block())
            );

            line_edit[XDialogMenu::LineEdit::DynamicOriginItem]->setText(
                QString("%1").arg(menu.get_dynamic_origin_item())
            );

            break;

        default:
            break;
    }
}

/*
 * Adjusts current selected XanteMenu events information in the UI.
 */
void XDialogMenu::setup_events_widgets(XanteMenu menu)
{
    int i;
    QList<QPair<enum XanteMenu::Event,
                QPair<enum XDialogMenu::LineEdit,
                      enum XDialogMenu::CheckBox>>> events;

    if (menu.has_events() == false)
        return;

    events.append(qMakePair(XanteMenu::Event::Selected,
                            qMakePair(XDialogMenu::LineEdit::EventSelected,
                                      XDialogMenu::CheckBox::EvSelected)));

    events.append(qMakePair(XanteMenu::Event::Exit,
                            qMakePair(XDialogMenu::LineEdit::EventExit,
                                      XDialogMenu::CheckBox::EvExit)));

    group_box[XDialogMenu::GroupBox::Events]->setChecked(true);
    group_box[XDialogMenu::GroupBox::Events]->setEnabled(true);

    for (i = 0; i < events.size(); i++) {
        QPair<enum XanteMenu::Event,
              QPair<enum XDialogMenu::LineEdit,
                    enum XDialogMenu::CheckBox>> p = events.at(i);

        QString event_name = menu.get_event(p.first);
        QPair<enum XDialogMenu::LineEdit,
              enum XDialogMenu::CheckBox> pp = p.second;

        if (event_name.isEmpty() == false) {
            line_edit[pp.first]->setText(event_name);
            check_box[pp.second]->setChecked(true);
        }
    }
}

void XDialogMenu::disable_all_widgets(void)
{
    group_box[XDialogMenu::GroupBox::Events]->setChecked(false);
    group_box[XDialogMenu::GroupBox::Events]->setEnabled(false);
    group_box[XDialogMenu::GroupBox::Dynamic]->setEnabled(false);
}

void XDialogMenu::setup_widgets(XanteMenu menu)
{
    disable_all_widgets();

    /* Enable/Disable dynamics groupbox */
    select_menu_type((menu.get_type() == XanteMenu::Type::Default) ? 0 : 1);

    line_edit[XDialogMenu::LineEdit::Name]->setText(menu.get_name());
    line_edit[XDialogMenu::LineEdit::ObjectId]->setText(menu.get_object_id());
    combo_box[XDialogMenu::ComboBox::Type]->setCurrentIndex((int)menu.get_type());
    combo_box[XDialogMenu::ComboBox::Mode]->setCurrentIndex((int)menu.get_mode());

    setup_dynamic_info_widgets(menu);
    setup_events_widgets(menu);
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
//    if (event->spontaneous() == false)
        /* TODO: Save content with current data */
//        XanteJTF jtf = project->get_jtf();
//        XanteMenu menu = jtf.menu_at(current_menu_index);
//    }

    event->accept();
}

void XDialogMenu::clear(void)
{
    for (int i = XDialogMenu::LineEdit::Name;
         i < XDialogMenu::LineEdit::MaxLineEdit;
         i++)
    {
        line_edit[i]->clear();
    }

    for (int i = XDialogMenu::ComboBox::Type;
         i < XDialogMenu::ComboBox::MaxComboBox;
         i++)
    {
        combo_box[i]->setCurrentIndex(0);
    }

    for (int i = XDialogMenu::CheckBox::EvSelected;
         i < XDialogMenu::CheckBox::MaxCheckBox;
         i++)
    {
        check_box[i]->setChecked(false);
    }
}

