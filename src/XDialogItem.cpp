
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
#include <QAction>

#include "xante_builder.hpp"

static const char *cbAccessModeName[] = {
    "hidden",
    "view",
    "edit"
};

#define ACCESS_MODE     \
    (sizeof(cbAccessModeName) / sizeof(cbAccessModeName[0]))

static const char *cbItemTypeName[] = {
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
    (sizeof(cbItemTypeName) / sizeof(cbItemTypeName[0]))

QHBoxLayout *XDialogItem::createIdentificationWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h = new QHBoxLayout;

    /* name */
    label = new QLabel(tr("Name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    lineEdit[XDialogItem::LineEdit::Name] = edit;
    h->addWidget(label);
    h->addWidget(edit);

    /* objectId */
    label = new QLabel(tr("Object ID:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    edit->setEnabled(false);
    lineEdit[XDialogItem::LineEdit::ObjectId] = edit;
    h->addWidget(label);
    h->addWidget(edit);

    return h;
}

QHBoxLayout *XDialogItem::createTypeWidgets(void)
{
    QLabel *label;
    QComboBox *cb;
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Item type:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 1; i < ITEM_TYPE; i++)
        cb->addItem(QString(cbItemTypeName[i]));

    h->addWidget(label);
    h->addWidget(cb);
    connect(cb, SIGNAL(activated(int)), this, SLOT(selectItemType(int)));
    comboBox[XDialogItem::ComboBox::Type] = cb;

    /* mode */
    label = new QLabel(tr("Access mode:"));
    cb = new QComboBox;

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb ->addItem(QString(cbAccessModeName[i]));

    h->addWidget(label);
    h->addWidget(cb);
    comboBox[XDialogItem::ComboBox::Mode] = cb;

    /* menuId */
    label = new QLabel(tr("Referenced menu:"));
    cb = new QComboBox;

    h->addWidget(label);
    h->addWidget(cb);
    comboBox[XDialogItem::ComboBox::MenuReference] = cb;

    return h;
}

QGroupBox *XDialogItem::createItemConfigurationWidgets(void)
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
    lineEdit[XDialogItem::LineEdit::DefaultValue] = edit;

    label = new QLabel(tr("Block:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::CfgBlock] = edit;

    label = new QLabel(tr("Item:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::CfgItem] = edit;

    g->setLayout(h);
    groupBox[XDialogItem::GroupBox::Config] = g;

    return g;
}

QGroupBox *XDialogItem::createItemOptionsWidgets(void)
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
    lineEdit[XDialogItem::LineEdit::Options] = edit;

    QPushButton *btAdd = new QPushButton(tr("Add"));
    QPushButton *btDel = new QPushButton(tr("Remove"));
    vbuttons->addWidget(btAdd, 0, Qt::AlignBottom);
    vbuttons->addWidget(btDel, 0, Qt::AlignTop);
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addOption()));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delOption()));

    lwidget = new QListWidget;
    hbuttons->addWidget(lwidget);
    hbuttons->addLayout(vbuttons);
    lo->setLayout(hbuttons);
    listWidget[XDialogItem::ListWidget::OptionsLw] = lwidget;
    groupBox[XDialogItem::GroupBox::ListOptions] = lo;

    v->addLayout(hdescription);
    v->addWidget(lo);
    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::OptionsGb] = g;

    return g;
}

QGroupBox *XDialogItem::createItemHelpWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QListWidget *lwidget;
    QGroupBox *g = new QGroupBox(tr("Help details")),
              *gList = new QGroupBox();
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
    lineEdit[XDialogItem::LineEdit::HelpBrief] = edit;

    label = new QLabel(tr("Description:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    hdescription->addWidget(label);
    hdescription->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::HelpDescriptive] = edit;

    QPushButton *btAdd = new QPushButton(tr("Add"));
    QPushButton *btDel = new QPushButton(tr("Remove"));
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addOptionHelp()));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delOptionHelp()));
    vbuttons->addWidget(btAdd, 0, Qt::AlignBottom);
    vbuttons->addWidget(btDel, 0, Qt::AlignTop);

    lwidget = new QListWidget;
    hbuttons->addWidget(lwidget);
    hbuttons->addLayout(vbuttons);
    listWidget[XDialogItem::ListWidget::HelpOptions] = lwidget;
    gList->setLayout(hbuttons);
    groupBox[XDialogItem::GroupBox::ListHelpOptions] = gList;

    v->addLayout(hbrief);
    v->addLayout(hdescription);
    v->addWidget(gList);
    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    connect(g, SIGNAL(toggled(bool)), this, SLOT(helpGroupToggled(bool)));
    groupBox[XDialogItem::GroupBox::Help] = g;

    return g;
}

