
/*
 * Description:
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

TabContent::TabContent(const XanteBuilderConfig &config, QWidget *parent)
    : QWidget(parent), config(config)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* We're going to use this to change mandatory fields color */
    groups = QVector<QGroupBox *>(TabContent::GroupBox::MaxGroupBox);
    labels = QVector<QLabel *>(TabContent::Label::MaxLabel);
    connect(parent, SIGNAL(newSettings()), this, SLOT(handleNewSettings()));

    h->addWidget(createItemOptionsWidgets());
    h->addLayout(createContentWidgets());

    setLayout(h);
}

QGroupBox *TabContent::createItemOptionsWidgets(void)
{
    QLabel *label;
    QVBoxLayout *vbuttons = new QVBoxLayout,
                *v = new QVBoxLayout;;
    QHBoxLayout *hdescription = new QHBoxLayout,
                *hbuttons = new QHBoxLayout;

    gOptionsList = new QGroupBox(tr("List:"));
    groups[TabContent::GroupBox::OptionsList] = gOptionsList;
    label = new QLabel(tr("Description:"));
    labels[TabContent::Label::Description] = label;
    leDescription = new QLineEdit;
    connect(leDescription, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leDescription);
    hdescription->addWidget(label);
    hdescription->addWidget(leDescription);

    btAdd = new QPushButton(tr("Add"));
    btDel = new QPushButton(tr("Remove"));
    btDefaultValue = new QPushButton(tr("Set default value"));
    btDefaultValue->setVisible(false);
    vbuttons->addWidget(btAdd, 0, Qt::AlignBottom);
    vbuttons->addWidget(btDel, 1, Qt::AlignTop);
    vbuttons->addWidget(btDefaultValue, 0, Qt::AlignBottom);
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addOption()));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delOption()));
    connect(btDefaultValue, SIGNAL(clicked()), this, SLOT(setDefaultValue()));

    lwOptions = new QListWidget;
    hbuttons->addWidget(lwOptions);
    hbuttons->addLayout(vbuttons);
    gOptionsList->setLayout(hbuttons);

    v->addLayout(hdescription);
    v->addWidget(gOptionsList);

    QGroupBox *g= new QGroupBox(tr("Options"));
    g->setLayout(v);

    return g;
}

QGroupBox *TabContent::createItemConfigurationWidgets(void)
{
    QLabel *label;
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h;

    h = new QHBoxLayout;
    label = new QLabel(tr("Block:"));
    labels[TabContent::Label::Block] = label;
    leBlock = new QLineEdit;
    connect(leBlock, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBlock);
    h->addWidget(label);
    h->addWidget(leBlock);
    v->addLayout(h);

    h = new QHBoxLayout;
    label = new QLabel(tr("Entry:"));
    labels[TabContent::Label::Entry] = label;
    leEntry = new QLineEdit;
    connect(leEntry, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leEntry);
    h->addWidget(label);
    h->addWidget(leEntry);
    v->addLayout(h);
    v->setAlignment(Qt::AlignTop);

    gSettings = new QGroupBox(tr("Settings"));
    gSettings->setLayout(v);

    return gSettings;
}

