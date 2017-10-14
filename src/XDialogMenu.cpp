
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

static const char *cbMenuTypeName[] = {
    "default",
    "dynamic"
};

#define MENU_TYPE       \
    (sizeof(cbMenuTypeName) / sizeof(cbMenuTypeName[0]))

static const char *cbAccessModeName[] = {
    "hidden",
    "view",
    "edit"
};

#define ACCESS_MODE     \
    (sizeof(cbAccessModeName) / sizeof(cbAccessModeName[0]))

QHBoxLayout *XDialogMenu::createIdentificationWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h = new QHBoxLayout;

    /* name */
    label = new QLabel(tr("Name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    lineEdit[XDialogMenu::LineEdit::Name] = edit;
    h->addWidget(label);
    h->addWidget(lineEdit[XDialogMenu::LineEdit::Name]);

    /* objectId */
    label = new QLabel(tr("Object ID:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    edit->setEnabled(false);
    lineEdit[XDialogMenu::LineEdit::ObjectId] = edit;
    h->addWidget(label);
    h->addWidget(lineEdit[XDialogMenu::LineEdit::ObjectId]);

    return h;
}

QHBoxLayout *XDialogMenu::createTypeWidgets(void)
{
    QLabel *label;
    QComboBox *cb;
    QHBoxLayout *h = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Menu type:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < MENU_TYPE; i++)
        cb->addItem(QString(cbMenuTypeName[i]));

    h->addWidget(label);
    h->addWidget(cb);
    connect(cb, SIGNAL(activated(int)), this, SLOT(selectMenuType(int)));
    comboBox[XDialogMenu::ComboBox::Type] = cb;

    /* mode */
    label = new QLabel(tr("Access mode:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb->addItem(QString(cbAccessModeName[i]));

    h->addWidget(label);
    h->addWidget(cb);
    comboBox[XDialogMenu::ComboBox::Mode] = cb;

    return h;
}

QGroupBox *XDialogMenu::createEventsWidgets(void)
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
    checkBox[XDialogMenu::CheckBox::EvSelected] = cb;
    lineEdit[XDialogMenu::LineEdit::EventSelected] = edit;

    cb = new QCheckBox(tr("Exit event"));
    label = new QLabel(tr("Function name:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h2->addWidget(cb);
    h2->addWidget(label);
    h2->addWidget(edit);
    checkBox[XDialogMenu::CheckBox::EvExit] = cb;
    lineEdit[XDialogMenu::LineEdit::EventExit] = edit;

    v->addLayout(h1);
    v->addLayout(h2);
    gb->setLayout(v);
    groupBox[XDialogMenu::GroupBox::Events] = gb;

    return gb;
}

QGroupBox *XDialogMenu::createDynamicFixedSizeWidgets(void)
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
    lineEdit[XDialogMenu::LineEdit::DynamicNumberOfCopies] = edit;
    groupBox[XDialogMenu::GroupBox::DynamicFixed] = gb;

    return gb;
}

QGroupBox *XDialogMenu::createDynamicFixedOptionsWidgets(void)
{
    QGroupBox *gb = new QGroupBox;
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;

    QPushButton *btAdd = new QPushButton(tr("Add"));
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addDynamicFixedOption()));

    QPushButton *btDel = new QPushButton(tr("Remove"));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delDynamicFixedOption()));

    v->addWidget(btAdd, 0, Qt::AlignBottom);
    v->addWidget(btDel, 0, Qt::AlignTop);

    dynamicOptions = new QListWidget;
    h->addWidget(dynamicOptions);
    h->addLayout(v);

    gb->setLayout(h);
    groupBox[XDialogMenu::GroupBox::DynamicOptions] = gb;

    return gb;
}