QHBoxLayout *XDialogItem::createItemDetailsWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    h->addWidget(createItemOptionsWidgets());
    h->addWidget(createItemHelpWidgets());

    return h;
}

QGroupBox *XDialogItem::createRangesWidgets(void)
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
    lineEdit[XDialogItem::LineEdit::InputStringLength] = edit;
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    label = new QLabel(tr("Min:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::InputMin] = edit;
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    label = new QLabel(tr("Max:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::InputMax] = edit;
    v->addLayout(h);

    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::InputRanges] = g;

    return g;
}

QGroupBox *XDialogItem::createEventsWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QCheckBox *cb;
    QHBoxLayout *h;
    QMap<QString, QPair<enum XDialogItem::LineEdit, enum XDialogItem::CheckBox>> eventsLabel;
    QGroupBox *g = new QGroupBox(tr("Events"));
    QVBoxLayout *v = new QVBoxLayout;

    eventsLabel.insert("Selected",
                        qMakePair(XDialogItem::LineEdit::EventSelected,
                                  XDialogItem::CheckBox::EvSelected));

    eventsLabel.insert("Value confirmed",
                        qMakePair(XDialogItem::LineEdit::EventValueConfirmed,
                                  XDialogItem::CheckBox::EvValueConfirmed));

    eventsLabel.insert("Value changed",
                        qMakePair(XDialogItem::LineEdit::EventValueChanged,
                                  XDialogItem::CheckBox::EvValueChanged));

    eventsLabel.insert("Exit",
                        qMakePair(XDialogItem::LineEdit::EventExit,
                                  XDialogItem::CheckBox::EvExit));

    QMapIterator<QString, QPair<enum XDialogItem::LineEdit, enum XDialogItem::CheckBox>> i(eventsLabel);

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
        lineEdit[pair.first] = edit;
        checkBox[pair.second] = cb;

        v->addLayout(h);
    }

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::Events] = g;

    return g;
}

QHBoxLayout *XDialogItem::createRangesAndEventsWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    h->addWidget(createRangesWidgets());
    h->addWidget(createEventsWidgets());

    return h;
}

XDialogItem::XDialogItem(QWidget *parent)
    : QWidget(parent)
{
    lineEdit = QVector<QLineEdit *>(XDialogItem::MaxLineEdit);
    checkBox = QVector<QCheckBox *>(XDialogItem::MaxCheckBox);
    groupBox = QVector<QGroupBox *>(XDialogItem::MaxGroupBox);
    comboBox = QVector<QComboBox *>(XDialogItem::MaxComboBox);
    listWidget = QVector<QListWidget *>(XDialogItem::MaxListWidget);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addLayout(createIdentificationWidgets());
    layout->addLayout(createTypeWidgets());
    layout->addWidget(createItemConfigurationWidgets());
    layout->addLayout(createItemDetailsWidgets());
    layout->addLayout(createRangesAndEventsWidgets());

    setLayout(layout);
}

XDialogItem::~XDialogItem()
{
}

/*
 * Sets the current project that is been edited, so all other informations
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteItem to the @selectedMenuIndex and the
 * @selectedItemIndex inside it.
 */
void XDialogItem::setCurrentProject(int selectedMenuIndex,
    int selectedItemIndex)
{
    setSelection(selectedMenuIndex, selectedItemIndex);
}

/*
 * Sets data by using the @selectedMenuIndex to get its corresponding
 * XanteItem.
 */
void XDialogItem::setSelection(int selectedMenuIndex, int selectedItemIndex)
{
    clear();
    currentMenuIndex = selectedMenuIndex;
    currentItemIndex = selectedItemIndex;
    setupWidgets();
}

