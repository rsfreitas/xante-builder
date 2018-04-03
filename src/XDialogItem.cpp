
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
    XANTE_STR_WIDGET_MENU,
    XANTE_STR_WIDGET_INPUT_INT,
    XANTE_STR_WIDGET_INPUT_FLOAT,
    XANTE_STR_WIDGET_INPUT_DATE,
    XANTE_STR_WIDGET_INPUT_TIME,
    XANTE_STR_WIDGET_INPUT_STRING,
    XANTE_STR_WIDGET_INPUT_PASSWD,
    XANTE_STR_WIDGET_CALENDAR,
    XANTE_STR_WIDGET_TIMEBOX,
    XANTE_STR_WIDGET_RADIO_CHECKLIST,
    XANTE_STR_WIDGET_CHECKLIST,
    XANTE_STR_WIDGET_YESNO,
    XANTE_STR_WIDGET_DYNAMIC_MENU,
    XANTE_STR_WIDGET_DELETE_DYNAMIC_MENU,
    XANTE_STR_WIDGET_ADD_DYNAMIC_MENU,
    XANTE_STR_WIDGET_CUSTOM,
    XANTE_STR_WIDGET_PROGRESS,
    XANTE_STR_WIDGET_SPINNER_SYNC,
    XANTE_STR_WIDGET_DOTS_SYNC,
    XANTE_STR_WIDGET_RANGE,
    XANTE_STR_WIDGET_FILE_SELECT,
    XANTE_STR_WIDGET_DIR_SELECT,
    XANTE_STR_WIDGET_FILE_VIEW,
    XANTE_STR_WIDGET_TAILBOX,
    XANTE_STR_WIDGET_SCROLLTEXT,
    XANTE_STR_WIDGET_UPDATE_OBJECT,
    XANTE_STR_WIDGET_INPUTSCROLL,
    XANTE_STR_WIDGET_MIXEDFORM,
    XANTE_STR_WIDGET_BUILDLIST,
    XANTE_STR_WIDGET_SPREADSHEET,
    XANTE_STR_GADGET_CLOCK,
};

#define ITEM_TYPE       \
    (sizeof(cbItemTypeName) / sizeof(cbItemTypeName[0]))

/** *** *** *** ***
 *
 * Handles the UI creation -- Begin
 *
 */

/*
 *
 * Main information
 *
 */

QGroupBox *XDialogItem::createMainWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QComboBox *cb;
    QHBoxLayout *h = new QHBoxLayout;
    QGroupBox *g = new QGroupBox(tr("Details"));

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

    g->setLayout(h);

    return g;
}

/*
 *
 * Content
 *
 */

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

