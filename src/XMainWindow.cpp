
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

XMainWindow::XMainWindow(XanteConfig &config)
    : config(config)
{
    create_menu();
    statusBar()->showMessage(APP_NAME);

    dialog = new XMainDialog(this);
    setCentralWidget(dialog);

    setWindowTitle(APP_NAME);
    resize(config.get_window_size());
    move(config.get_window_position());
}

void XMainWindow::closeEvent(QCloseEvent *event)
{
    config.set_window_size(size());
    config.set_window_position(pos());
    config.write_configuration_file();
    event->accept();
}

void XMainWindow::new_project()
{
    XProjectWizard project_wizard;

    if (project_wizard.exec()) {
        editing_project = true;
        control_menu_options(true);
        project = project_wizard.get_project();
        printf("%s: create new JTF\n", __FUNCTION__);
    }
}

void XMainWindow::open_project()
{
    QFileDialog::Options options;
    QString selected_filter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Select file"),
                                                    tr(""),
                                                    tr("Project files (*.pjx)"),
                                                    &selected_filter, options);

    if (filename.isEmpty() == false)
        project = new XanteProject(filename);
}

void XMainWindow::save_project()
{
    project->save();
}

void XMainWindow::close_project()
{
    if (editing_project == false)
        return;

    control_menu_options(false);

    if (project != nullptr)
        delete project;
}

void XMainWindow::edit_jtf_info()
{
}

void XMainWindow::jtf_test()
{
}

void XMainWindow::about_us()
{
    QString msg;

    msg.sprintf("<b>%s</b> - An application to build libxante projects.\n"
                   "Version %d.%d.%d", APP_NAME, MAJOR_VERSION, MINOR_VERSION,
                RELEASE);

    QMessageBox::about(this, tr("About xante-builder"), msg);
}

void XMainWindow::create_menu(void)
{
    QMenu *m_main = menuBar()->addMenu(tr("&Project"));
    ac_new_project = m_main->addAction(tr("&New project"), this,
                                                &XMainWindow::new_project);

    ac_new_project->setStatusTip(tr("Creates a new libxante project."));

    m_main->addSeparator();
    ac_open = m_main->addAction(tr("&Open project"), this,
                                         &XMainWindow::open_project);

    ac_open->setStatusTip(tr("Opens a previously created project file."));
    ac_save = m_main->addAction(tr("&Save project"), this,
                                         &XMainWindow::save_project);

    ac_save->setStatusTip(tr("Saves the project."));
    ac_close = m_main->addAction(tr("&Close project"), this,
                                          &XMainWindow::close_project);

    ac_close->setStatusTip(tr("Closes the project."));
    m_main->addSeparator();
    QAction *ac_exit = m_main->addAction(tr("&Quit"), this, &QWidget::close);
    ac_exit->setStatusTip(tr("Quits the application."));

    QMenu *m_actions = menuBar()->addMenu(tr("&JTF"));
    ac_jtf_main_info = m_actions->addAction(tr("&Informations"), this,
                                                     &XMainWindow::edit_jtf_info);

    ac_jtf_main_info->setStatusTip(tr("Edits the JTF main informations."));
    m_actions->addSeparator();
    ac_test_jtf = m_actions->addAction(tr("&Test"), this,
                                                &XMainWindow::jtf_test);

    ac_test_jtf->setStatusTip(tr("Puts the current JTF configuration into a "
                                 "test."));

    QMenu *m_help = menuBar()->addMenu(tr("&Help"));
    QAction *ac_about = m_help->addAction(tr("&About this application"), this,
                                          &XMainWindow::about_us);

    ac_about->setStatusTip(tr("Shows a little help about this application."));
    QAction *ac_about_qt = m_help->addAction(tr("About &Qt"), qApp,
                                             &QApplication::aboutQt);

    ac_about_qt->setStatusTip(tr("Shows the current Qt version."));
    control_menu_options(false);
}

void XMainWindow::control_menu_options(bool enable)
{
    ac_new_project->setEnabled(!enable);
    ac_open->setEnabled(!enable);
    ac_save->setEnabled(enable);
    ac_close->setEnabled(enable);
    ac_jtf_main_info->setEnabled(enable);
    ac_test_jtf->setEnabled(enable);
}
