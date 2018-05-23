
/*
 * Description: The main tab of a XanteItem.
 *
 * Author: Rodrigo Freitas
 * Created at: Tue Apr  3 13:54:53 -03 2018
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

TabDetails::TabDetails(const XanteBuilderConfig &config, QWidget *parent)
    : QWidget(parent), config(config)
{
    /* We're going to use this to change mandatory fields color */
    labels = QVector<QLabel *>(TabDetails::Label::MaxLabel);
    connect(parent, SIGNAL(newSettings()), this, SLOT(handleNewSettings()));
    setLayout(createMainWidgets());
}

QVBoxLayout *TabDetails::createMainWidgets(void)
{
    QHBoxLayout *h;
    QVBoxLayout *v = new QVBoxLayout;
    QLabel *label;

    h = new QHBoxLayout;

    /* name */
    label = new QLabel(tr("Name:"));
    labels[TabDetails::Label::Name] = label;
    leName = new QLineEdit;
    connect(leName, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leName);
    h->addWidget(label);
    h->addWidget(leName);

    /* objectId */
    label = new QLabel(tr("Object ID:"));
    leObjectId = new QLineEdit;
    label->setBuddy(leObjectId);
    leObjectId->setEnabled(false);
    h->addWidget(label);
    h->addWidget(leObjectId);
    v->addLayout(h);

    h = new QHBoxLayout;

    /* type */
    label = new QLabel(tr("Item type:"));
    labels[TabDetails::Label::Type] = label;
    cbType = new QComboBox;
    connect(cbType, SIGNAL(activated(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(cbType);

    for (unsigned int i = 1; i < ITEM_TYPE; i++)
        cbType->addItem(QString(cbItemTypeName[i]));

    h->addWidget(label);
    h->addWidget(cbType);
    connect(cbType, SIGNAL(activated(int)), this, SLOT(selectItemType(int)));

    /* mode */
    label = new QLabel(tr("Access mode:"));
    labels[TabDetails::Label::Mode] = label;
    cbMode = new QComboBox;
    connect(cbMode, SIGNAL(activated(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    for (unsigned int i = 0; i < ACCESS_MODE; i++)
        cbMode ->addItem(QString(cbAccessModeName[i]));

    h->addWidget(label);
    h->addWidget(cbMode);
    v->addLayout(h);
    v->setAlignment(Qt::AlignTop);

    return v;
}

void TabDetails::setSelectedItem(const XanteItem &item)
{
    /* name */
    leName->setText(item.name());

    /* objectId */
    leObjectId->setText(item.objectId());

    /* type */
    cbType->setCurrentIndex((int)item.type());

    /* mode */
    cbMode->setCurrentIndex((int)item.mode());
}

void TabDetails::updateSelectedItem(XanteItem &item)
{
    QString data;

    /* name */
    data = leName->text();

    if (data.isEmpty())
        throw std::runtime_error(tr("<li>Name</li>").toLocal8Bit().data());

    item.name(data);

    /* type */
    enum XanteItem::Type type = (enum XanteItem::Type)cbType->currentIndex();
    item.type(type);

    /* access mode */
    enum XanteAccessMode mode = (enum XanteAccessMode)cbMode->currentIndex();
    item.mode(mode);
}

void TabDetails::prepareWidgets(enum XanteItem::Type type)
{
    Q_UNUSED(type);
    handleNewSettings();
}

void TabDetails::clearCurrentData(void)
{
    leName->clear();
    leObjectId->clear();
    cbType->setCurrentIndex(0);
    cbMode->setCurrentIndex(0);
}

/*
 * Enables/Disables widgets according current item type. We call it @index
 * but it is really its type.
 */
void TabDetails::selectItemType(int index)
{
    emit itemTypeChanged(index);
}

void TabDetails::notifyChange(void)
{
    emit dataChanged();
}

void TabDetails::contentChanged(const QString &value)
{
    Q_UNUSED(value);
    notifyChange();
}

/*
 * Since we received this notification we must update the color of the mandatory
 * fields so the user may notice them.
 */
void TabDetails::handleNewSettings(void)
{
    QVector<QLabel *>::iterator i;

    for (i = labels.begin(); i != labels.end(); ++i)
        (*i)->setStyleSheet(QString("QLabel { color : %1 }")
                                     .arg(config.mandatoryFieldColor()));
}

