
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

TabSpreadsheet::TabSpreadsheet(const XanteBuilderConfig &config, QWidget *parent)
    : QWidget(parent), config(config)
{
    QVBoxLayout *v = new QVBoxLayout;

    connect(parent, SIGNAL(newSettings()), this, SLOT(handleNewSettings()));

    /* Layout */
    v->addLayout(createTitleWidgets());
    v->addWidget(createColumnWidgets());
    v->addWidget(createRowWidgets());

    setLayout(v);
}

QHBoxLayout *TabSpreadsheet::createTitleWidgets(void)
{
    QHBoxLayout *h = new QHBoxLayout;
    QLabel *l;

    l = new QLabel(tr("Title:"));
    labels.append(l);
    leTitle = new QLineEdit;
    l->setBuddy(leTitle);
    h->addWidget(l);
    h->addWidget(leTitle);

    l = new QLabel(tr("Number of columns:"));
    labels.append(l);
    sbColumn = new QSpinBox;
    connect(sbColumn, SIGNAL(valueChanged(int)), this, SLOT(columnsChanged(int)));
    sbColumn->blockSignals(true);
    sbColumn->setRange(1, 30);
    sbColumn->blockSignals(false);
    l->setBuddy(sbColumn);
    h->addWidget(l);
    h->addWidget(sbColumn);

    l = new QLabel(tr("Number of rows:"));
    labels.append(l);
    sbRow = new QSpinBox;
    connect(sbRow, SIGNAL(valueChanged(int)), this, SLOT(rowsChanged(int)));
    sbRow->blockSignals(true);
    sbRow->setRange(1, 30);
    sbRow->blockSignals(false);
    l->setBuddy(sbRow);
    h->addWidget(l);
    h->addWidget(sbRow);

    return h;
}

QGroupBox *TabSpreadsheet::createColumnWidgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Columns"));
    QVBoxLayout *v = new QVBoxLayout;

    tbColumn = new QTableWidget(0, 1);
    tbColumn->setVerticalHeaderLabels(QStringList() << tr("Title"));
    v->addWidget(tbColumn);

    g->setLayout(v);
    groups.append(g);

    return g;
}

QGroupBox *TabSpreadsheet::createRowWidgets(void)
{
    QGroupBox *g = new QGroupBox(tr("Row"));
    QVBoxLayout *v = new QVBoxLayout;

    tbRow = new QTableWidget(0, 1);
    tbRow->setHorizontalHeaderLabels(QStringList() << tr("Title"));

    v->addWidget(tbRow);
    g->setLayout(v);
    groups.append(g);

    return g;
}

void TabSpreadsheet::assembleColumnTable(int columns)
{
    QStringList title;

    if (columns == -1) {
        // back to default columns
        for (int i = tbColumn->rowCount() - 1; i >= 0; i--)
            tbColumn->removeRow(i);

        return;
    }

    for (int i = 0; i < columns; i++) {
        tbColumn->insertRow(tbColumn->rowCount());
        title << tr("Title");
    }

    tbColumn->setVerticalHeaderLabels(title);
}

