
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
#include <QMap>
#include <QPair>
#include <QVector>
#include <QMapIterator>
#include <QInputDialog>

#include "xante_builder.hpp"

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
    "input-time",
    "input-string",
    "input-passwd",
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
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h = new QHBoxLayout;

    /* name */
    label = new QLabel(tr("Name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    line_edit[XDialogItem::LineEdit::Name] = edit;
    h->addWidget(label);
    h->addWidget(edit);

    /* object_id */
    label = new QLabel(tr("Object ID:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    edit->setEnabled(false);
    line_edit[XDialogItem::LineEdit::ObjectId] = edit;
    h->addWidget(label);
    h->addWidget(edit);

    return h;
}

QHBoxLayout *XDialogItem::create_type_widgets(void)
{
    QLabel *label;
    QComboBox *cb;
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Item type:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 1; i < ITEM_TYPE; i++)
        cb->addItem(QString(cb_item_type_name[i]));

    h->addWidget(label);
    h->addWidget(cb);
    connect(cb, SIGNAL(activated(int)), this, SLOT(select_item_type(int)));
    combo_box[XDialogItem::ComboBox::Type] = cb;

    /* mode */
    label = new QLabel(tr("Access mode:"));
    cb = new QComboBox;

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb ->addItem(QString(cb_access_mode_name[i]));

    h->addWidget(label);
    h->addWidget(cb);
    combo_box[XDialogItem::ComboBox::Mode] = cb;

    /* menu_id */
    label = new QLabel(tr("Referenced menu:"));
    cb = new QComboBox;

    h->addWidget(label);
    h->addWidget(cb);
    combo_box[XDialogItem::ComboBox::MenuReference] = cb;

    return h;
}

QGroupBox *XDialogItem::create_item_configuration_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QGroupBox *g = new QGroupBox(tr("Configuration"));
    QHBoxLayout *h = new QHBoxLayout;

    /* default value */
    label = new QLabel(tr("Default item value:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::DefaultValue] = edit;

    label = new QLabel(tr("Block:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::CfgBlock] = edit;

    label = new QLabel(tr("Item:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::CfgItem] = edit;

    g->setLayout(h);
    group_box[XDialogItem::GroupBox::Config] = g;

    return g;
}

QGroupBox *XDialogItem::create_item_options_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QListWidget *lwidget;
    QGroupBox *g = new QGroupBox(tr("Options"));
    QVBoxLayout *vbuttons = new QVBoxLayout,
                *v = new QVBoxLayout;;
    QHBoxLayout *hdescription = new QHBoxLayout,
                *hbuttons = new QHBoxLayout;

    QGroupBox *lo = new QGroupBox(tr("List:"));
    label = new QLabel(tr("Description:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    hdescription->addWidget(label);
    hdescription->addWidget(edit);
    line_edit[XDialogItem::LineEdit::Options] = edit;

    QPushButton *bt_add = new QPushButton(tr("Add"));
    QPushButton *bt_del = new QPushButton(tr("Remove"));
    vbuttons->addWidget(bt_add, 0, Qt::AlignBottom);
    vbuttons->addWidget(bt_del, 0, Qt::AlignTop);
    connect(bt_add, SIGNAL(clicked()), this, SLOT(add_option()));
    connect(bt_del, SIGNAL(clicked()), this, SLOT(del_option()));

    lwidget = new QListWidget;
    hbuttons->addWidget(lwidget);
    hbuttons->addLayout(vbuttons);
    lo->setLayout(hbuttons);
    list_widget[XDialogItem::ListWidget::OptionsLw] = lwidget;
    group_box[XDialogItem::GroupBox::ListOptions] = lo;

    v->addLayout(hdescription);
//    v->addLayout(hbuttons);
    v->addWidget(lo);
    g->setLayout(v);
    group_box[XDialogItem::GroupBox::OptionsGb] = g;

    return g;
}

QGroupBox *XDialogItem::create_item_help_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QListWidget *lwidget;
    QGroupBox *g = new QGroupBox(tr("Help details"));
    QVBoxLayout *v = new QVBoxLayout,
                *vbuttons = new QVBoxLayout;
    QHBoxLayout *hbuttons = new QHBoxLayout,
                *hdescription = new QHBoxLayout,
                *hbrief = new QHBoxLayout;

    label = new QLabel(tr("Brief:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    hbrief->addWidget(label);
    hbrief->addWidget(edit);
    line_edit[XDialogItem::LineEdit::HelpBrief] = edit;

    label = new QLabel(tr("Description:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    hdescription->addWidget(label);
    hdescription->addWidget(edit);
    line_edit[XDialogItem::LineEdit::HelpDescription] = edit;

    QPushButton *bt_add = new QPushButton(tr("Add"));
    QPushButton *bt_del = new QPushButton(tr("Remove"));
    connect(bt_add, SIGNAL(clicked()), this, SLOT(add_option_help()));
    connect(bt_del, SIGNAL(clicked()), this, SLOT(del_option_help()));
    vbuttons->addWidget(bt_add, 0, Qt::AlignBottom);
    vbuttons->addWidget(bt_del, 0, Qt::AlignTop);

    lwidget = new QListWidget;
    hbuttons->addWidget(lwidget);
    hbuttons->addLayout(vbuttons);
    list_widget[XDialogItem::ListWidget::HelpOptions] = lwidget;

    v->addLayout(hbrief);
    v->addLayout(hdescription);
    v->addLayout(hbuttons);
    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    group_box[XDialogItem::GroupBox::Help] = g;

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
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h;
    QGroupBox *g = new QGroupBox(tr("Input ranges"));
    QVBoxLayout *v = new QVBoxLayout;

    /* String length */
    h = new QHBoxLayout;
    label = new QLabel(tr("String length:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::InputStringLength] = edit;
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    label = new QLabel(tr("Min:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::InputMin] = edit;
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    label = new QLabel(tr("Max:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    line_edit[XDialogItem::LineEdit::InputMax] = edit;
    v->addLayout(h);

    g->setLayout(v);
    group_box[XDialogItem::GroupBox::InputRanges] = g;

    return g;
}

QGroupBox *XDialogItem::create_events_widgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QCheckBox *cb;
    QHBoxLayout *h;
    QMap<QString, QPair<enum XDialogItem::LineEdit, enum XDialogItem::CheckBox>> events_label;
    QGroupBox *g = new QGroupBox(tr("Events"));
    QVBoxLayout *v = new QVBoxLayout;

    events_label.insert("Selected",
                        qMakePair(XDialogItem::LineEdit::EventSelected,
                                  XDialogItem::CheckBox::EvSelected));

    events_label.insert("Value confirmed",
                        qMakePair(XDialogItem::LineEdit::EventValueConfirmed,
                                  XDialogItem::CheckBox::EvValueConfirmed));

    events_label.insert("Value changed",
                        qMakePair(XDialogItem::LineEdit::EventValueChanged,
                                  XDialogItem::CheckBox::EvValueChanged));

    events_label.insert("Exit",
                        qMakePair(XDialogItem::LineEdit::EventExit,
                                  XDialogItem::CheckBox::EvExit));

    QMapIterator<QString, QPair<enum XDialogItem::LineEdit, enum XDialogItem::CheckBox>> i(events_label);

    while (i.hasNext()) {
        i.next();
        QPair<enum XDialogItem::LineEdit, enum XDialogItem::CheckBox> pair;

        h = new QHBoxLayout;
        cb = new QCheckBox(i.key());
        label = new QLabel(tr("Function name:"));
        edit = new QLineEdit;
        label->setBuddy(edit);
        h->addWidget(cb);
        h->addWidget(label);
        h->addWidget(edit);

        pair = i.value();
        line_edit[pair.first] = edit;
        check_box[pair.second] = cb;

        v->addLayout(h);
    }

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    group_box[XDialogItem::GroupBox::Events] = g;

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
    line_edit = QVector<QLineEdit *>(XDialogItem::MaxLineEdit);
    check_box = QVector<QCheckBox *>(XDialogItem::MaxCheckBox);
    group_box = QVector<QGroupBox *>(XDialogItem::MaxGroupBox);
    combo_box = QVector<QComboBox *>(XDialogItem::MaxComboBox);
    list_widget = QVector<QListWidget *>(XDialogItem::MaxListWidget);

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

/*
 * Sets the current project that's been edited, so all other informations
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteItem to the @selected_menu_index and the
 * @selected_item_index inside it.
 */
void XDialogItem::set_current_project(XanteProject *project,
    int selected_menu_index, int selected_item_index)
{
    this->project = project;
    set_selection(selected_menu_index, selected_item_index);
}

/*
 * Sets data by using the @selected_menu_index to get its corresponding
 * XanteItem.
 */
void XDialogItem::set_selection(int selected_menu_index, int selected_item_index)
{
    current_menu_index = selected_menu_index;
    current_item_index = selected_item_index;
    setup_widgets();
}

void XDialogItem::setup_widgets(void)
{
    XanteJTF jtf = project->get_jtf();
    XanteMenu menu = jtf.menu_at(current_menu_index);
    XanteItem item = menu.item_at(current_item_index);

    setup_widgets(item);
}

void XDialogItem::setup_config_widgets(XanteItem item)
{
    if (item.has_config() == false)
        return;

    line_edit[XDialogItem::LineEdit::CfgBlock]->setText(item.get_config_block());
    line_edit[XDialogItem::LineEdit::CfgItem]->setText(item.get_config_item());
}

void XDialogItem::setup_events_widgets(XanteItem item)
{
    int i;
    QList<QPair<enum XanteItem::Event,
                QPair<enum XDialogItem::LineEdit,
                      enum XDialogItem::CheckBox>>> events;

    if (item.has_events() == false)
        return;

    events.append(qMakePair(XanteItem::Event::Selected,
                            qMakePair(XDialogItem::LineEdit::EventSelected,
                                      XDialogItem::CheckBox::EvSelected)));

    events.append(qMakePair(XanteItem::Event::Exit,
                            qMakePair(XDialogItem::LineEdit::EventExit,
                                      XDialogItem::CheckBox::EvExit)));

    events.append(qMakePair(XanteItem::Event::ValueConfirmed,
                            qMakePair(XDialogItem::LineEdit::EventValueConfirmed,
                                      XDialogItem::CheckBox::EvValueConfirmed)));

    events.append(qMakePair(XanteItem::Event::ValueChanged,
                            qMakePair(XDialogItem::LineEdit::EventValueChanged,
                                      XDialogItem::CheckBox::EvValueChanged)));

    group_box[XDialogItem::GroupBox::Events]->setChecked(true);

    for (i = 0; i < events.size(); i++) {
        QPair<enum XanteItem::Event,
              QPair<enum XDialogItem::LineEdit,
                    enum XDialogItem::CheckBox>> p = events.at(i);

        QString event_name = item.get_event(p.first);
        QPair<enum XDialogItem::LineEdit,
              enum XDialogItem::CheckBox> pp = p.second;

        if (event_name.isEmpty() == false) {
            line_edit[pp.first]->setText(event_name);
            check_box[pp.second]->setChecked(true);
        }
    }
}

void XDialogItem::setup_help_widgets(XanteItem item)
{
}

void XDialogItem::setup_input_ranges_widgets(XanteItem item)
{
    if (item.has_input_ranges() == false)
        return;
}

void XDialogItem::setup_options_widgets(XanteItem item)
{
    if (item.has_options() == false)
        return;
}

void XDialogItem::setup_widgets(XanteItem item)
{
    /* Prepare dialog widgets for item */
    select_item_type(item.get_type());

    /* Common informations */
    line_edit[XDialogItem::LineEdit::Name]->setText(item.get_name());
    line_edit[XDialogItem::LineEdit::ObjectId]->setText(item.get_object_id());
    combo_box[XDialogItem::ComboBox::Type]->setCurrentIndex((int)item.get_type());
    combo_box[XDialogItem::ComboBox::Mode]->setCurrentIndex((int)item.get_mode());

    setup_config_widgets(item);
    setup_events_widgets(item);
    setup_help_widgets(item);
    setup_input_ranges_widgets(item);
    setup_options_widgets(item);
}

void XDialogItem::disable_all_widgets(void)
{
    combo_box[XDialogItem::ComboBox::MenuReference]->setEnabled(false);
    group_box[XDialogItem::GroupBox::Config]->setEnabled(false);
    group_box[XDialogItem::GroupBox::OptionsGb]->setEnabled(false);
    group_box[XDialogItem::GroupBox::InputRanges]->setEnabled(false);
}

void XDialogItem::enable_input_ranges(int type)
{
    bool input_string = false, input_min = false, input_max = false;

    if ((type != XanteItem::Type::InputInt) &&
        (type != XanteItem::Type::InputFloat) &&
        (type != XanteItem::Type::InputString) &&
        (type != XanteItem::Type::InputPasswd))
    {
        return;
    }

    group_box[XDialogItem::GroupBox::InputRanges]->setEnabled(true);

    if ((type == XanteItem::Type::InputInt) ||
        (type == XanteItem::Type::InputFloat))
    {
        input_min = true;
        input_max = true;
    } else
        input_string = true;

    line_edit[XDialogItem::LineEdit::InputStringLength]->setEnabled(input_string);
    line_edit[XDialogItem::LineEdit::InputMin]->setEnabled(input_min);
    line_edit[XDialogItem::LineEdit::InputMax]->setEnabled(input_max);
}

void XDialogItem::enable_options(int type)
{
    bool list_options = false, line_options = false;

    if ((type == XanteItem::Type::Checklist) ||
        (type == XanteItem::Type::RadioChecklist))
    {
        list_options = true;
    } else
        line_options = true;

    group_box[XDialogItem::GroupBox::OptionsGb]->setEnabled(true);
    line_edit[XDialogItem::LineEdit::Options]->setEnabled(line_options);
    group_box[XDialogItem::GroupBox::ListOptions]->setEnabled(list_options);
}

void XDialogItem::select_item_type(int index)
{
    /* Adjusts current widgets according to the selected item type */
    enum XanteItem::Type type = (enum XanteItem::Type)index;

    disable_all_widgets();

    switch (type) {
        case XanteItem::Type::Menu:
            combo_box[XDialogItem::ComboBox::MenuReference]->setEnabled(true);
            break;

        case XanteItem::Type::InputInt:
        case XanteItem::Type::InputFloat:
        case XanteItem::Type::InputDate:
        case XanteItem::Type::InputTime:
        case XanteItem::Type::InputString:
        case XanteItem::Type::InputPasswd:
        case XanteItem::Type::Calendar:
        case XanteItem::Type::Timebox:
        case XanteItem::Type::YesNo:
        case XanteItem::Type::Checklist:
        case XanteItem::Type::RadioChecklist:
            group_box[XDialogItem::GroupBox::Config]->setEnabled(true);
            enable_input_ranges(type);
            enable_options(type);
            break;

        default:
            break;
    }
}

void XDialogItem::add_option(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New option"),
                                           tr("Enter the new option name:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    list_widget[XDialogItem::ListWidget::OptionsLw]->addItem(option);
}

void XDialogItem::del_option(void)
{
    QListWidget *l = list_widget[XDialogItem::ListWidget::OptionsLw];
    int row = l->currentRow();
}

void XDialogItem::hideEvent(QHideEvent *event)
{
    /* TODO: Save the current item modifications */
}

void XDialogItem::add_option_help(void)
{
}

void XDialogItem::del_option_help(void)
{
}