void XDialogItem::setupConfigWidgets(const XanteItem &item)
{
    if (item.hasConfig() == false)
        return;

    lineEdit[XDialogItem::LineEdit::CfgBlock]->setText(item.configBlock());
    lineEdit[XDialogItem::LineEdit::CfgItem]->setText(item.configItem());
    lineEdit[XDialogItem::LineEdit::DefaultValue]->setText(item.defaultValue());
}

void XDialogItem::setupEventsWidgets(const XanteItem &item)
{
    int i;
    QList<QPair<enum XanteItem::Event,
                QPair<enum XDialogItem::LineEdit,
                      enum XDialogItem::CheckBox>>> events;

    if (item.hasEvents() == false)
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

    groupBox[XDialogItem::GroupBox::Events]->setChecked(true);

    for (i = 0; i < events.size(); i++) {
        QPair<enum XanteItem::Event,
              QPair<enum XDialogItem::LineEdit,
                    enum XDialogItem::CheckBox>> p = events.at(i);

        QString eventName = item.event(p.first);
        QPair<enum XDialogItem::LineEdit,
              enum XDialogItem::CheckBox> pp = p.second;

        if (eventName.isEmpty() == false) {
            lineEdit[pp.first]->setText(eventName);
            checkBox[pp.second]->setChecked(true);
        }
    }
}

void XDialogItem::setupHelpWidgets(const XanteItem &item)
{
    QString text;
    int t;
    bool hasHelp = false;

    if (item.hasHelp())
        hasHelp = true;

    /* brief help */
    text = item.briefHelp();

    if (text.isEmpty() == false)
        lineEdit[XDialogItem::LineEdit::HelpBrief]->setText(text);

    /* descriptive help */
    text = item.descriptiveHelp();

    if (text.isEmpty() == false)
        lineEdit[XDialogItem::LineEdit::HelpDescriptive]->setText(text);

    /* checklist/radio-checklist help options */
    t = item.totalHelpOptions();

    for (int i = 0; i < t; i++)
        listWidget[XDialogItem::ListWidget::HelpOptions]->addItem(item.helpOption(i));

    groupBox[XDialogItem::GroupBox::Help]->setChecked(hasHelp);
}

void XDialogItem::setupInputRangesWidgets(const XanteItem &item)
{
    if (item.hasInputRanges() == false)
        return;

    enum XanteItem::Type type = item.type();

    if (type == XanteItem::Type::InputString) {
        lineEdit[XDialogItem::LineEdit::InputStringLength]->setText(
            QString("%1").arg(item.stringLength())
        );
    } else {
        QPair<QVariant, QVariant> mm = item.minMax();

        lineEdit[XDialogItem::LineEdit::InputMin]->setText(
            QString("%1").arg((type == XanteItem::Type::InputInt)
                                    ? mm.first.toInt()
                                    : mm.first.toFloat())
        );

        lineEdit[XDialogItem::LineEdit::InputMax]->setText(
            QString("%1").arg((type == XanteItem::Type::InputInt)
                                    ? mm.second.toInt()
                                    : mm.second.toFloat())
        );
    }
}

void XDialogItem::setupOptionsWidgets(const XanteItem &item)
{
    if (item.hasOptions() == false)
        return;

    enum XanteItem::Type type = item.type();

    if ((type == XanteItem::Type::Checklist) ||
        (type == XanteItem::Type::RadioChecklist))
    {
        for (int i = 0; i < item.totalOptions(); i++)
            listWidget[XDialogItem::ListWidget::OptionsLw]->addItem(item.option(i));
    } else
        lineEdit[XDialogItem::LineEdit::Options]->setText(item.option());
}

void XDialogItem::setupMenuReferences(const XanteItem &item, XanteJTF &jtf)
{
    QComboBox *cb = comboBox[XDialogItem::ComboBox::MenuReference];

    if (item.type() != XanteItem::Type::Menu)
        return;

    cb->clear();

    for (int i = 0; i < jtf.totalMenus(); i++) {
        XanteMenu m = jtf.menuAt(i);
        cb->addItem(m.name());
    }

    try {
        XanteMenu m = jtf.getMenu(item.referencedMenu());
        cb->setCurrentIndex(cb->findText(m.name()));
    } catch (std::exception &e) {
        // TODO: Handle exception here
    }
}