QGroupBox *XDialogMenu::createDynamicOptionsWidgets(void)
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
    lineEdit[XDialogMenu::LineEdit::BlockPrefix] = edit;

    label = new QLabel(tr("Block origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h2->addWidget(label);
    h2->addWidget(edit);
    lineEdit[XDialogMenu::LineEdit::DynamicOriginBlock] = edit;

    label = new QLabel(tr("Item origin:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h3->addWidget(label);
    h3->addWidget(edit);
    lineEdit[XDialogMenu::LineEdit::DynamicOriginItem] = edit;

    v->addLayout(h1);
    v->addLayout(h2);
    v->addLayout(h3);
    gb->setLayout(v);
    groupBox[XDialogMenu::GroupBox::DynamicConfig] = gb;

    return gb;
}

QGroupBox *XDialogMenu::createDynamicDetailsWidgets(void)
{
    QGridLayout *grid = new QGridLayout;

    static const char *option[] = {
        "Fixed size",
        "Fixed options",
        "Dynamic options"
    };

    /* Dynamic option properties */
    grid->addWidget(createDynamicFixedSizeWidgets(), 1, 0, Qt::AlignTop);
    grid->addWidget(createDynamicFixedOptionsWidgets(), 1, 1);
    grid->addWidget(createDynamicOptionsWidgets(), 1, 2, Qt::AlignTop);

    /* Radio buttons */
    for (int i = 0; i < XanteMenu::MaxDynamicMenuType; i++) {
        QRadioButton *rb = new QRadioButton(QString(option[i]));
        rb->setObjectName(QString("%1").arg(i));
        grid->addWidget(rb, 0, i);

        if (i == 0)
            rb->setChecked(true);
        else
            groupBox[i]->setEnabled(false);

        connect(rb, &QRadioButton::toggled, this, &XDialogMenu::dynamicRadioToggled);
        radioButton[i] = rb;
    }

    QGroupBox *gbDynamic = new QGroupBox(tr("Dynamic properties"));
    gbDynamic->setLayout(grid);
    groupBox[XDialogMenu::GroupBox::Dynamic] = gbDynamic;

    return gbDynamic;
}

XDialogMenu::XDialogMenu(QWidget *parent)
    : QWidget(parent)
{
    lineEdit = QVector<QLineEdit *>(XDialogMenu::MaxLineEdit);
    checkBox = QVector<QCheckBox *>(XDialogMenu::MaxCheckBox);
    comboBox = QVector<QComboBox *>(XDialogMenu::MaxComboBox);
    groupBox = QVector<QGroupBox *>(XDialogMenu::MaxGroupBox);
    radioButton = QVector<QRadioButton *>(XanteMenu::MaxDynamicMenuType);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addLayout(createIdentificationWidgets());
    layout->addLayout(createTypeWidgets());
    layout->addWidget(createEventsWidgets());
    layout->addWidget(createDynamicDetailsWidgets());

    setLayout(layout);
}

XDialogMenu::~XDialogMenu()
{
}

void XDialogMenu::dynamicRadioToggled(bool checked)
{
    if (checked == true) {
        QRadioButton *rb = static_cast<QRadioButton *>(sender());
        enum XanteMenu::DynamicType selectedButton = (enum XanteMenu::DynamicType)rb->objectName().toInt();
        int i;

        for (i = 0; i < XanteMenu::MaxDynamicMenuType; i++)
            groupBox[i]->setEnabled((selectedButton == i) ? true : false);
    }
}

/*
 * Sets the current project that's been edited, so all other informations
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteMenu to the @selectedMenuIndex inside it.
 */
void XDialogMenu::setCurrentProject(int selectedMenuIndex)
{
    setSelection(selectedMenuIndex);
}

/*
 * Sets data by using the @selectedMenuIndex to get its corresponding
 * XanteMenu.
 */
void XDialogMenu::setSelection(int selectedMenuIndex)
{
    currentMenuIndex = selectedMenuIndex;
    setupWidgets();
}

void XDialogMenu::setupWidgets(void)
{
    XanteProject &project = XMainWindow::getProject();

    XanteJTF jtf = project.getJtf();
    XanteMenu menu = jtf.menuAt(currentMenuIndex);

    setupWidgets(menu);
}

/*
 * Adjusts current selected XanteMenu dynamic informations in the UI if it
 * contains them.
 */
void XDialogMenu::setupDynamicInfoWidgets(XanteMenu menu)
{
    enum XanteMenu::DynamicType type = menu.getDynamicType();

    /*
     * If we're dealing with a default menu or an unsupported dynamic menu
     * type, abort.
     */
    if ((menu.getType() != XanteMenu::Type::Dynamic) ||
        (type >= XanteMenu::DynamicType::MaxDynamicMenuType))
    {
        return;
    }

    radioButton[type]->setChecked(true);

    switch (menu.getDynamicType()) {
        case XanteMenu::DynamicType::FixedSize:
            lineEdit[XDialogMenu::LineEdit::DynamicNumberOfCopies]->setText(
                QString("%1").arg(menu.getDynamicCopies())
            );

            break;

        case XanteMenu::DynamicType::FixedOptions:
            dynamicOptions->addItems(menu.getDynamicOptions());
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            lineEdit[XDialogMenu::LineEdit::BlockPrefix]->setText(
                QString("%1").arg(menu.getBlockPrefix())
            );

            lineEdit[XDialogMenu::LineEdit::DynamicOriginBlock]->setText(
                QString("%1").arg(menu.getDynamicOriginBlock())
            );

            lineEdit[XDialogMenu::LineEdit::DynamicOriginItem]->setText(
                QString("%1").arg(menu.getDynamicOriginItem())
            );

            break;

        default:
            break;
    }
}

/*
 * Adjusts current selected XanteMenu events information in the UI.
 */
void XDialogMenu::setupEventsWidgets(XanteMenu menu)
{
    int i;
    QList<QPair<enum XanteMenu::Event,
                QPair<enum XDialogMenu::LineEdit,
                      enum XDialogMenu::CheckBox>>> events;

    if (menu.hasEvents() == false)
        return;

    events.append(qMakePair(XanteMenu::Event::Selected,
                            qMakePair(XDialogMenu::LineEdit::EventSelected,
                                      XDialogMenu::CheckBox::EvSelected)));

    events.append(qMakePair(XanteMenu::Event::Exit,
                            qMakePair(XDialogMenu::LineEdit::EventExit,
                                      XDialogMenu::CheckBox::EvExit)));

    groupBox[XDialogMenu::GroupBox::Events]->setChecked(true);
    groupBox[XDialogMenu::GroupBox::Events]->setEnabled(true);

    for (i = 0; i < events.size(); i++) {
        QPair<enum XanteMenu::Event,
              QPair<enum XDialogMenu::LineEdit,
                    enum XDialogMenu::CheckBox>> p = events.at(i);

        QString eventName = menu.getEvent(p.first);
        QPair<enum XDialogMenu::LineEdit,
              enum XDialogMenu::CheckBox> pp = p.second;

        if (eventName.isEmpty() == false) {
            lineEdit[pp.first]->setText(eventName);
            checkBox[pp.second]->setChecked(true);
        }
    }
}

void XDialogMenu::disableAllWidgets(void)
{
    groupBox[XDialogMenu::GroupBox::Events]->setChecked(false);
    groupBox[XDialogMenu::GroupBox::Events]->setEnabled(false);
    groupBox[XDialogMenu::GroupBox::Dynamic]->setEnabled(false);
}

void XDialogMenu::setupWidgets(XanteMenu menu)
{
    disableAllWidgets();

    /* Enable/Disable dynamics groupbox */
    selectMenuType((menu.getType() == XanteMenu::Type::Default) ? 0 : 1);

    lineEdit[XDialogMenu::LineEdit::Name]->setText(menu.getName());
    lineEdit[XDialogMenu::LineEdit::ObjectId]->setText(menu.getObjectId());
    comboBox[XDialogMenu::ComboBox::Type]->setCurrentIndex((int)menu.getType());
    comboBox[XDialogMenu::ComboBox::Mode]->setCurrentIndex((int)menu.getMode());

    setupDynamicInfoWidgets(menu);
    setupEventsWidgets(menu);
}

void XDialogMenu::selectMenuType(int index)
{
    groupBox[XDialogMenu::GroupBox::Dynamic]->setEnabled((index == 0) ? false
                                                                       : true);
}

void XDialogMenu::addDynamicFixedOption(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New dynamic option"),
                                           tr("Enter the new dynamic option name:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    dynamicOptions->addItem(option);
}

void XDialogMenu::delDynamicFixedOption(void)
{
    int row = dynamicOptions->currentRow();

    if (row < 0)
        return;

    dynamicOptions->takeItem(row);
}

void XDialogMenu::clear(void)
{
    for (int i = XDialogMenu::LineEdit::Name;
         i < XDialogMenu::LineEdit::MaxLineEdit;
         i++)
    {
        lineEdit[i]->clear();
    }

    for (int i = XDialogMenu::ComboBox::Type;
         i < XDialogMenu::ComboBox::MaxComboBox;
         i++)
    {
        comboBox[i]->setCurrentIndex(0);
    }

    for (int i = XDialogMenu::CheckBox::EvSelected;
         i < XDialogMenu::CheckBox::MaxCheckBox;
         i++)
    {
        checkBox[i]->setChecked(false);
    }
}

bool XDialogMenu::updateXanteMenuDynamic(XanteMenu &menu)
{
    enum XanteMenu::DynamicType type;
    QString copies, prefix, block, item;

    for (int i = 0; i < XanteMenu::MaxDynamicMenuType; i++) {
        if (radioButton[i]->isChecked()) {
            type = (enum XanteMenu::DynamicType)i;
            break;
        }
    }

    switch (type) {
        case XanteMenu::DynamicType::FixedSize:
            copies =
                lineEdit[XDialogMenu::LineEdit::DynamicNumberOfCopies]->text();

            menu.setDynamic(copies.toInt());
            break;

        case XanteMenu::DynamicType::FixedOptions:
            for (int i = 0; i < dynamicOptions->count(); i++)
                menu.setDynamic(dynamicOptions->item(i)->text());

            break;

        case XanteMenu::DynamicType::DynamicOptions:
            prefix =
                lineEdit[XDialogMenu::LineEdit::BlockPrefix]->text();

            block =
                lineEdit[XDialogMenu::LineEdit::DynamicOriginBlock]->text();

            item =
                lineEdit[XDialogMenu::LineEdit::DynamicOriginItem]->text();

            menu.setDynamic(prefix, block, item);
            break;

        default:
            break;
    }

    return true;
}

bool XDialogMenu::updateXanteMenuEvents(XanteMenu &menu)
{
    QMap<enum XanteMenu::Event,
        QPair<QCheckBox *, QLineEdit *>> events;

    events.insert(XanteMenu::Event::Selected,
                  qMakePair(checkBox[XDialogMenu::CheckBox::EvSelected],
                            lineEdit[XDialogMenu::LineEdit::EventSelected]));

    events.insert(XanteMenu::Event::Exit,
                  qMakePair(checkBox[XDialogMenu::CheckBox::EvExit],
                            lineEdit[XDialogMenu::LineEdit::EventExit]));

    QMapIterator<enum XanteMenu::Event, QPair<QCheckBox *, QLineEdit *>> i(events);

    while (i.hasNext()) {
        i.next();

        enum XanteMenu::Event eventType = i.key();
        QPair<QCheckBox*, QLineEdit *> ui = i.value();
        QString text = ui.second->text();

        if (ui.first->isChecked() && (text.isEmpty() == false)) {
            menu.setEvent(text, eventType);
        }
    }

    return true;
}

bool XDialogMenu::updateXanteMenu(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    XanteMenu &menu = jtf.menuAt(currentMenuIndex);
    QString data;

    /* name */
    data = lineEdit[XDialogMenu::LineEdit::Name]->text();

    if ((data.isEmpty() == false) && (menu.getName() != data)) {
        menu.setName(data);
        /* TODO: update objectId */
        emit treeViewNeedsUpdate();
    }

    /* type */
    enum XanteMenu::Type type =
        (enum XanteMenu::Type)comboBox[XDialogMenu::ComboBox::Type]->currentIndex();

    menu.setType(type);

    /* dynamic options */
    if (type == XanteMenu::Type::Dynamic)
        if (updateXanteMenuDynamic(menu) == false)
            return false;

    /* access mode */
    enum XanteMode mode =
        (enum XanteMode)comboBox[XDialogMenu::ComboBox::Mode]->currentIndex();

    menu.setMode(mode);

    /* events */
    if (groupBox[XDialogMenu::GroupBox::Events]->isChecked())
        if (updateXanteMenuEvents(menu) == false)
            return false;

    return true;
}

void XDialogMenu::hideEvent(QHideEvent *event)
{
    /* Save all modifications */
    if ((event->spontaneous() == false) && XMainWindow::activeProject()) {
        if (updateXanteMenu() == false) {
            event->ignore();
            return;
        }

        emit projectHasChanges();
    }

    event->accept();
}

