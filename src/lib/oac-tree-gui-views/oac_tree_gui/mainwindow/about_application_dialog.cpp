/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "about_application_dialog.h"

#include <oac_tree_gui/core/version.h>

#include <mvvm/editors/custom_event_filters.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDate>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{

std::unique_ptr<QLabel> CreateLinkLabel()
{
  auto result = std::make_unique<QLabel>();

  result->setTextFormat(Qt::RichText);
  result->setTextInteractionFlags(Qt::TextBrowserInteraction);
  result->setText(
      "<a href=\"https://git.iter.org/projects/COA/repos\">CODAC Operational Applications</a>");
  result->setOpenExternalLinks(true);
  return result;
}

std::unique_ptr<QLabel> CreateCopyrightLabel()
{
  const QDate date = QDate::currentDate();
  const QString copyright =
      QString("Copyright: 2010-%1 ITER Organization ").arg(date.toString("yyyy"));

  auto result = std::make_unique<QLabel>();
  result->setContentsMargins(0, 0, 0, mvvm::utils::UnitSize());
  return result;
}

std::unique_ptr<QLabel> CreateLogoLabel()
{
  const QPixmap logo(":/oac-tree/icons/about_logo.awk", "JPG");

  auto result = std::make_unique<QLabel>();

  result->setPixmap(
      logo.scaled(mvvm::utils::UnitSize(50), mvvm::utils::UnitSize(50), Qt::KeepAspectRatio));
  return result;
}

}  // namespace

namespace oac_tree_gui
{

AboutApplicationDialog::AboutApplicationDialog(QWidget* parent_widget) : QDialog(parent_widget)
{
  setWindowTitle("About oac-tree");
  setWindowFlags(Qt::Dialog);

  auto details_layout = new QHBoxLayout;
  details_layout->addLayout(CreateLogoLayout().release(), 2);
  details_layout->addLayout(CreateTextLayout().release(), 5);

  auto main_layout = new QVBoxLayout;
  main_layout->addLayout(details_layout);
  main_layout->addLayout(CreateButtonLayout().release());

  setLayout(main_layout);
}

std::unique_ptr<QBoxLayout> AboutApplicationDialog::CreateLogoLayout()
{
  auto result = std::make_unique<QVBoxLayout>();

  QPixmap logo(":/sup-gui-core/icons/iter_logo.png");
  auto label = new QLabel;
  label->setPixmap(
      logo.scaled(mvvm::utils::UnitSize(10), mvvm::utils::UnitSize(10), Qt::KeepAspectRatio));

  result->addWidget(label);
  result->addStretch(1);
  auto gap = mvvm::utils::UnitSize(1.0);
  result->setContentsMargins(0, gap, gap, gap);

  return result;
}

std::unique_ptr<QBoxLayout> AboutApplicationDialog::CreateTextLayout()
{
  auto result = std::make_unique<QVBoxLayout>();

  // title
  auto about_title_label =
      new QLabel(QString("oac-tree GUI version ").append(QString::fromStdString(ProjectVersion())));
  mvvm::utils::ScaleLabelFont(about_title_label, 1.2, true);
  about_title_label->setContentsMargins(0, 0, 0, mvvm::utils::UnitSize());

  // description
  QString description =
      "Integrated development environment for oac-tree of the ITER's Supervision and Automation "
      "system";

  auto description_label = new QLabel(description);
  description_label->setWordWrap(true);

  result->addWidget(about_title_label);
  result->addWidget(description_label);
  result->addStretch(1);
  result->addWidget(CreateCopyrightLabel().release());
  result->addWidget(CreateLinkLabel().release());
  result->addStretch(1);

  auto gap = mvvm::utils::UnitSize(1.0);
  result->setContentsMargins(0, gap, gap, gap);

  return result;
}

std::unique_ptr<QBoxLayout> AboutApplicationDialog::CreateButtonLayout()
{
  auto result = std::make_unique<QHBoxLayout>();

  auto closeButton = new QPushButton("Close");
  connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

  result->addStretch(1);
  result->addWidget(closeButton);

  static const char mydata[] = {0x64, 0x65, 0x76, 0x73};
  QByteArray b = QByteArray::fromRawData(mydata, sizeof(mydata));
  auto f = new mvvm::ShortcodeFilter(b, this);
  connect(f, &mvvm::ShortcodeFilter::found, this,
          [=]() { layout()->addWidget(CreateLogoLabel().release()); });
  installEventFilter(f);

  return result;
}

}  // namespace oac_tree_gui
