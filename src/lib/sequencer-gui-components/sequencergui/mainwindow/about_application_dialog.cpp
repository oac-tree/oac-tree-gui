/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "about_application_dialog.h"

#include <sequencergui/core/version.h>

#include <mvvm/editors/custom_event_filters.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDate>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{

QLabel* createLinkLabel()
{
  auto result = new QLabel();
  result->setTextFormat(Qt::RichText);
  result->setTextInteractionFlags(Qt::TextBrowserInteraction);
  result->setText(
      "<a href=\"https://git.iter.org/projects/COA/repos\">CODAC Operational Applications</a>");
  result->setOpenExternalLinks(true);
  return result;
}

QLabel* createCopyrightLabel()
{
  QDate date = QDate::currentDate();
  QString copyright = QString("Copyright: 2010-%1 ITER Organization ").arg(date.toString("yyyy"));

  auto result = new QLabel(copyright);
  result->setContentsMargins(0, 0, 0, mvvm::utils::UnitSize());
  return result;
}

QLabel* createLogoLabel()
{
  QPixmap logo(":/icons/about_logo.awk", "JPG");
  auto result = new QLabel;
  result->setPixmap(
      logo.scaled(mvvm::utils::UnitSize(50), mvvm::utils::UnitSize(50), Qt::KeepAspectRatio));
  return result;
}
}  // namespace

namespace sequencergui
{

AboutApplicationDialog::AboutApplicationDialog(QWidget* parent) : QDialog(parent)
{
  QColor bgColor(240, 240, 240, 255);
  QPalette palette;
  palette.setColor(QPalette::Window, bgColor);
  setAutoFillBackground(true);
  setPalette(palette);

  setWindowTitle("About Sequencer");
  setWindowFlags(Qt::Dialog);

  auto details_layout = new QHBoxLayout;
  details_layout->addLayout(createLogoLayout());
  details_layout->addLayout(createTextLayout());

  auto main_layout = new QVBoxLayout;
  main_layout->addLayout(details_layout);
  main_layout->addLayout(createButtonLayout());

  //  setMinimumHeight(styleutils::UnitSize(20));
  //  setMinimumWidth(styleutils::UnitSize(30));

  setLayout(main_layout);
}

QBoxLayout* AboutApplicationDialog::createLogoLayout()
{
  auto result = new QVBoxLayout;

  QPixmap logo(":/icons/iter_logo.png");
  auto label = new QLabel;
  label->setPixmap(
      logo.scaled(mvvm::utils::UnitSize(10), mvvm::utils::UnitSize(10), Qt::KeepAspectRatio));

  result->addWidget(label);
  result->addStretch(1);
  auto gap = mvvm::utils::UnitSize(1.0);
  result->setContentsMargins(0, gap, gap, gap);

  return result;
}

QBoxLayout* AboutApplicationDialog::createTextLayout()
{
  auto result = new QVBoxLayout;

  // title
  auto about_title_label = new QLabel(
      QString("Sequencer GUI version ").append(QString::fromStdString(ProjectVersion())));
  mvvm::utils::ScaleLabelFont(about_title_label, 1.2, true);
  about_title_label->setContentsMargins(0, 0, 0, mvvm::utils::UnitSize());

  // copyright
  auto copyright_label = createCopyrightLabel();

  // description
  QString description =
      "Integrated development environment for Sequencer of the ITER's Supervision and Automation "
      "system";

  auto description_label = new QLabel(description);
  description_label->setWordWrap(true);

  result->addWidget(about_title_label);
  result->addWidget(description_label);
  result->addStretch(1);
  result->addWidget(copyright_label);
  result->addWidget(createLinkLabel());
  result->addStretch(1);

  auto gap = mvvm::utils::UnitSize(1.0);
  result->setContentsMargins(0, gap, gap, gap);

  return result;
}

QBoxLayout* AboutApplicationDialog::createButtonLayout()
{
  auto result = new QHBoxLayout;

  auto closeButton = new QPushButton("Close");
  connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

  result->addStretch(1);
  result->addWidget(closeButton);

  static const char mydata[] = {0x64, 0x65, 0x76, 0x73};
  QByteArray b = QByteArray::fromRawData(mydata, sizeof(mydata));
  auto f = new mvvm::ShortcodeFilter(b, this);
  connect(f, &mvvm::ShortcodeFilter::found, this,
          [=]() { layout()->addWidget(createLogoLabel()); });
  installEventFilter(f);

  return result;
}
}  // namespace sequencergui
