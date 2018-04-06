
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

TabContent::TabContent(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *h = new QHBoxLayout;

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
    label = new QLabel(tr("Description:"));
    leDescription = new QLineEdit;
    label->setBuddy(leDescription);
    hdescription->addWidget(label);
    hdescription->addWidget(leDescription);

    QPushButton *btAdd = new QPushButton(tr("Add"));
    QPushButton *btDel = new QPushButton(tr("Remove"));
    vbuttons->addWidget(btAdd, 0, Qt::AlignBottom);
    vbuttons->addWidget(btDel, 0, Qt::AlignTop);
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addOption()));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delOption()));

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
    leBlock = new QLineEdit;
    label->setBuddy(leBlock);
    h->addWidget(label);
    h->addWidget(leBlock);
    v->addLayout(h);

    h = new QHBoxLayout;
    label = new QLabel(tr("Entry:"));
    leEntry = new QLineEdit;
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
    sbStringLength = new QSpinBox;
    sbStringLength->setRange(1, 1024);
    h->addWidget(label);
    h->addWidget(sbStringLength);
    v->addLayout(h);

    /* Min */
    h = new QHBoxLayout;
    label = new QLabel(tr("Min:"));
    sbMin = new QSpinBox;
    sbMin->setRange(1, 1024); // FIXME: adjust limit
    dsbMin = new QDoubleSpinBox;
    dsbMin->setRange(1, 1024); // FIXME: adjust limit
    dsbMin->setVisible(false);
    h->addWidget(label);
    h->addWidget(sbMin);
    h->addWidget(dsbMin);
    v->addLayout(h);

    /* Max */
    h = new QHBoxLayout;
    label = new QLabel(tr("Max:"));
    sbMax = new QSpinBox;
    sbMax->setRange(1, 1024); // FIXME: adjust limit
    dsbMax = new QDoubleSpinBox;
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
    cbReferencedMenu = new QComboBox;
    h->addWidget(label);
    h->addWidget(cbReferencedMenu);
    v->addLayout(h);

    /* Default Value */
    h = new QHBoxLayout;
    label = new QLabel(tr("Default value:"));
    leDefaultValue = new QLineEdit;
    label->setBuddy(leDefaultValue);
    h->addWidget(label);
    h->addWidget(leDefaultValue);
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
            for (int i = 0; i < item.totalOptions(); i++)
                lwOptions->addItem(item.option(i));
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
}

void TabContent::updateSelectedItem(XanteItem &item)
{
    enum XanteItem::Type type = item.type();
    int min = 0, max = 0;

    if (XanteItem::needsSettings(type)) {
        item.configBlock(leBlock->text());
        item.configEntry(leEntry->text());
    }

    if (XanteItem::needsDefaultValue(type))
        item.defaultValue(leDefaultValue->text());

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

    if (XanteItem::needsStringLengthRange(type))
        item.stringLength(sbStringLength->value());

    if (type == XanteItem::Type::InputFloat)
        item.minMax(dsbMin->value(), dsbMax->value());
    else {
        if (XanteItem::needsMinRange(type))
            min = sbMin->value();

        if (XanteItem::needsMaxRange(type))
            max = sbMax->value();

        item.minMax(min, max);
    }
}

void TabContent::clearCurrentData(void)
{
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
}

void TabContent::prepareWidgets(int type)
{
    enum XanteItem::Type t = (enum XanteItem::Type)type;
    bool showFloatField = false;

    cbReferencedMenu->setEnabled(XanteItem::needsMenuReference(t));
    leDescription->setEnabled(XanteItem::needsDescription(t));
    leDefaultValue->setEnabled(XanteItem::needsDefaultValue(t));
    gOptionsList->setEnabled(XanteItem::needsOptions(t));
    gSettings->setEnabled(XanteItem::needsSettings(t));

    if (t == XanteItem::Type::InputFloat)
        showFloatField = true;

    dsbMin->setVisible(showFloatField);
    dsbMax->setVisible(showFloatField);
    sbMin->setVisible(!showFloatField);
    sbMax->setVisible(!showFloatField);

    dsbMin->setEnabled(XanteItem::needsMinRange(t));
    dsbMax->setEnabled(XanteItem::needsMaxRange(t));
    sbMin->setEnabled(XanteItem::needsMinRange(t));
    sbMax->setEnabled(XanteItem::needsMaxRange(t));
    sbStringLength->setEnabled(XanteItem::needsStringLengthRange(t));
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
}

void TabContent::delOption(void)
{
    int row = lwOptions->currentRow();

    if (row < 0)
        return;

    lwOptions->takeItem(row);
}