QGroupBox *XDialogItem::createItemConfigurationWidgets(void)
{
    QLabel *label;
    QLineEdit *edit;
    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *g = new QGroupBox(tr("Settings (config)"));
    QHBoxLayout *h;

    /* default value */
    h = new QHBoxLayout;
    label = new QLabel(tr("Default value:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    v->addLayout(h);
    lineEdit[XDialogItem::LineEdit::DefaultValue] = edit;

    h = new QHBoxLayout;
    label = new QLabel(tr("Block:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    v->addLayout(h);
    lineEdit[XDialogItem::LineEdit::CfgBlock] = edit;

    h = new QHBoxLayout;
    label = new QLabel(tr("Entry:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    v->addLayout(h);
    lineEdit[XDialogItem::LineEdit::CfgItem] = edit;

    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::Config] = g;

    return g;
}

QGroupBox *XDialogItem::createRangesWidgets(void)
{
    QLabel *label;
    QSpinBox *sb;
    QHBoxLayout *h;
    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *g = new QGroupBox(tr("Input ranges"));

    /* String length */
    h = new QHBoxLayout;
    label = new QLabel(tr("String length:"));
    sb = new QSpinBox;
    sb->setRange(1, 1024);
    h->addWidget(label);
    h->addWidget(sb);
    spinBox[XDialogItem::SpinBox::InputStringLength] = sb;
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    label = new QLabel(tr("Min:"));
    sb = new QSpinBox;
    sb->setRange(1, 1024); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sb);
    spinBox[XDialogItem::SpinBox::InputMin] = sb;
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    label = new QLabel(tr("Max:"));
    sb = new QSpinBox;
    sb->setRange(1, 1024); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sb);
    spinBox[XDialogItem::SpinBox::InputMax] = sb;
    v->addLayout(h);

    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::InputRanges] = g;

    return g;
}

QHBoxLayout *XDialogItem::createRangesAndSettingsWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* Add Settings and Ranges */
    h->addWidget(createItemConfigurationWidgets());
    h->addWidget(createRangesWidgets());

    return h;
}

QVBoxLayout *XDialogItem::createContentWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h = new QHBoxLayout;

    /* Referenced Menu*/
    QLabel *label = new QLabel(tr("Referenced menu:"));
    QComboBox *cb = new QComboBox;
    h->addWidget(label);
    h->addWidget(cb);
    comboBox[XDialogItem::ComboBox::MenuReference] = cb;
    v->addLayout(h);

    /* Ranges and Settings */
    v->addLayout(createRangesAndSettingsWidgets());

    return v;
}

QGroupBox *XDialogItem::createDataWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;
    QGroupBox *g = new QGroupBox(tr("Content"));

    h->addWidget(createItemOptionsWidgets());
    h->addLayout(createContentWidgets());
    g->setLayout(h);

    return g;
}

/*
 *
 * Events
 *
 */

QGroupBox *XDialogItem::createEventsWidgets(void)
{
    QGroupBox *gb = new QGroupBox(tr("Events"));
    QHBoxLayout *l = new QHBoxLayout;

    tbEvents = new QTableWidget(XANTE_ITEM_DEFAULT_EVENTS, 1);
    tbEvents->setHorizontalHeaderLabels(QStringList() << tr("Function name"));
    tbEvents->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tbEvents->setVerticalHeaderLabels(QStringList() << tr("Selected")
                                                    << tr("Confirm value")
                                                    << tr("Value changed")
                                                    << tr("Exit")
                                                    << tr("Extra button pressed"));


    l->addWidget(tbEvents);
    gb->setCheckable(true);
    gb->setChecked(false);
    gb->setLayout(l);
    groupBox[XDialogItem::GroupBox::Events] = gb;

    return gb;
}

/*
 *
 * UI Object
 *
 */

QGroupBox *XDialogItem::createGeometryWidgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Geometry"));
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h;
    QSpinBox *sb;
    QLabel *label;

    h = new QHBoxLayout;
    label = new QLabel(tr("Width:"));
    sb = new QSpinBox;
    sb->setRange(30, 60); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sb);
    v->addLayout(h);
    spinBox[XDialogItem::SpinBox::Width] = sb;

    h = new QHBoxLayout;
    label = new QLabel(tr("Height:"));
    sb = new QSpinBox;
    sb->setRange(30, 60); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sb);
    v->addLayout(h);
    spinBox[XDialogItem::SpinBox::Height] = sb;

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::Geometry] = g;

    return g;
}

QVBoxLayout *XDialogItem::createExtraFeaturesWidgets(void)
{
    QHBoxLayout *hw, *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;
    QCheckBox *cb = new QCheckBox(tr("Use Extra button"));
    QLabel *l;
    QLineEdit *le;

    /* Extra button */
    v->addWidget(cb);
    checkBox[XDialogItem::CheckBox::ExtraButton] = cb;

    /* Window Title */
    hw = new QHBoxLayout;
    l = new QLabel(tr("Window title:"));
    le = new QLineEdit;
    l->setBuddy(le);
    hw->addWidget(l);
    hw->addWidget(le);
    v->addLayout(hw);
    lineEdit[XDialogItem::LineEdit::Title] = le;

    /* Geometry */
    v->addWidget(createGeometryWidgets());

    return v;
}

