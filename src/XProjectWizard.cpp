
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
    setTitle(tr("Project Information"));

    QLabel *lname = new QLabel(tr("Project name:"));
    leName = new QLineEdit;
    lname->setBuddy(lname);

    QHBoxLayout *hname = new QHBoxLayout;
    hname->addWidget(lname);
    hname->addWidget(leName);

    QLabel *ldescription = new QLabel(tr("Project description:"));
    leDescription = new QLineEdit;
    ldescription->setBuddy(leDescription);

    QHBoxLayout *hdescription = new QHBoxLayout;
    hdescription->addWidget(ldescription);
    hdescription->addWidget(leDescription);

    QLabel *lpathname = new QLabel(tr("Destination:"));
    lePathname = new QLineEdit;
    lePathname->setText(getenv("HOME"));
    lpathname->setBuddy(lePathname);

    QPushButton *bt = new QPushButton(tr("..."));
    connect(bt, SIGNAL(clicked()), this, SLOT(chooseDir()));

    QHBoxLayout *hpathname = new QHBoxLayout;
    hpathname->addWidget(lpathname);
    hpathname->addWidget(lePathname);
    hpathname->addWidget(bt);

    QLabel *lAuthor = new QLabel(tr("Author:"));
    leAuthor = new QLineEdit;
    lAuthor->setBuddy(leAuthor);

    QHBoxLayout *hAuthor = new QHBoxLayout;
    hAuthor->addWidget(lAuthor);
    hAuthor->addWidget(leAuthor);

    QLabel *l = new QLabel(tr("Language:"));
    cbLanguage= new QComboBox;
    l->setBuddy(cbLanguage);
    cbLanguage->addItem(tr("C"));
    cbLanguage->addItem(tr("Go"));
    cbLanguage->addItem(tr("Rust"));

    QHBoxLayout *hLanguage = new QHBoxLayout;
    hLanguage->addWidget(l);
    hLanguage->addWidget(cbLanguage);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(hname);
    layout->addLayout(hdescription);
    layout->addLayout(hpathname);
    layout->addLayout(hAuthor);
    layout->addLayout(hLanguage);
    setLayout(layout);

    registerField("projectName", leName);
    registerField("projectDescription", leDescription);
    registerField("projectPathname", lePathname);
    registerField("projectAuthor", leAuthor);
    registerField("projectLanguage", cbLanguage, "currentText", "currentTextChanged");
}

void ProjectInfoPage::chooseDir(void)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open directory"),
                                                    getenv("HOME"),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty() == false)
        lePathname->setText(dir);
}

ProjectConfigPage::ProjectConfigPage(QWidget *parent)
    : QWizardPage(parent)
{
    enum validatorType {
        VLD_STRING,
        VLD_INT,
        VLD_FLOAT
    };

    struct item {
        QString             label;
        QLineEdit           *le;
        enum validatorType validator;
        const char          *fieldName;
    };

    struct item *p, pageItems[] = {
        { tr("Company name:"),                  leCompany,     VLD_STRING, "projectCompany"  },
        { tr("Plugin name:"),                   lePlugin,      VLD_STRING, "projectPlugin"   },
        { tr("Configuration file directoty:"),  leCfgPath,    VLD_STRING, "projectCfg"      },
        { tr("Log file directory:"),            leLogPath,    VLD_STRING, "projectLog"      },
        { tr("Version:"),                       leVersion,     VLD_FLOAT,  "projectVersion"  },
        { tr("Revision:"),                      leRevision,    VLD_INT,    "projectRevision" },
        { tr("Build:"),                         leBuild,       VLD_INT,    "projectBuild"    }
    };
    int i, t;

    t = sizeof(pageItems) / sizeof(pageItems[0]);
    setTitle(tr("Project Configuration"));
    QVBoxLayout *layout = new QVBoxLayout;

    for (i = 0; i < t; i++) {
        p = &pageItems[i];
        QLabel *l = new QLabel(p->label);
        p->le = new QLineEdit;
        l->setBuddy(p->le);

        if (p->validator == VLD_INT)
            p->le->setValidator(new QIntValidator(p->le));
        else if (p->validator == VLD_FLOAT)
            p->le->setValidator(new QDoubleValidator(p->le));

        registerField(p->fieldName, p->le);
        QHBoxLayout *h = new QHBoxLayout;
        h->addWidget(l);
        h->addWidget(p->le);

        layout->addLayout(h);
    }

    QLabel *l = new QLabel(tr("Beta version:"));
    cbBeta = new QComboBox;
    l->setBuddy(cbBeta);
    cbBeta->addItem(tr("Yes"));
    cbBeta->addItem(tr("No"));

    QHBoxLayout *h = new QHBoxLayout;
    h->addWidget(l);
    h->addWidget(cbBeta);
    registerField("projectBeta", cbBeta);

    layout->addLayout(h);
    setLayout(layout);
}

XProjectWizard::XProjectWizard(XanteBuilderConfig &config, QWidget *parent)
    : QWizard(parent), config(config)
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
    if (releaseProject && (project != nullptr))
        delete project;
}

void XProjectWizard::accept()
{
    QDialog::accept();
}

XanteProject *XProjectWizard::buildProject(void)
{
    QString projectName = field("projectName").toString();
    project = new XanteProject(projectName,
                               field("projectPathname").toString(),
                               XanteJTF::Builder()
                                    .setApplicationName(projectName)
                                    .setDescription(field("projectDescription").toString())
                                    .setCompany(field("projectCompany").toString())
                                    .setPlugin(field("projectPlugin").toString())
                                    .setCfgPathname(field("projectCfg").toString())
                                    .setLogPathname(field("projectLog").toString())
                                    .setVersion(field("projectVersion").toString())
                                    .setRevision(field("projectRevision").toInt())
                                    .setBuild(field("projectBuild").toInt())
                                    .setBeta(field("projectBeta").toBool())
                                    .build());

    callSourceTemplate();
    project->create();

    /*
     * Don't let release the project object when this object goes out of scope.
     */
    releaseProject = false;

    return project;
}

/*
 * Calls the source-template application.
 */
void XProjectWizard::callSourceTemplate()
{
    QProcess p;
    QString language = field("projectLanguage").toString().toLower(),
            projectName = field("projectName").toString(),
            author = field("projectAuthor").toString(),
            path = field("projectPathname").toString();

    p.start(QString("%5/source-template -name %1 -language %2 -type xante-plugin "
                    "-path %3 -author \"%4\"").arg(projectName)
                                              .arg(language)
                                              .arg(path)
                                              .arg(author)
                                              .arg(config.sourceTemplatePath()));

    p.waitForFinished();
}

