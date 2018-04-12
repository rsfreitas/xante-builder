
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
    QGroupBox *g;
    QHBoxLayout *hg, *h = new QHBoxLayout;

    g = new QGroupBox(tr("Main information"));
    hg = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Menu type:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < MENU_TYPE; i++)
        cb->addItem(QString(cbMenuTypeName[i]));

    hg->addWidget(label);
    hg->addWidget(cb);
    connect(cb, SIGNAL(activated(int)), this, SLOT(selectMenuType(int)));
    comboBox[XDialogMenu::ComboBox::Type] = cb;

    /* mode */
    label = new QLabel(tr("Access mode:"));
    cb = new QComboBox;
    label->setBuddy(cb);

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cb->addItem(QString(cbAccessModeName[i]));

    hg->addWidget(label);
    hg->addWidget(cb);
    comboBox[XDialogMenu::ComboBox::Mode] = cb;
    g->setLayout(hg);
    h->addWidget(g);

    g = new QGroupBox(tr("Geometry"));
    hg = new QHBoxLayout;
    label = new QLabel(tr("Width:"));
    sbWidth = new QSpinBox;
    sbWidth->setRange(30, 60);
    hg->addWidget(label);
    hg->addWidget(sbWidth);

    label = new QLabel(tr("Height:"));
    sbHeight = new QSpinBox;
    sbHeight->setRange(30, 60);
    hg->addWidget(label);
    hg->addWidget(sbHeight);
    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(hg);
    h->addWidget(g);

    return h;
}