void XDialogItem::setupWidgets(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    XanteMenu menu = jtf.menuAt(currentMenuIndex);
    XanteItem &item = menu.itemAt(currentItemIndex);

    /* Prepare dialog widgets for item */
    selectItemType(item.type());

    /* Common informations */
    lineEdit[XDialogItem::LineEdit::Name]->setText(item.name());
    lineEdit[XDialogItem::LineEdit::ObjectId]->setText(item.objectId());
    comboBox[XDialogItem::ComboBox::Type]->setCurrentIndex((int)item.type());
    comboBox[XDialogItem::ComboBox::Mode]->setCurrentIndex((int)item.mode());

    /* Specific informations */
    setupConfigWidgets(item);
    setupEventsWidgets(item);
    setupHelpWidgets(item);
    setupInputRangesWidgets(item);
    setupOptionsWidgets(item);
    setupMenuReferences(item, jtf);
}

void XDialogItem::disableAllWidgets(void)
{
    comboBox[XDialogItem::ComboBox::MenuReference]->setEnabled(false);
    groupBox[XDialogItem::GroupBox::Config]->setEnabled(false);
    groupBox[XDialogItem::GroupBox::OptionsGb]->setEnabled(false);
    groupBox[XDialogItem::GroupBox::InputRanges]->setEnabled(false);
    groupBox[XDialogItem::GroupBox::Events]->setChecked(false);
    groupBox[XDialogItem::GroupBox::Events]->setEnabled(false);
}

void XDialogItem::enableInputRanges(int type)
{
    bool inputString = false, inputMin = false, inputMax = false;

    if ((type != XanteItem::Type::InputInt) &&
        (type != XanteItem::Type::InputFloat) &&
        (type != XanteItem::Type::InputString) &&
        (type != XanteItem::Type::InputPasswd))
    {
        return;
    }

    groupBox[XDialogItem::GroupBox::InputRanges]->setEnabled(true);

    if ((type == XanteItem::Type::InputInt) ||
        (type == XanteItem::Type::InputFloat))
    {
        inputMin = true;
        inputMax = true;
    } else
        inputString = true;

    lineEdit[XDialogItem::LineEdit::InputStringLength]->setEnabled(inputString);
    lineEdit[XDialogItem::LineEdit::InputMin]->setEnabled(inputMin);
    lineEdit[XDialogItem::LineEdit::InputMax]->setEnabled(inputMax);
}

void XDialogItem::enableOptions(int type)
{
    bool listOptions = false, lineOptions = false;

    if ((type == XanteItem::Type::Checklist) ||
        (type == XanteItem::Type::RadioChecklist))
    {
        listOptions = true;
    } else
        lineOptions = true;

    groupBox[XDialogItem::GroupBox::OptionsGb]->setEnabled(true);
    lineEdit[XDialogItem::LineEdit::Options]->setEnabled(lineOptions);
    groupBox[XDialogItem::GroupBox::ListOptions]->setEnabled(listOptions);
}

void XDialogItem::enableHelp(int type)
{
    bool listOptions = false;

    if (((type == XanteItem::Type::Checklist) ||
         (type == XanteItem::Type::RadioChecklist)) &&
        groupBox[XDialogItem::GroupBox::Help]->isChecked())
    {
        listOptions = true;
    }

    groupBox[XDialogItem::GroupBox::ListHelpOptions]->setEnabled(listOptions);
}

void XDialogItem::selectItemType(int index)
{
    /* Adjusts current widgets according to the selected item type */
    enum XanteItem::Type type = (enum XanteItem::Type)index;

    disableAllWidgets();

    /* Enable common widgets */
    groupBox[XDialogItem::GroupBox::Events]->setEnabled(true);
    enableHelp(type);

    /* And specifics for each type */
    switch (type) {
        case XanteItem::Type::Menu:
            comboBox[XDialogItem::ComboBox::MenuReference]->setEnabled(true);
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
            groupBox[XDialogItem::GroupBox::Config]->setEnabled(true);
            enableInputRanges(type);
            enableOptions(type);
            break;

        default:
            break;
    }
}

