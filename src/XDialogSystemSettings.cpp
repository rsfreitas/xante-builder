
/*
 * Description: A Dialog to configure the application.
 *
 * Author: Rodrigo Freitas
 * Created at: Mon Apr  2 10:45:05 -03 2018
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
/*
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
*/
#include "xante_builder.hpp"

XDialogSystemSettings::XDialogSystemSettings(XanteBuilderConfig &config, QWidget *parent)
    : QDialog(parent), config(config)
{
    setWindowTitle(tr("System settings"));
    createWidgets();

    /* Fill widgets with config */
    leSourceTemplatePath->setText(config.sourceTemplatePath());
    leJerminusPath->setText(config.jerminusPath());
}

void XDialogSystemSettings::chooseSourceTemplatePath(void)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Source Template"),
                                                    getenv("HOME"),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty() == false)
        leSourceTemplatePath->setText(dir);
}

void XDialogSystemSettings::chooseJerminusPath(void)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Jerminus"),
                                                    getenv("HOME"),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty() == false)
        leJerminusPath->setText(dir);
}

QGroupBox *XDialogSystemSettings::createUtilitiesWidgets(void)
{
    QHBoxLayout *h;
    QLabel *l;
    QPushButton *bt;
    QVBoxLayout *v = new QVBoxLayout;
    QGroupBox *g = new QGroupBox(tr("Utilities"));

    h = new QHBoxLayout;
    l = new QLabel(tr("Source Template location:"));
    leSourceTemplatePath = new QLineEdit;
    l->setBuddy(leSourceTemplatePath);
    bt = new QPushButton(tr("..."));
    connect(bt, SIGNAL(clicked()), this, SLOT(chooseSourceTemplatePath()));
    h->addWidget(l);
    h->addWidget(leSourceTemplatePath);
    h->addWidget(bt);
    v->addLayout(h);

    h = new QHBoxLayout;
    l = new QLabel(tr("Jerminus location:"));
    leJerminusPath = new QLineEdit;
    l->setBuddy(leJerminusPath);
    bt = new QPushButton(tr("..."));
    connect(bt, SIGNAL(clicked()), this, SLOT(chooseJerminusPath()));
    h->addWidget(l);
    h->addWidget(leJerminusPath);
    h->addWidget(bt);
    v->addLayout(h);

    g->setLayout(v);

    return g;
}

void XDialogSystemSettings::confirmOk(void)
{
    QString tmp;

    tmp = leSourceTemplatePath->text();
    config.sourceTemplatePath(tmp);
    tmp = leJerminusPath->text();
    config.jerminusPath(tmp);

    done(0);
}

void XDialogSystemSettings::confirmCancel(void)
{
    done(-1);
}

QHBoxLayout *XDialogSystemSettings::createButtons(void)
{
    QHBoxLayout *h = new QHBoxLayout;

    QPushButton *btnOk = new QPushButton(tr("Ok"));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(confirmOk()));

    QPushButton *btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(confirmCancel()));

    h->addWidget(btnOk);
    h->addWidget(btnCancel);

    return h;
}

void XDialogSystemSettings::createWidgets(void)
{
    QVBoxLayout *v = new QVBoxLayout;

    v->addWidget(createUtilitiesWidgets());
    v->addLayout(createButtons());

    setLayout(v);
}

