
/*
 * Description:
 *
 * Author: Rodrigo Freitas
 * Created at: Fri Jun  9 13:35:25 2017
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

#include <QWizard>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QComboBox>

#include "xante_builder.hpp"

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction"));
    label = new QLabel(tr("This assistant will help you create a new project "
                          "to be used with libxante."));

    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Conclusion"));
    label = new QLabel;
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}

void ConclusionPage::initializePage()
{
    QString text = wizard()->buttonText(QWizard::FinishButton);
    text.remove('&');
    label->setText(tr("Click %1 to create the project.").arg(text));
}

ProjectInfoPage::ProjectInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Project Informations"));

    QLabel *lname = new QLabel(tr("Project name:"));
    le_name = new QLineEdit;
    lname->setBuddy(lname);

    QHBoxLayout *hname = new QHBoxLayout;
    hname->addWidget(lname);
    hname->addWidget(le_name);

    QLabel *ldescription = new QLabel(tr("Project description:"));
    le_description = new QLineEdit;
    ldescription->setBuddy(le_description);

    QHBoxLayout *hdescription = new QHBoxLayout;
    hdescription->addWidget(ldescription);
    hdescription->addWidget(le_description);

    QLabel *lpathname = new QLabel(tr("Destination:"));
    le_pathname = new QLineEdit;
    le_pathname->setText(getenv("HOME"));
    lpathname->setBuddy(le_pathname);

    QPushButton *bt = new QPushButton(tr("..."));
    connect(bt, SIGNAL(clicked()), this, SLOT(choose_dir()));

    QHBoxLayout *hpathname = new QHBoxLayout;
    hpathname->addWidget(lpathname);
    hpathname->addWidget(le_pathname);
    hpathname->addWidget(bt);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hname);
    layout->addLayout(hdescription);
    layout->addLayout(hpathname);
    setLayout(layout);

    registerField("project_name", le_name);
    registerField("project_description", le_description);
    registerField("project_pathname", le_pathname);
}

void ProjectInfoPage::choose_dir(void)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open directory"),
                                                    getenv("HOME"),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty() == false)
        le_pathname->setText(dir);
}

ProjectConfigPage::ProjectConfigPage(QWidget *parent)
    : QWizardPage(parent)
{
    enum validator_type {
        VLD_STRING,
        VLD_INT,
        VLD_FLOAT
    };

    struct item {
        QString             label;
        QLineEdit           *le;
        enum validator_type validator;
        const char          *field_name;
    };

    struct item *p, page_items[] = {
        { tr("Company name:"),                  le_company,     VLD_STRING, "project_company"  },
        { tr("Plugin name:"),                   le_plugin,      VLD_STRING, "project_plugin"   },
        { tr("Configuration file directoty:"),  le_cfg_path,    VLD_STRING, "project_cfg"      },
        { tr("Log file directory:"),            le_log_path,    VLD_STRING, "project_log"      },
        { tr("Version:"),                       le_version,     VLD_FLOAT,  "project_version"  },
        { tr("Revision:"),                      le_revision,    VLD_INT,    "project_revision" },
        { tr("Build:"),                         le_build,       VLD_INT,    "project_build"    }
    };
    int i, t;

    t = sizeof(page_items) / sizeof(page_items[0]);
    setTitle(tr("Project Configuration"));
    QVBoxLayout *layout = new QVBoxLayout;

    for (i = 0; i < t; i++) {
        p = &page_items[i];
        QLabel *l = new QLabel(p->label);
        p->le = new QLineEdit;
        l->setBuddy(p->le);

        if (p->validator == VLD_INT)
            p->le->setValidator(new QIntValidator(p->le));
        else if (p->validator == VLD_FLOAT)
            p->le->setValidator(new QDoubleValidator(p->le));

        registerField(p->field_name, p->le);
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(l);
        h->addWidget(p->le);

        layout->addLayout(h);
    }

    QLabel *l = new QLabel(tr("Beta version:"));
    cb_beta = new QComboBox;
    l->setBuddy(cb_beta);
    cb_beta->addItem(tr("Yes"));
    cb_beta->addItem(tr("No"));

    QHBoxLayout *h = new QHBoxLayout;
    h->addWidget(l);
    h->addWidget(cb_beta);
    registerField("project_beta", cb_beta);

    layout->addLayout(h);
    setLayout(layout);
}

XProjectWizard::XProjectWizard(QWidget *parent)
    : QWizard(parent)
{
    setWizardStyle(QWizard::ModernStyle);
    setWindowTitle(tr("New project"));
    addPage(new IntroPage(this));
    addPage(new ProjectInfoPage(this));
    addPage(new ProjectConfigPage(this));
    addPage(new ConclusionPage(this));
}

XProjectWizard::~XProjectWizard()
{
    if (release_project && (project != nullptr))
        delete project;
}

void XProjectWizard::accept()
{
    /*
     * TODO: Call source-tpl to create the templates
     */

    QDialog::accept();
}

XanteProject *XProjectWizard::get_project(void)
{
    QString project_name = field("project_name").toString();
    project = new XanteProject(project_name,
                               field("project_pathname").toString(),
                               XanteJTF::Builder()
                                .set_application_name(project_name)
                                .set_description(field("project_description").toString())
                                .set_company(field("project_company").toString())
                                .set_plugin(field("project_plugin").toString())
                                .set_cfg_pathname(field("project_cfg").toString())
                                .set_log_pathname(field("project_log").toString())
                                .set_version(field("project_version").toString())
                                .set_revision(field("project_revision").toInt())
                                .set_build(field("project_build").toInt())
                                .set_beta(field("project_beta").toBool())
                                .build());

    project->create();

    /*
     * Don't let release the project object when this object goes out of
     * scope.
     */
    release_project = false;

    return project;
}

