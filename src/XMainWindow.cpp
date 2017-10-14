
/*
 * Description: Handles the application main window.
 *
 * Author: Rodrigo Freitas
 * Created at: Thu Jun  8 18:50:19 2017
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

#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QFileDialog>

#include "xante_builder.hpp"

XanteProject *XMainWindow::project = nullptr;

XMainWindow::XMainWindow(XanteConfig &config)
    : config(config)
{
    dialog = new XMainDialog(this);
    connect(dialog, SIGNAL(projectHasChanges()), this, SLOT(projectChanged()));
    createMenu();
    statusBar()->showMessage(APP_NAME);

    setCentralWidget(dialog);
    setWindowTitle(APP_NAME);
    resize(config.getWindowSize());
    move(config.getWindowPosition());
}

void XMainWindow::closeEvent(QCloseEvent *event)
{
    config.setWindowSize(size());
    config.setWindowPosition(pos());
    event->accept();
}

void XMainWindow::newProject()
{
    XProjectWizard projectWizard;

    if (projectWizard.exec()) {
        project = projectWizard.getProject();
        dialog->activeProject(true);
        setWindowWidgetsEnabled(true);
    }
}

void XMainWindow::loadFile(const QString &filename)
{
    setCurrentFile(filename);
    project = new XanteProject(filename);
    setWindowTitle(QString("%1 [%2]").arg(APP_NAME)
                                     .arg(project->getProjectName()));

    dialog->activeProject(true);
    setWindowWidgetsEnabled(true);
}

void XMainWindow::openProject()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file"),
                                                    tr(""),
                                                    tr("Project files (*.pjx)"),
                                                    &selectedFilter, options);

    if (filename.isEmpty() == false)
        loadFile(filename);
}

void XMainWindow::saveProject()
{
    project->save();
    acSave->setEnabled(false);
    setWindowTitle(QString("%1 [%2]").arg(APP_NAME)
                                     .arg(project->getProjectName()));
}

void XMainWindow::closeProject()
{
    if (editingProject == false)
        return;

    dialog->activeProject(false);
    setWindowWidgetsEnabled(false);
    setWindowTitle(APP_NAME);

    if (project != nullptr) {
        delete project;
        project = nullptr;
    }
}

void XMainWindow::editJtfInfo()
{
    if (editingProject == false)
        return;

    XDialogJTFInfo dlg(this);

    if (dlg.exec()) {
    }
}

void XMainWindow::jtfTest()
{
}

void XMainWindow::aboutUs()
{
    QString msg;

    msg.sprintf("<b>%s</b> - An application to build libxante projects.\n"
                "Version %d.%d.%d", APP_NAME, MAJOR_VERSION, MINOR_VERSION,
                RELEASE);

    QMessageBox::about(this, tr("About xante-builder"), msg);
}

void XMainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>(sender());

    if (action)
        loadFile(action->data().toString());
}

void XMainWindow::createMenu(void)
{
    QMenu *mMain = menuBar()->addMenu(tr("&Project"));
    acNewProject = mMain->addAction(tr("&New project"), this,
                                       &XMainWindow::newProject);

    acNewProject->setStatusTip(tr("Creates a new libxante project."));

    mMain->addSeparator();
    acOpen = mMain->addAction(tr("&Open project"), this,
                                &XMainWindow::openProject);

    acOpen->setStatusTip(tr("Opens a previously created project file."));
    acSave = mMain->addAction(tr("&Save project"), this,
                                &XMainWindow::saveProject);

    acSave->setStatusTip(tr("Saves the project."));
    acSave->setEnabled(false);
    acClose = mMain->addAction(tr("&Close project"), this,
                                 &XMainWindow::closeProject);

    acClose->setStatusTip(tr("Closes the project."));
    mMain->addSeparator();

    for (int i = 0; i < MaxRecentFiles; i++) {
        QString file = config.getRecentFile(i);
        acRecentFiles[i] = mMain->addAction(file.isEmpty() ? tr("") : file,
                                               this,
                                               &XMainWindow::openRecentFile);

        QString text = tr("&%1 %2").arg(i + 1)
                                   .arg(QFileInfo(file).fileName());

        if (file.isEmpty())
            acRecentFiles[i]->setVisible(false);
        else {
            acRecentFiles[i]->setText(text);
            acRecentFiles[i]->setData(file);
        }
    }

    mMain->addSeparator();
    QAction *acExit = mMain->addAction(tr("&Quit"), this, &QWidget::close);
    acExit->setStatusTip(tr("Quits the application."));

    QMenu *mActions = menuBar()->addMenu(tr("&JTF"));
    acJtfMainInfo = mActions->addAction(tr("&Informations"), this,
                                            &XMainWindow::editJtfInfo);

    acJtfMainInfo->setStatusTip(tr("Edits the JTF main informations."));
    mActions->addSeparator();
    acTestJtf = mActions->addAction(tr("&Test"), this,
                                       &XMainWindow::jtfTest);

    acTestJtf->setStatusTip(tr("Puts the current JTF configuration into a "
                                 "test."));

    QMenu *mHelp = menuBar()->addMenu(tr("&Help"));
    QAction *acAbout = mHelp->addAction(tr("&About this application"), this,
                                          &XMainWindow::aboutUs);

    acAbout->setStatusTip(tr("Shows a little help about this application."));
    QAction *acAboutQt = mHelp->addAction(tr("About &Qt"), qApp,
                                             &QApplication::aboutQt);

    acAboutQt->setStatusTip(tr("Shows the current Qt version."));
    setWindowWidgetsEnabled(false);
}

void XMainWindow::setWindowWidgetsEnabled(bool enable)
{
    XTreeModel *model;

    editingProject = enable;

    /* Enable/Disable menu options */
    acNewProject->setEnabled(!enable);
    acOpen->setEnabled(!enable);
    acClose->setEnabled(enable);
    acJtfMainInfo->setEnabled(enable);
    acTestJtf->setEnabled(enable);

    for (int j = 0; j < MaxRecentFiles; j++)
        acRecentFiles[j]->setEnabled(!enable);

    /* Populate the tree view */
    model = new XTreeModel(enable, this);
    dialog->setTreeContent(model, enable);
    dialog->controlProjectWidgets(enable);
}

void XMainWindow::setCurrentFile(const QString &filename)
{
    if (config.setRecentFile(filename) == false)
        return;

    int nRecentFiles = qMin(config.recentFilesSize(), (int)MaxRecentFiles);

    for (int i = 0; i < nRecentFiles; i++) {
        QString file = config.getRecentFile(i);
        QString text = tr("&%1 %2").arg(i + 1)
                                   .arg(QFileInfo(file).fileName());

        acRecentFiles[i]->setText(text);
        acRecentFiles[i]->setData(file);
        acRecentFiles[i]->setVisible(true);
    }

    for (int j = nRecentFiles; j < MaxRecentFiles; j++)
        acRecentFiles[j]->setVisible(false);
}

void XMainWindow::projectChanged()
{
    acSave->setEnabled(true);
    setWindowTitle(QString("%1 [%2] *").arg(APP_NAME)
                                       .arg(project->getProjectName()));
}