void XDialogItem::addOption(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New option"),
                                           tr("Enter the new option name:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    listWidget[XDialogItem::ListWidget::OptionsLw]->addItem(option);
}

void XDialogItem::delOption(void)
{
    QListWidget *l = listWidget[XDialogItem::ListWidget::OptionsLw];
    int row = l->currentRow();

    if (row < 0)
        return;

    l->takeItem(row);
}

void XDialogItem::addOptionHelp(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New option"),
                                           tr("Enter the new option brief help:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    listWidget[XDialogItem::ListWidget::HelpOptions]->addItem(option);
}

void XDialogItem::delOptionHelp(void)
{
    QListWidget *l = listWidget[XDialogItem::ListWidget::HelpOptions];
    int row = l->currentRow();

    if (row < 0)
        return;

    l->takeItem(row);
}

void XDialogItem::helpGroupToggled(bool on)
{
    if (on == false)
        return;

    /* Are we editing a checklist or a radioChecklist item? */
    enableHelp(comboBox[XDialogItem::ComboBox::Type]->currentIndex());
}

void XDialogItem::clear(void)
{
    for (int i = XDialogItem::LineEdit::Name;
         i < XDialogItem::LineEdit::MaxLineEdit;
         i++)
    {
        lineEdit[i]->clear();
    }

    for (int i = XDialogItem::ComboBox::Type;
         i < XDialogItem::ComboBox::MaxComboBox;
         i++)
    {
        comboBox[i]->setCurrentIndex(0);
    }

    for (int i = XDialogItem::CheckBox::EvSelected;
         i < XDialogItem::CheckBox::MaxCheckBox;
         i++)
    {
        checkBox[i]->setChecked(false);
    }

    for (int i = XDialogItem::ListWidget::OptionsLw;
         i < XDialogItem::ListWidget::MaxListWidget;
         i++)
    {
        listWidget[i]->clear();
    }

    currentMenuIndex = -1;
    currentItemIndex = -1;
}

void XDialogItem::updateXanteItemEvents(XanteItem &item)
{
    QMap<enum XanteItem::Event,
        QPair<QCheckBox *, QLineEdit *>> events;

    events.insert(XanteItem::Event::Selected,
                  qMakePair(checkBox[XDialogItem::CheckBox::EvSelected],
                            lineEdit[XDialogItem::LineEdit::EventSelected]));

    events.insert(XanteItem::Event::Exit,
                  qMakePair(checkBox[XDialogItem::CheckBox::EvExit],
                            lineEdit[XDialogItem::LineEdit::EventExit]));

    events.insert(XanteItem::Event::ValueConfirmed,
                  qMakePair(checkBox[XDialogItem::CheckBox::EvValueConfirmed],
                            lineEdit[XDialogItem::LineEdit::EventValueConfirmed]));

    events.insert(XanteItem::Event::ValueChanged,
                  qMakePair(checkBox[XDialogItem::CheckBox::EvValueChanged],
                            lineEdit[XDialogItem::LineEdit::EventValueChanged]));

    QMapIterator<enum XanteItem::Event, QPair<QCheckBox *, QLineEdit *>> i(events);

    while (i.hasNext()) {
        i.next();

        enum XanteItem::Event eventType = i.key();
        QPair<QCheckBox*, QLineEdit *> ui = i.value();
        QString text = ui.second->text();

        if (ui.first->isChecked() && (text.isEmpty() == false)) {
            item.event(text, eventType);
        }
    }
}

void XDialogItem::updateXanteItemHelp(XanteItem &item)
{
    enum XanteItem::Type type = item.type();
    QString data;

    if ((type == XanteItem::Type::Checklist) ||
        (type == XanteItem::Type::RadioChecklist))
    {
        QListWidget *l = listWidget[XDialogItem::ListWidget::HelpOptions];

        for (int i = 0; i < l->count(); i++) {
            data = l->item(i)->text();
            item.helpOption(data);
        }
    }

    data = lineEdit[XDialogItem::LineEdit::HelpBrief]->text();
    item.briefHelp(data);

    data = lineEdit[XDialogItem::LineEdit::HelpDescriptive]->text();
    item.descriptiveHelp(data);
}

void XDialogItem::updateXanteItemOptions(XanteItem &item)
{
    enum XanteItem::Type type = item.type();
    QString data;

    if ((type == XanteItem::Type::Checklist) ||
        (type == XanteItem::Type::RadioChecklist))
    {
        QListWidget *l = listWidget[XDialogItem::ListWidget::OptionsLw];

        for (int i = 0; i < l->count(); i++) {
            data = l->item(i)->text();
            item.option(data);
        }
    } else {
        data = lineEdit[XDialogItem::LineEdit::Options]->text();
        item.option(data);
    }
}

void XDialogItem::updateXanteItemInputRanges(XanteItem &item)
{
    QString data, min, max;
    enum XanteItem::Type type = item.type();

    switch (type) {
        case XanteItem::Type::InputString:
        case XanteItem::Type::InputPasswd:
            data = lineEdit[XDialogItem::LineEdit::InputStringLength]->text();
            item.stringLength(data.toInt());
            break;

        case XanteItem::Type::InputInt:
        case XanteItem::Type::InputFloat:
            min = lineEdit[XDialogItem::LineEdit::InputMin]->text();
            max = lineEdit[XDialogItem::LineEdit::InputMax]->text();

            if (type == XanteItem::Type::InputInt)
                item.minMax(min.toInt(), max.toInt());
            else
                item.minMax(min.toFloat(), max.toFloat());

            break;

        default:
            break;
    }
}

void XDialogItem::updateXanteItemConfig(XanteItem &item)
{
    QString data;

    data = lineEdit[XDialogItem::LineEdit::CfgBlock]->text();
    item.configBlock(data);

    data = lineEdit[XDialogItem::LineEdit::CfgItem]->text();
    item.configItem(data);

    data = lineEdit[XDialogItem::LineEdit::DefaultValue]->text();
    item.defaultValue(data);
}

/*
 * Creates a XanteItem object from the widgets current content.
 */
XanteItem XDialogItem::createXanteItemFromWidgets(XanteJTF &jtf,
    const XanteMenu &menu)
{
    QString data;

    /* name */
    data = lineEdit[XDialogItem::LineEdit::Name]->text();
    XanteItem item(jtf.applicationName(), menu.name(), data);

    /* type */
    enum XanteItem::Type type =
        (enum XanteItem::Type)comboBox[XDialogItem::ComboBox::Type]->currentIndex();

    item.type(type);

    /* access mode */
    enum XanteMode mode =
        (enum XanteMode)comboBox[XDialogItem::ComboBox::Mode]->currentIndex();

    item.mode(mode);

    /* events */
    if (groupBox[XDialogItem::GroupBox::Events]->isChecked())
        updateXanteItemEvents(item);

    /* help */
    if (groupBox[XDialogItem::GroupBox::Help]->isChecked())
        updateXanteItemHelp(item);

    /* options */
    if (item.hasOptions())
        updateXanteItemOptions(item);

    /* input ranges */
    if (item.hasInputRanges())
        updateXanteItemInputRanges(item);

    /* config */
    if (item.hasConfig())
        updateXanteItemConfig(item);

    if (type == XanteItem::Type::Menu) {
        XanteMenu m = jtf.menuAt(comboBox[XDialogItem::ComboBox::MenuReference]->currentIndex());
        item.referencedMenu(m.objectId());
    }

    return item;
}

/**
 * @name updateXanteItem
 * @brief Updates the current edited XanteItem data.
 *
 * This function is responsible to update the current edited XanteItem
 * informations with data from the widgets.
 *
 * @return If an invalid change has been made returns false, otherwise returns
 *         true, even if no change has been made.
 */
bool XDialogItem::updateXanteItem(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    XanteMenu &menu = jtf.menuAt(currentMenuIndex);
    XanteItem &item = menu.itemAt(currentItemIndex),
              newItem = createXanteItemFromWidgets(jtf, menu);

    if (item != newItem) {
        qDebug() << item.debug();
        qDebug() << newItem.debug();

        /* The TreeView must be updated when the names are different. */
        if (item.name() != newItem.name())
            emit treeViewNeedsUpdate();

        /* TODO: Do we have all the required fields filled? */
        if (0) {
            return false;
        }

        /* The MainWindow must know that we have a change to be saved. */
        emit projectHasChanges();
        item = newItem;
    }

    return true;
}

void XDialogItem::hideEvent(QHideEvent *event)
{
    /* Savel all modifications */
    if ((event->spontaneous() == false) && XMainWindow::activeProject()) {
        if (updateXanteItem() == false) {
            event->ignore();
            return;
        }
    }

    event->accept();
}

void XDialogItem::saveCurrentState(void)
{
    updateXanteItem();
}