void TabSpreadsheet::assembleRowTable(int rows, int columns)
{
    QStringList title;

    if (columns == -1) {
        // back to default rows and columns
        for (int i = tbRow->rowCount() - 1; i >= 0; i--)
            tbRow->removeRow(i);

        for (int i = tbRow->columnCount() - 1; i >= 1; i--)
            tbRow->removeColumn(i);

        return;
    }

    title << tr("Title");

    /* Add new columns */
    for (int i = 0; i < columns; i++) {
        tbRow->insertColumn(tbRow->columnCount());
        title << QString(tr("Value\nof column %1")).arg(i + 1);
    }

    /* Add new rows */
    for (int i = 0; i < rows; i++)
        tbRow->insertRow(tbRow->rowCount());

    tbRow->setHorizontalHeaderLabels(title);
    tbRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void TabSpreadsheet::setSelectedItem(const XanteItem &item)
{
    SpreadsheetData sd(item.option());
    int columns = 1, rows = 1;
    QStringList title;

    leTitle->setText(sd.title());
    rows = sd.rows();
    columns = sd.columns();
    sbRow->blockSignals(true);
    sbRow->setValue(rows);
    sbRow->blockSignals(false);
    sbColumn->blockSignals(true);
    sbColumn->setValue(columns);
    sbColumn->blockSignals(false);

    printf("Monta tabelas com {rows:%d columns:%d}\n", rows, columns);
    printf("%s: {row:%d, column:%d}\n", __FUNCTION__,
            tbRow->rowCount(), tbColumn->rowCount());

    assembleColumnTable(columns);
    assembleRowTable(rows, columns);

    for (int i = 0; i < sd.rows(); i++) {
        SpreadsheetRow row = sd.rowAt(i);
        QTableWidgetItem *it;

        // row title
        it = new QTableWidgetItem(row.title());
        tbRow->setItem(i, 0, it);

        // column values
        for (int j = 0; j < row.columns(); j++) {
            SpreadsheetColumn column = row.columnAt(j);
            it = new QTableWidgetItem(column.defaultValue());
            tbRow->setItem(i, j + 1, it);

            if (i == 0) {
                it = new QTableWidgetItem(column.title());
                tbColumn->setItem(j, 0, it);
            }
        }
    }
}

SpreadsheetData TabSpreadsheet::buildData(void)
{
    SpreadsheetData sd;
    sd.title(leTitle->text());

    printf("%s: {row:%d, column:%d}\n", __FUNCTION__,
            tbRow->rowCount(), tbColumn->rowCount());

    for (int i = 0; i < tbRow->rowCount(); i++) {
        QTableWidgetItem *item;

        /* Initialize the row */
        item = tbRow->item(i, 0);
        SpreadsheetRow r;

        /* Its title */
        r.title(item->text());

        /* Insert its columns */
        for (int j = 0; j < tbColumn->rowCount(); j++) {
            QString cTitle;

            /* Column title (only when we're at the first row) */
            if (i == 0) {
                item = tbColumn->item(j, 0);
                cTitle = item->text();
            }

            /* Column value */
            item = tbRow->item(i, j + 1);

            SpreadsheetColumn c(cTitle, item->text());
            r.addColumn(c);
        }

        /* Add to the table */
        sd.addRow(r);
    }

    return sd;
}

void TabSpreadsheet::updateSelectedItem(XanteItem &item)
{
    SpreadsheetData sd = buildData();
    QJsonObject data = sd.toJson();
    QJsonDocument doc(data);

    item.option(QString(doc.toJson(QJsonDocument::Compact)));
}

void TabSpreadsheet::clearCurrentData(void)
{
    tbColumn->clearContents();
    tbRow->clearContents();
    leTitle->clear();

    assembleColumnTable(-1);
    assembleRowTable(-1, -1);

    tbColumn->setEnabled(false);
    tbRow->setEnabled(false);
    leTitle->setEnabled(false);
    sbColumn->setEnabled(false);
    sbRow->setEnabled(false);
    sbColumn->blockSignals(true);
    sbColumn->setValue(1);
    sbColumn->blockSignals(false);
    sbRow->blockSignals(true);
    sbRow->setValue(1);
    sbRow->blockSignals(false);
}

void TabSpreadsheet::prepareWidgets(enum XanteItem::Type type)
{
    Q_UNUSED(type);

    if (type == XanteItem::Type::Spreadsheet) {
        tbColumn->setEnabled(true);
        tbRow->setEnabled(true);
        leTitle->setEnabled(true);
        sbColumn->setEnabled(true);
        sbRow->setEnabled(true);
    }

    handleNewSettings();
}

void TabSpreadsheet::handleNewSettings(void)
{
    QList<QLabel *>::iterator i;
    QList<QGroupBox *>::iterator ii;

    for (i = labels.begin(); i != labels.end(); ++i)
        (*i)->setStyleSheet(QString("QLabel { color : %1 }")
                                     .arg(config.mandatoryFieldColor()));

    for (ii = groups.begin(); ii != groups.end(); ++ii)
        (*ii)->setStyleSheet(QString("QGroupBox { color : %1 }")
                                     .arg(config.mandatoryFieldColor()));
}

void TabSpreadsheet::columnsChanged(int i)
{
    int diff = 0;
    QStringList title;

    printf("%s\n", __FUNCTION__);
    if ((i == tbColumn->rowCount()) || !i)
        return;

    /* Remove/Add rows from tbColumn and columns from tbRow */
    diff = i - tbColumn->rowCount();

    if (diff > 0) {
        for (int i = 0; i < diff; i++) {
            tbColumn->insertRow(tbColumn->rowCount());
            tbRow->insertColumn(tbRow->columnCount());
        }
    } else {
        for (int i = 0, j = tbColumn->rowCount() - 1, z = tbRow->columnCount() - 1;
             i < abs(diff);
             i++, j--, z--)
        {
            tbColumn->removeRow(j);
            tbRow->removeColumn(z);
        }
    }

    for (int i = 0; i < tbColumn->rowCount(); i++)
        title << tr("Title");

    tbColumn->setVerticalHeaderLabels(title);
    title.clear();
    title << tr("Title");

    for (int i = 0; i < tbColumn->rowCount(); i++)
        title << QString(tr("Value\nof column %1")).arg(i + 1);

    tbRow->setHorizontalHeaderLabels(title);
    tbRow->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    emit dataChanged();
}

void TabSpreadsheet::rowsChanged(int i)
{
    int diff = 0;

    printf("%s\n", __FUNCTION__);

    if ((i == tbRow->rowCount()) || !i)
        return;

    /* Remove/Add rows from tbRow */
    diff = i - tbRow->rowCount();

    if (diff > 0) {
        for (int i = 0; i < diff; i++)
            tbRow->insertRow(tbRow->rowCount());
    } else {
        for (int i = 0, j = tbRow->rowCount() - 1; i < abs(diff); i++, j--)
            tbRow->removeRow(j);
    }

    emit dataChanged();
}