QHBoxLayout *XDialogItem::createUiTitleWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;
    QLabel *l;
    QLineEdit *le;

    l = new QLabel(tr("Window title:"));
    le = new QLineEdit;
    l->setBuddy(le);
    h->addWidget(l);
    h->addWidget(le);

    return h;
}

QGroupBox *XDialogItem::createBtnLabelsWidgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Buttons"));
    QVBoxLayout *v = new QVBoxLayout;
    QLabel *label;
    QLineEdit *edit;
    QHBoxLayout *h;

    /* ok */
    h = new QHBoxLayout;
    label = new QLabel(tr("Ok:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::BtnOkLabel] = edit;
    v->addLayout(h);

    /* cancel */
    h = new QHBoxLayout;
    label = new QLabel(tr("Cancel:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::BtnCancelLabel] = edit;
    v->addLayout(h);

    /* extra */
    h = new QHBoxLayout;
    label = new QLabel(tr("Extra:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::BtnExtraLabel] = edit;
    v->addLayout(h);

    /* help */
    h = new QHBoxLayout;
    label = new QLabel(tr("Help:"));
    edit = new QLineEdit;
    label->setBuddy(edit);
    h->addWidget(label);
    h->addWidget(edit);
    lineEdit[XDialogItem::LineEdit::BtnHelpLabel] = edit;
    v->addLayout(h);

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::BtnLabels] = g;

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

QGroupBox *XDialogItem::createUiWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *g = new QGroupBox(tr("UI"));
    QHBoxLayout *h = new QHBoxLayout;

    h->addLayout(createExtraFeaturesWidgets());
    h->addWidget(createBtnLabelsWidgets());
    h->addWidget(createItemHelpWidgets());
    v->addLayout(h);

    g->setCheckable(true);
    g->setChecked(false);
    g->setLayout(v);
    groupBox[XDialogItem::GroupBox::Ui] = g;

    return g;
}

/** *** *** *** ***
 *
 * Handles the UI creation -- End
 *
 */

XDialogItem::XDialogItem(QWidget *parent)
    : QWidget(parent)
{
    listWidget = QVector<QListWidget *>(XDialogItem::MaxListWidget);
    lineEdit = QVector<QLineEdit *>(XDialogItem::MaxLineEdit);
    groupBox = QVector<QGroupBox *>(XDialogItem::MaxGroupBox);
    comboBox = QVector<QComboBox *>(XDialogItem::MaxComboBox);
    checkBox = QVector<QCheckBox *>(XDialogItem::MaxCheckBox);
    spinBox = QVector<QSpinBox *>(XDialogItem::MaxSpinBox);

    QVBoxLayout *layout = new QVBoxLayout;
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(createMainWidgets());
    splitter->addWidget(createDataWidgets());
    splitter->addWidget(createEventsWidgets());
    splitter->addWidget(createUiWidgets());
    layout->addWidget(splitter);

    setLayout(layout);
}

/*
 * Clears all data inside the widgets, so a new item may be displayed or to
 * leave it clear when a project is closed.
 */
void XDialogItem::clear(void)
{
    // LineEdits
    for (int i = XDialogItem::LineEdit::Name;
         i < XDialogItem::LineEdit::MaxLineEdit;
         i++)
    {
        lineEdit[i]->clear();
    }

    // ComboBoxes
    for (int i = XDialogItem::ComboBox::Type;
         i < XDialogItem::ComboBox::MaxComboBox;
         i++)
    {
        comboBox[i]->setCurrentIndex(0);
    }

    // ListWidgets
    for (int i = XDialogItem::ListWidget::OptionsLw;
         i < XDialogItem::ListWidget::MaxListWidget;
         i++)
    {
        listWidget[i]->clear();
    }

    // Event's grid
    tbEvents->clearContents();
    currentMenuIndex = -1;
    currentItemIndex = -1;

    /* Also, remove rows which aren't default item events */
    for (int i = XANTE_ITEM_DEFAULT_EVENTS; i < tbEvents->rowCount(); i++)
        tbEvents->removeRow(i);
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
    QList<enum XanteItem::Event> events;

    groupBox[XDialogItem::GroupBox::Events]->setChecked(false);
    groupBox[XDialogItem::GroupBox::Events]->setEnabled(true);

    if (item.hasEvents() == false)
        return;

    events.append(XanteItem::Event::Selected);
    events.append(XanteItem::Event::Exit);
    events.append(XanteItem::Event::ValueConfirmed);
    events.append(XanteItem::Event::ValueChanged);
    events.append(XanteItem::Event::ExtraButtonPressed);

    switch (item.type()) {
        case XanteItem::Type::Inputscroll:
            events.append(XanteItem::Event::ValueStrlen);
            events.append(XanteItem::Event::ValueCheck);
            break;

        case XanteItem::Type::SpinnerSync:
        case XanteItem::Type::DotsSync:
            events.append(XanteItem::Event::SyncRoutine);
            break;

        case XanteItem::Type::UpdateObject:
        case XanteItem::Type::Progress:
            events.append(XanteItem::Event::UpdateRoutine);
            break;

        case XanteItem::Type::Custom:
            events.append(XanteItem::Event::CustomFunction);
            break;

        default:
            break;
    }

    groupBox[XDialogItem::GroupBox::Events]->setChecked(true);
    QListIterator<enum XanteItem::Event> i(events);
    int row = 0;

    while (i.hasNext()) {
        QTableWidgetItem *it = new QTableWidgetItem(
            QString(item.event(i.next()))
        );

        tbEvents->setItem(row, 0, it);
        row += 1;
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

/*    if (type == XanteItem::Type::InputString) {
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
    }*/
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

    try {
        for (int i = 0; i < jtf.totalMenus(); i++) {
            XanteMenu m = jtf.menuAt(i);
            cb->addItem(m.name());
        }

        XanteMenu m = jtf.getMenu(item.referencedMenu());
        cb->setCurrentIndex(cb->findText(m.name()));
    } catch (std::exception &e) {
        // TODO: Handle exception here
    }
}

/*
 * In possession of the selected item, its content is used to fill the widgets
 * on the screen.
 *
 * Therefore, in addition to filling the data into the widgets we also enable and
 * disable some of them.
 */
void XDialogItem::setupWidgets(void)
{
    XanteProject &project = XMainWindow::getProject();
    XanteJTF &jtf = project.getJtf();

    try {
        XanteMenu menu = jtf.menuAt(currentMenuIndex);
        XanteItem &item = menu.itemAt(currentItemIndex);

        /* Prepare dialog widgets for item */
        selectItemType(item.type());

        /* Common information */
        lineEdit[XDialogItem::LineEdit::Name]->setText(item.name());
        lineEdit[XDialogItem::LineEdit::ObjectId]->setText(item.objectId());
        comboBox[XDialogItem::ComboBox::Type]->setCurrentIndex((int)item.type());
        comboBox[XDialogItem::ComboBox::Mode]->setCurrentIndex((int)item.mode());

        /* Specific information */
        setupConfigWidgets(item);
        setupEventsWidgets(item);
        setupHelpWidgets(item);
        setupInputRangesWidgets(item);
        setupOptionsWidgets(item);
        setupMenuReferences(item, jtf);
    } catch (std::exception &e) {
        /* Ok, something went wrong. Return the index to the first object. */
        currentMenuIndex = 0;
        currentItemIndex = 0;
    }
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

/*
 * Sets the current project that is been edited, so all other information
 * (or selections inside the main list view) may use it. At the same time,
 * sets the current XanteItem to the @selectedMenuIndex and the
 * @selectedItemIndex inside it.
 */
void XDialogItem::setCurrentProject(int selectedMenuIndex,
    int selectedItemIndex)
{
    setSelection(selectedMenuIndex, selectedItemIndex);
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

/*    lineEdit[XDialogItem::LineEdit::InputStringLength]->setEnabled(inputString);
    lineEdit[XDialogItem::LineEdit::InputMin]->setEnabled(inputMin);
    lineEdit[XDialogItem::LineEdit::InputMax]->setEnabled(inputMax);*/
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

/*
 * Enables/Disables widgets according with current item type. We call it @index
 * but it is really its type.
 */
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

        case XanteItem::Type::Spreadsheet:
            /*
             * TODO
             * - config
             * - options (Description)
             */
            break;

        case XanteItem::Type::Range:
            /*
             * TODO
             * - add config
             * - options description
             * - ranges (min / max)
             */
            break;

        case XanteItem::Type::Buildlist:
            /*
             * TODO
             * - add config
             * - default_value
             * - options list
             */
            break;

        case XanteItem::Type::Mixedform:
            /*
             * TODO
             * - options description
             */
            break;

        case XanteItem::Type::FileSelect:
        case XanteItem::Type::DirSelect:
            /*
             * TODO
             * - options description
             */
            break;

        case XanteItem::Type::Inputscroll:
            /*
             * TODO
             * - add config
             * - add default_value
             * - add options description
             * - ranges (string_length)
             * - add event row (value-strlen)
             * - add event row (value-check)
             */
            break;

        case XanteItem::Type::Tailbox:
            /*
             * TODO
             * - add default_value
             */
            break;

        case XanteItem::Type::Scrolltext:
            /*
             * TODO
             * - add options description
             * - add default_value
             */
            break;

        case XanteItem::Type::SpinnerSync:
        case XanteItem::Type::DotsSync:
            /*
             * TODO
             * - add event row (sync-routine)
             * - add event row (item-custom-data)
             * - options description
             * - ranges (max)
             */
            break;

        case XanteItem::Type::UpdateObject:
        case XanteItem::Type::Progress:
            /*
             * TODO
             * - add event row (update-routine)
             * - add event row (item-custom-data)
             * - options description
             * - progress only:
             *      - ranges (max)
             */
            break;

        case XanteItem::Type::Custom:
            /*
             * TODO
             * - add event row (custom-event)
             */
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

void XDialogItem::updateXanteItemEvents(XanteItem &item)
{
    qDebug() << tbEvents->rowCount();
    for (int i = 0; i < tbEvents->rowCount(); i++) {
        QTableWidgetItem *it = tbEvents->item(i, 0);

        if (NULL == it)
            continue;

        QString fname = it->text();

        if (fname.isEmpty() == false)
            item.event(fname, static_cast<enum XanteItem::Event>(i));
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
//            data = lineEdit[XDialogItem::LineEdit::InputStringLength]->text();
//            item.stringLength(data.toInt());
            break;

        case XanteItem::Type::InputInt:
        case XanteItem::Type::InputFloat:
/*            min = lineEdit[XDialogItem::LineEdit::InputMin]->text();
            max = lineEdit[XDialogItem::LineEdit::InputMax]->text();

            if (type == XanteItem::Type::InputInt)
                item.minMax(min.toInt(), max.toInt());
            else
                item.minMax(min.toFloat(), max.toFloat());
*/
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
    enum XanteAccessMode mode =
        (enum XanteAccessMode)comboBox[XDialogItem::ComboBox::Mode]->currentIndex();

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

    /* button labels */

    /* extra button */

    /* geometry */
    return item;
}

/**
 * @name updateXanteItem
 * @brief Updates the current edited XanteItem data.
 *
 * This function is responsible to update the current edited XanteItem
 * information with data from the widgets.
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

