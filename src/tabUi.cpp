
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

TabUi::TabUi(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *h = new QHBoxLayout;

    h->addLayout(createExtraFeaturesWidgets());
    h->addWidget(createBtnLabelsWidgets());
    h->addWidget(createItemHelpWidgets());

    setLayout(h);
}

QGroupBox *TabUi::createGeometryWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h;
    QLabel *label;

    gGeometry = new QGroupBox(tr("Geometry"));

    h = new QHBoxLayout;
    label = new QLabel(tr("Width:"));
    sbWidth = new QSpinBox;
    connect(sbWidth, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    sbWidth->setRange(30, 60); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sbWidth);
    v->addLayout(h);

    h = new QHBoxLayout;
    label = new QLabel(tr("Height:"));
    sbHeight = new QSpinBox;
    connect(sbHeight, SIGNAL(valueChanged(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    sbHeight->setRange(30, 60); // FIXME: adjust limit
    h->addWidget(label);
    h->addWidget(sbHeight);
    v->addLayout(h);
    v->setAlignment(Qt::AlignTop);

    gGeometry->setCheckable(true);
    gGeometry->setChecked(false);
    gGeometry->setLayout(v);

    return gGeometry;
}

QVBoxLayout *TabUi::createExtraFeaturesWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *v = new QVBoxLayout;
    QLabel *l;

    /* Extra button */
    cbExtraButton = new QCheckBox(tr("Use Extra button"));
    v->addWidget(cbExtraButton);

    /* Window Title */
    l = new QLabel(tr("Window title:"));
    leTitle = new QLineEdit;
    connect(leTitle, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    l->setBuddy(leTitle);
    h->addWidget(l);
    h->addWidget(leTitle);
    v->addLayout(h);

    /* Geometry */
    v->addWidget(createGeometryWidgets());

    return v;
}

QGroupBox *TabUi::createBtnLabelsWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h;
    QLabel *label;

    /* ok */
    h = new QHBoxLayout;
    label = new QLabel(tr("Ok:"));
    leBtnOk = new QLineEdit;
    connect(leBtnOk, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBtnOk);
    h->addWidget(label);
    h->addWidget(leBtnOk);
    v->addLayout(h);

    /* cancel */
    h = new QHBoxLayout;
    label = new QLabel(tr("Cancel:"));
    leBtnCancel = new QLineEdit;
    connect(leBtnCancel, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBtnCancel);
    h->addWidget(label);
    h->addWidget(leBtnCancel);
    v->addLayout(h);

    /* extra */
    h = new QHBoxLayout;
    label = new QLabel(tr("Extra:"));
    leBtnExtra = new QLineEdit;
    connect(leBtnExtra, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBtnExtra);
    h->addWidget(label);
    h->addWidget(leBtnExtra);
    v->addLayout(h);

    /* help */
    h = new QHBoxLayout;
    label = new QLabel(tr("Help:"));
    leBtnHelp = new QLineEdit;
    connect(leBtnHelp, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBtnHelp);
    h->addWidget(label);
    h->addWidget(leBtnHelp);
    v->addLayout(h);
    v->setAlignment(Qt::AlignTop);

    gButtons = new QGroupBox(tr("Buttons"));
    gButtons->setCheckable(true);
    gButtons->setChecked(false);
    gButtons->setLayout(v);

    return gButtons;
}

QGroupBox *TabUi::createItemHelpWidgets(void)
{
    QLabel *label;
    QVBoxLayout *v = new QVBoxLayout,
                *vbuttons = new QVBoxLayout;
    QHBoxLayout *hbuttons = new QHBoxLayout,
                *hdescription = new QHBoxLayout,
                *hbrief = new QHBoxLayout;

    label = new QLabel(tr("Brief:"));
    leBrief = new QLineEdit;
    connect(leBrief, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leBrief);
    hbrief->addWidget(label);
    hbrief->addWidget(leBrief);

    label = new QLabel(tr("Description:"));
    leDescription = new QLineEdit;
    connect(leDescription, SIGNAL(textEdited(const QString &)), this,
            SLOT(contentChanged(const QString &)));

    label->setBuddy(leDescription);
    hdescription->addWidget(label);
    hdescription->addWidget(leDescription);

    btAdd = new QPushButton(tr("Add"));
    btDel = new QPushButton(tr("Remove"));
    connect(btAdd, SIGNAL(clicked()), this, SLOT(addOption()));
    connect(btDel, SIGNAL(clicked()), this, SLOT(delOption()));
    vbuttons->addWidget(btAdd, 0, Qt::AlignBottom);
    vbuttons->addWidget(btDel, 0, Qt::AlignTop);

    lwOptions = new QListWidget;
    hbuttons->addWidget(lwOptions);
    hbuttons->addLayout(vbuttons);

    QGroupBox *g = new QGroupBox();
    g->setLayout(hbuttons);

    v->addLayout(hbrief);
    v->addLayout(hdescription);
    v->addWidget(g);

    gHelp = new QGroupBox(tr("Help details")),
    gHelp->setCheckable(true);
    gHelp->setChecked(false);
    gHelp->setLayout(v);

    return gHelp;
}

void TabUi::setSelectedItem(const XanteItem &item)
{
    enum XanteItem::Type type = item.type();
    QString tmp;

    if (item.hasHelp())
        gHelp->setChecked(true);

    if (XanteItem::needsOptions(type)) {
        int total = item.totalHelpOptions();

        for (int i = 0; i < total; i++)
            lwOptions->addItem(item.helpOption(i));
    }

    tmp = item.briefHelp();

    if (tmp.isEmpty() == false)
        leBrief->setText(tmp);

    tmp = item.descriptiveHelp();

    if (tmp.isEmpty() == false)
        leDescription->setText(tmp);

    tmp = item.title();

    if (tmp.isEmpty() == false)
        leTitle->setText(tmp);

    if (item.hasButtons()) {
        gButtons->setChecked(true);
        tmp = item.okLabel();

        if (tmp.isEmpty() == false)
            leBtnOk->setText(tmp);

        tmp = item.cancelLabel();

        if (tmp.isEmpty() == false)
            leBtnCancel->setText(tmp);

        tmp = item.extraLabel();

        if (tmp.isEmpty() == false)
            leBtnExtra->setText(tmp);

        tmp = item.helpLabel();

        if (tmp.isEmpty() == false)
            leBtnHelp->setText(tmp);
    }

    cbExtraButton->setChecked(item.btnExtraEnabled());

    if (item.hasGeometry()) {
        QSize s = item.size();
        sbWidth->setValue(s.width());
        sbHeight->setValue(s.height());
        gGeometry->setChecked(true);
    }
}

void TabUi::updateSelectedItem(XanteItem &item)
{
    QString tmp;

    if (gGeometry->isChecked()) {
        QSize s(sbWidth->value(), sbHeight->value());
        item.size(s);
    }

    if (gButtons->isChecked()) {
        tmp = leBtnOk->text();

        if (tmp.isEmpty() == false)
            item.okLabel(tmp);

        tmp = leBtnCancel->text();

        if (tmp.isEmpty() == false)
            item.cancelLabel(tmp);

        tmp = leBtnExtra->text();

        if (tmp.isEmpty() == false)
            item.extraLabel(tmp);

        tmp = leBtnHelp->text();

        if (tmp.isEmpty() == false)
            item.helpLabel(tmp);
    }

    if (gHelp->isChecked()) {
        tmp = leBrief->text();

        if (tmp.isEmpty() == false)
            item.briefHelp(tmp);

        tmp = leDescription->text();

        if (tmp.isEmpty() == false)
            item.descriptiveHelp(tmp);

        if (XanteItem::needsOptions(item.type())) {
            for (int i = 0; i < lwOptions->count(); i++)
                item.helpOption(lwOptions->item(i)->text());
        }
    }

    item.btnExtraEnabled(cbExtraButton->isChecked());
}

void TabUi::clearCurrentData(void)
{
    gGeometry->setChecked(false);
    gButtons->setChecked(false);
    gHelp->setChecked(false);
    cbExtraButton->setChecked(false);
    lwOptions->setEnabled(false);
    btAdd->setEnabled(false);
    btDel->setEnabled(false);
    lwOptions->clear();
    leTitle->clear();
    leBtnOk->clear();
    leBtnCancel->clear();
    leBtnExtra->clear();
    leBtnHelp->clear();
    leDescription->clear();
    leBrief->clear();
}

void TabUi::prepareWidgets(int type)
{
    enum XanteItem::Type t = (enum XanteItem::Type)type;

    lwOptions->setEnabled(XanteItem::needsOptions(t));
    btAdd->setEnabled(XanteItem::needsOptions(t));
    btDel->setEnabled(XanteItem::needsOptions(t));
}

void TabUi::addOption(void)
{
    bool ok;
    QString option = QInputDialog::getText(this, tr("New option"),
                                           tr("Enter the new option brief help:"),
                                           QLineEdit::Normal, "", &ok);

    if ((ok == false) || (option.isEmpty()))
        return;

    lwOptions->addItem(option);
    notifyChange();
}

void TabUi::delOption(void)
{
    int row = lwOptions->currentRow();

    if (row < 0)
        return;

    lwOptions->takeItem(row);
    notifyChange();
}

void TabUi::notifyChange(void)
{
    emit dataChanged();
}

void TabUi::contentChanged(const QString &value)
{
    Q_UNUSED(value);
    notifyChange();
}