QGroupBox *XDialogMenu::createEventsWidgets(void)
{
    QGroupBox *gb = new QGroupBox(tr("Events"));
    QHBoxLayout *l = new QHBoxLayout;

    tbEvents = new QTableWidget(1, 1);
    tbEvents->setHorizontalHeaderLabels(QStringList() << tr("Function name"));
    tbEvents->setVerticalHeaderLabels(QStringList() << tr("Exit event"));
    tbEvents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    l->addWidget(tbEvents);
    gb->setCheckable(true);
    gb->setChecked(false);
    gb->setLayout(l);
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
//    checkBox = QVector<QCheckBox *>(XDialogMenu::MaxCheckBox);
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
 * Sets the current project that is been edited, so all other information
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteMenu to the @selectedMenuIndex inside it.
 */
void XDialogMenu::setCurrentProject(int selectedMenuIndex)
{
    setSelection(selectedMenuIndex);
}

/*
 * Adjusts current selected XanteMenu dynamic information in the UI if it
 * contains them.
 */
void XDialogMenu::setupDynamicInfoWidgets(XanteMenu menu)
{
    QPair<QString, QPair<QString, QString>> origin;
    enum XanteMenu::DynamicType type = menu.dynamicType();

    /*
     * If we're dealing with a default menu or an unsupported dynamic menu
     * type, abort.
     */
    if ((menu.type() != XanteMenu::Type::Dynamic) ||
        (type >= XanteMenu::DynamicType::MaxDynamicMenuType))
    {
        return;
    }

    radioButton[type]->setChecked(true);

    switch (type) {
        case XanteMenu::DynamicType::FixedSize:
            lineEdit[XDialogMenu::LineEdit::DynamicNumberOfCopies]->setText(
                QString("%1").arg(menu.dynamicCopies())
            );

            break;

        case XanteMenu::DynamicType::FixedOptions:
            dynamicOptions->addItems(menu.dynamicOptions());
            break;

        case XanteMenu::DynamicType::DynamicOptions:
            origin = menu.dynamicOrigin();

            lineEdit[XDialogMenu::LineEdit::BlockPrefix]->setText(
                QString("%1").arg(origin.first)
            );

            lineEdit[XDialogMenu::LineEdit::DynamicOriginBlock]->setText(
                QString("%1").arg(origin.second.first)
            );

            lineEdit[XDialogMenu::LineEdit::DynamicOriginItem]->setText(
                QString("%1").arg(origin.second.second)
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
    groupBox[XDialogMenu::GroupBox::Events]->setEnabled(true);

    if (menu.hasEvents() == false)
        return;

    groupBox[XDialogMenu::GroupBox::Events]->setChecked(true);

    for (int i = XanteMenu::Event::Exit, row = 0;
         i < XanteMenu::Event::MaxEvents;
         i++, row++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(
            QString(menu.event(static_cast<enum XanteMenu::Event>(i)))
        );

        tbEvents->setItem(row, 0, item);
    }
}

void XDialogMenu::disableAllWidgets(void)
{
    groupBox[XDialogMenu::GroupBox::Events]->setChecked(false);
    groupBox[XDialogMenu::GroupBox::Events]->setEnabled(false);
    groupBox[XDialogMenu::GroupBox::Dynamic]->setEnabled(false);
}

void XDialogMenu::selectMenuType(int index)
{
    groupBox[XDialogMenu::GroupBox::Dynamic]->setEnabled((index == 0) ? false
                                                                       : true);
}

void XDialogMenu::setupWidgets(XanteMenu menu)
{
    disableAllWidgets();

    /* Enable/Disable dynamics groupbox */
    selectMenuType((menu.type() == XanteMenu::Type::Default) ? 0 : 1);

    lineEdit[XDialogMenu::LineEdit::Name]->setText(menu.name());
    lineEdit[XDialogMenu::LineEdit::ObjectId]->setText(menu.objectId());
    comboBox[XDialogMenu::ComboBox::Type]->setCurrentIndex((int)menu.type());
    comboBox[XDialogMenu::ComboBox::Mode]->setCurrentIndex((int)menu.mode());

    setupDynamicInfoWidgets(menu);
    setupEventsWidgets(menu);
}

void XDialogMenu::setupWidgets(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF jtf = project.getJtf();

    try {
        XanteMenu menu = jtf.menuAt(currentMenuIndex);
        setupWidgets(menu);
    } catch (std::exception &e) {
        /* Ok, something went wrong. Return the index to the first object. */
        currentMenuIndex = 0;
    }
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

    tbEvents->clearContents();
    currentMenuIndex = -1;
}

/*
 * Sets data on the User Interface by using the @selectedMenuIndex to get its
 * corresponding XanteMenu (which was previously loaded).
 */
void XDialogMenu::setSelection(int selectedMenuIndex)
{
    clear();
    currentMenuIndex = selectedMenuIndex;
    setupWidgets();
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

void XDialogMenu::updateXanteMenuDynamic(XanteMenu &menu)
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

            menu.dynamicCopies(copies.toInt());
            break;

        case XanteMenu::DynamicType::FixedOptions:
            for (int i = 0; i < dynamicOptions->count(); i++)
                menu.dynamicOptions(dynamicOptions->item(i)->text());

            break;

        case XanteMenu::DynamicType::DynamicOptions:
            prefix =
                lineEdit[XDialogMenu::LineEdit::BlockPrefix]->text();

            block =
                lineEdit[XDialogMenu::LineEdit::DynamicOriginBlock]->text();

            item =
                lineEdit[XDialogMenu::LineEdit::DynamicOriginItem]->text();

            menu.dynamicOrigin(prefix, block, item);
            break;

        default:
            break;
    }
}

void XDialogMenu::updateXanteMenuEvents(XanteMenu &menu)
{
    for (int i = 0; i < tbEvents->rowCount(); i++) {
        QTableWidgetItem *item = tbEvents->item(i, 0);

        if (NULL == item)
            continue;

        QString fname = item->text();

        if (fname.isEmpty() == false)
            menu.event(fname, static_cast<enum XanteMenu::Event>(i));
    }
}

XanteMenu XDialogMenu::createXanteMenuFromWidgets(XanteJTF &jtf)
{
    QString data;

    /* name */
    data = lineEdit[XDialogMenu::LineEdit::Name]->text();
    XanteMenu menu(jtf.applicationName(), data);

    /* type */
    enum XanteMenu::Type type =
        (enum XanteMenu::Type)comboBox[XDialogMenu::ComboBox::Type]->currentIndex();

    menu.type(type);

    /* dynamic options */
    if (type == XanteMenu::Type::Dynamic)
        updateXanteMenuDynamic(menu);

    /* access mode */
    enum XanteAccessMode mode =
        (enum XanteAccessMode)comboBox[XDialogMenu::ComboBox::Mode]->currentIndex();

    menu.mode(mode);

    /* events */
    if (groupBox[XDialogMenu::GroupBox::Events]->isChecked())
        updateXanteMenuEvents(menu);

    return menu;
}

bool XDialogMenu::updateXanteMenu(void)
{
    bool new_menu_name = false;
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();
    XanteMenu &menu = jtf.menuAt(currentMenuIndex),
              newMenu = createXanteMenuFromWidgets(jtf);

    if (menu != newMenu) {
        qDebug() << menu.debug(false);
        qDebug() << newMenu.debug(false);

        /* The TreeView must be updated when the names are different. */
        if (menu.name() != newMenu.name())
            new_menu_name = true;

        /* Do we have all the required fields filled? */
        if (newMenu.name().isEmpty()) {
            QMessageBox::warning(this, tr("Update error"),
                                 QString("The menu <b>name</b> is missing"));

            return false;
        }

        menu = newMenu;

        if (new_menu_name)
            emit treeViewNeedsUpdate();

        /* The MainWindow must know that we have a change to be saved. */
        emit projectHasChanges();
    }

    return true;
}

bool XDialogMenu::saveCurrentState(void)
{
    return updateXanteMenu();
}

