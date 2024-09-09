/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "settings_editor_dialog.h"

#include "settings_editor.h"

#include <mvvm/widgets/widget_utils.h>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

namespace sequencergui
{

namespace
{

/**
 * @brief Creates layout with OK/Cancel buttons.
 */
std::unique_ptr<QBoxLayout> CreateButtonLayout(QDialog* dialog)
{
  std::unique_ptr<QBoxLayout> result = std::make_unique<QVBoxLayout>();

  auto button_box = new QDialogButtonBox;
  auto button =
      button_box->addButton("Save settings (restart required)", QDialogButtonBox::AcceptRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  button = button_box->addButton("Cancel", QDialogButtonBox::RejectRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  QDialogButtonBox::connect(button_box, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
  QDialogButtonBox::connect(button_box, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

  auto gap = mvvm::utils::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  result->addWidget(button_box);
  return result;
}

const QString kGroupName = "SettingsEditorDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

}  // namespace

SettingsEditorDialog::SettingsEditorDialog(QWidget* parent)
    : QDialog(parent), m_settings_editor(new SettingsEditor)
{
  setWindowTitle("Application Settings");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_settings_editor);
  layout->addLayout(CreateButtonLayout(this).release());
}

void SettingsEditorDialog::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(20), mvvm::utils::UnitSize(60));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());
}

void SettingsEditorDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
}

}  // namespace sequencergui