QGroupBox *TabContent::createRangesWidgets(void)
{
    QLabel *label;
    QHBoxLayout *h;
    QVBoxLayout *v = new QVBoxLayout;

    /* String length */
    h = new QHBoxLayout;
    label = new QLabel(tr("String length:"));
    labels[TabContent::Label::StringLength] = label;
    sbStringLength = new QSpinBox;
    connect(sbStringLength, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    sbStringLength->setRange(1, 1024);
    h->addWidget(label);
    h->addWidget(sbStringLength);
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    label = new QLabel(tr("Min:"));
    labels[TabContent::Label::Min] = label;
    sbMin = new QSpinBox;
    connect(sbMin, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    sbMin->setRange(1, 1024); // FIXME: adjust limit
    dsbMin = new QDoubleSpinBox;
    connect(dsbMin, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    dsbMin->setRange(1, 1024); // FIXME: adjust limit
    dsbMin->setVisible(false);
    h->addWidget(label);
    h->addWidget(sbMin);
    h->addWidget(dsbMin);
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    label = new QLabel(tr("Max:"));
    labels[TabContent::Label::Max] = label;
    sbMax = new QSpinBox;
    connect(sbMax, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    sbMax->setRange(1, 1024); // FIXME: adjust limit
    dsbMax = new QDoubleSpinBox;
    connect(dsbMax, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    dsbMax->setRange(1, 1024); // FIXME: adjust limit
    dsbMax->setVisible(false);
    h->addWidget(label);
    h->addWidget(sbMax);
    h->addWidget(dsbMax);
    v->addLayout(h);
    v->setAlignment(Qt::AlignTop);

    QGroupBox *g = new QGroupBox(tr("Input ranges"));
    g->setLayout(v);

    return g;
}

QHBoxLayout *TabContent::createRangesAndSettingsWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    /* Add Settings and Ranges */
    h->addWidget(createItemConfigurationWidgets());
    h->addWidget(createRangesWidgets());

    return h;
}

QVBoxLayout *TabContent::createContentWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h = new QHBoxLayout;

    /* Referenced Menu*/
    QLabel *label = new QLabel(tr("Referenced menu:"));
    labels[TabContent::Label::ReferencedMenu] = label;
    cbReferencedMenu = new QComboBox;
    h->addWidget(label);
    h->addWidget(cbReferencedMenu);
    v->addLayout(h);

    /* Default Value */
    h = new QHBoxLayout;
    label = new QLabel(tr("Default value:"));
    labels[TabContent::Label::DefaultValue] = label;
    leDefaultValue = new QLineEdit;
    connect(leDefaultValue, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leDefaultValue);
    cbDefaultValue = new QComboBox;
    cbDefaultValue->setVisible(false);
    dtDefaultValue = new QDateTimeEdit(QDateTime::currentDateTime());
    dtDefaultValue->setVisible(false);
    h->addWidget(label);
    h->addWidget(leDefaultValue);
    h->addWidget(cbDefaultValue);
    h->addWidget(dtDefaultValue);
    v->addLayout(h);

    /* Ranges and Settings */
    v->addLayout(createRangesAndSettingsWidgets());

    return v;
}

void TabContent::setSelectedItem(const XanteItem &item)
{
    enum XanteItem::Type type = item.type();

    leBlock->setText(item.configBlock());
    leEntry->setText(item.configEntry());
    leDefaultValue->setText(item.defaultValue());

    if (item.hasOptions()) {
        if ((type == XanteItem::Type::Checklist) ||
            (type == XanteItem::Type::RadioChecklist))
        {
            for (int i = 0; i < item.totalOptions(); i++) {
                QString data = item.option(i);
                lwOptions->addItem(data);
                cbDefaultValue->addItem(data);
            }

            if (type == XanteItem::Type::RadioChecklist)
                cbDefaultValue->setCurrentIndex(item.defaultValue().toInt());
        } else
            leDescription->setText(item.option());
    }

    if (item.type() == XanteItem::Type::Menu) {
        XanteProject &project = XMainWindow::getProject();
        XanteJTF &jtf = project.getJtf();

        cbReferencedMenu->clear();

        try {
            for (int i = 0; i < jtf.totalMenus(); i++) {
                XanteMenu m = jtf.menuAt(i);
                cbReferencedMenu->addItem(m.name());
            }

            XanteMenu m = jtf.getMenu(item.referencedMenu());
            cbReferencedMenu->setCurrentIndex(cbReferencedMenu->findText(m.name()));
        } catch (std::exception &e) {
            // TODO: Handle exception here
        }
    }

    if (item.hasInputRanges()) {
        if (XanteItem::needsStringLengthRange(type)) {
            sbStringLength->setValue(item.stringLength());
        } else {
            QPair<QVariant, QVariant> mm = item.minMax();

            if (type == XanteItem::Type::InputFloat) {
                dsbMin->setValue(mm.first.toDouble());
                dsbMax->setValue(mm.second.toDouble());
            } else {
                sbMin->setValue(mm.first.toInt());
                sbMax->setValue(mm.second.toInt());
            }
        }
    }

    /* We can notify changes now */
    mayNotify = true;
}

void TabContent::updateSelectedItem(XanteItem &item)
{
    enum XanteItem::Type type = item.type();
    int min = 0, max = 0;
    QString error;

    if (XanteItem::needsSettings(type)) {
        item.configBlock(leBlock->text());
        item.configEntry(leEntry->text());

        if (item.configBlock().isEmpty())
            error += QString("<li>Config Block</li>");

        if (item.configEntry().isEmpty())
            error += QString("<li>Config Entry</li>");
    }

    if (XanteItem::needsDefaultValue(type)) {
        if (type == XanteItem::Type::RadioChecklist)
            item.defaultValue(QString("%1").arg(cbDefaultValue->currentIndex()));
        else if (type == XanteItem::Type::Calendar)
            item.defaultValue(dtDefaultValue->date().toString("yyyy-MM-dd"));
        else if (type == XanteItem::Type::Timebox)
            item.defaultValue(dtDefaultValue->time().toString("HH:mm:ss"));
        else
            item.defaultValue(leDefaultValue->text());

        if (item.defaultValue().isEmpty())
            error += QString("<li>Default value</li>");
    }

    if (XanteItem::needsDescription(type))
        item.option(leDescription->text());
    else if (XanteItem::needsOptions(type)) {
        for (int i = 0; i < lwOptions->count(); i++)
            item.option(lwOptions->item(i)->text());
    }

    if (XanteItem::needsMenuReference(type)) {
        XanteProject &project = XMainWindow::getProject();
        XanteJTF &jtf = project.getJtf();
        XanteMenu m = jtf.menuAt(cbReferencedMenu->currentIndex());
        item.referencedMenu(m.objectId());
    }

    if (XanteItem::needsRange(type)) {
        if (XanteItem::needsStringLengthRange(type))
            item.stringLength(sbStringLength->value());

        if (type == XanteItem::Type::InputFloat)
            item.minMax(dsbMin->value(), dsbMax->value());
        else {
            if ((XanteItem::needsMinRange(type)) ||
                (XanteItem::needsMaxRange(type)))
            {
                if (XanteItem::needsMinRange(type))
                    min = sbMin->value();

                if (XanteItem::needsMaxRange(type))
                    max = sbMax->value();

                item.minMax(min, max);
            }
        }
    }

    if (!error.isEmpty())
        throw std::runtime_error(error.toLatin1().data());

    return;
}

void TabContent::clearCurrentData(void)
{
    /* And now we don't */
    mayNotify = false;

    lwOptions->clear();
    leDescription->clear();
    leDefaultValue->clear();
    leBlock->clear();
    leEntry->clear();
    cbReferencedMenu->setCurrentIndex(0);

    sbMin->setEnabled(false);
    sbMax->setEnabled(false);
    sbStringLength->setEnabled(false);

    gSettings->setEnabled(false);
    gOptionsList->setEnabled(false);
    leDescription->setEnabled(false);
    leDefaultValue->setEnabled(false);

    cbDefaultValue->clear();
    selectedLabels.clear();
    selectedGroup.clear();

    for (QVector<QLabel *>::iterator i = labels.begin(); i != labels.end(); ++i)
        (*i)->setStyleSheet(QString(""));

    for (QVector<QGroupBox *>::iterator i = groups.begin(); i != groups.end(); ++i)
        (*i)->setStyleSheet(QString(""));
}

void TabContent::prepareDefaultValue(enum XanteItem::Type type)
{
    leDefaultValue->setVisible(false);
    cbDefaultValue->setVisible(false);
    btDefaultValue->setVisible(false);
    dtDefaultValue->setVisible(false);
    dtDefaultValue->setCalendarPopup(false);

    switch (type) {
        case XanteItem::Type::RadioChecklist:
            cbDefaultValue->setVisible(true);
            break;

        case XanteItem::Type::Calendar:
            dtDefaultValue->setVisible(true);
            dtDefaultValue->setCalendarPopup(true);
            dtDefaultValue->setDisplayFormat(QString("yyyy-MM-dd"));
            break;

        case XanteItem::Type::Timebox:
            dtDefaultValue->setVisible(true);
            dtDefaultValue->setDisplayFormat(QString("HH:mm:ss"));
            break;

        default:
            if (type == XanteItem::Type::Checklist)
                btDefaultValue->setVisible(true);

            leDefaultValue->setVisible(true);
            break;
    }
}

void TabContent::prepareMandatoryFields(enum XanteItem::Type type)
{
    if (XanteItem::needsMenuReference(type))
        selectedLabels.append(labels[TabContent::Label::ReferencedMenu]);

    if (XanteItem::needsDescription(type))
        selectedLabels.append(labels[TabContent::Label::Description]);

    if (XanteItem::needsSettings(type)) {
        selectedLabels.append(labels[TabContent::Label::Block]);
        selectedLabels.append(labels[TabContent::Label::Entry]);
    }

    if (XanteItem::needsDefaultValue(type))
        selectedLabels.append(labels[TabContent::Label::DefaultValue]);

    if (XanteItem::needsStringLengthRange(type))
        selectedLabels.append(labels[TabContent::Label::StringLength]);

    if (XanteItem::needsMinRange(type))
        selectedLabels.append(labels[TabContent::Label::Min]);

    if (XanteItem::needsMaxRange(type))
        selectedLabels.append(labels[TabContent::Label::Max]);

    if (XanteItem::needsOptions(type))
        selectedGroup.append(groups[TabContent::GroupBox::OptionsList]);
}

void TabContent::prepareWidgets(enum XanteItem::Type type)
{
    bool showFloatField = false;

    /*
     * Set which type of Widget we're going to use to adjust the item's
     * default value.
     */
    prepareDefaultValue(type);

    /*
     * Adjust the QLabels which are going to be in different color as the
     * mandatory fields.
     */
    prepareMandatoryFields(type);

    /*
     * Adjusts how the rows will be selected inside the list of options for
     * a checklist widget.
     */
    if (type == XanteItem::Type::RadioChecklist)
        lwOptions->setSelectionMode(QAbstractItemView::SingleSelection);
    else
        lwOptions->setSelectionMode(QAbstractItemView::ExtendedSelection);

    cbReferencedMenu->setEnabled(XanteItem::needsMenuReference(type));
    leDescription->setEnabled(XanteItem::needsDescription(type));
    leDefaultValue->setEnabled(XanteItem::needsDefaultValue(type) &&
                               (type != XanteItem::Type::Checklist));

    lwOptions->setEnabled(XanteItem::needsOptions(type));
    gOptionsList->setEnabled(XanteItem::needsOptions(type));
    gSettings->setEnabled(XanteItem::needsSettings(type));
    btAdd->setEnabled(XanteItem::needsOptions(type));
    btDel->setEnabled(XanteItem::needsOptions(type));
    btDefaultValue->setEnabled(XanteItem::needsOptions(type));

    if (type == XanteItem::Type::InputFloat)
        showFloatField = true;

    dsbMin->setVisible(showFloatField);
    dsbMax->setVisible(showFloatField);
    sbMin->setVisible(!showFloatField);
    sbMax->setVisible(!showFloatField);

    dsbMin->setEnabled(XanteItem::needsMinRange(type));
    dsbMax->setEnabled(XanteItem::needsMaxRange(type));
    sbMin->setEnabled(XanteItem::needsMinRange(type));
    sbMax->setEnabled(XanteItem::needsMaxRange(type));
    sbStringLength->setEnabled(XanteItem::needsStringLengthRange(type));

    handleNewSettings();
}

void TabContent::addOption(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New option"),
                                           tr("Enter the new option name:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    lwOptions->addItem(option);
    notifyChange();
}

void TabContent::delOption(void)
{
    int row = lwOptions->currentRow();

    if (row < 0)
        return;

    lwOptions->takeItem(row);
    notifyChange();
}

void TabContent::setDefaultValue(void)
{
    int value = 0;

    for (int i = 0; i < lwOptions->count(); i++)
        if (lwOptions->item(i)->isSelected())
            value += (int)pow(2, i);

    leDefaultValue->setText(QString("%1").arg(value));
    notifyChange();
}

void TabContent::notifyChange(void)
{
    emit dataChanged();
}

void TabContent::contentChanged(const QString &value)
{
    Q_UNUSED(value);

    if (mayNotify)
        notifyChange();
}


/*
 * Since we received this notification we must update the color of the mandatory
 * fields so the user may notice them.
 */
void TabContent::handleNewSettings(void)
{
    QList<QLabel *>::iterator i;
    QList<QGroupBox *>::iterator ii;

    for (i = selectedLabels.begin(); i != selectedLabels.end(); ++i)
        (*i)->setStyleSheet(QString("QLabel { color : %1 }")
                                     .arg(config.mandatoryFieldColor()));

    for (ii = selectedGroup.begin(); ii != selectedGroup.end(); ++ii)
        (*ii)->setStyleSheet(QString("QGroupBox { color : %1 }")
                                     .arg(config.mandatoryFieldColor()));

}

