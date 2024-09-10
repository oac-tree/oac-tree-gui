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
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

namespace sequencergui
{

namespace
{

const QString kGroupName = "SettingsEditorDialog";
const QString kWindowSizeSettingName = kGroupName + "/" + "size";

/**
 * @brief Creates layout with OK/Cancel buttons.
 */
std::unique_ptr<QBoxLayout> CreateButtonLayout(QDialog* dialog)
{
  auto result = std::make_unique<QVBoxLayout>();

  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton("Save settings", QDialogButtonBox::AcceptRole);
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

}  // namespace

SettingsEditorDialog::SettingsEditorDialog(QWidget* parent)
    : QDialog(parent), m_label(new QLabel), m_settings_editor(new SettingsEditor)
{
  setWindowTitle("Application Settings");

  m_label->setText("Changes in settings will come into effect at the next application start");
  m_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_label->setFixedHeight(mvvm::utils::UnitSize(2));

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_settings_editor);
  layout->addWidget(m_label);
  // layout->addSpacing(mvvm::utils::UnitSize(0.5));
  layout->addLayout(CreateButtonLayout(this).release());

  ReadSettings();
}

SettingsEditorDialog::~SettingsEditorDialog()
{
  WriteSettings();
}

void SettingsEditorDialog::SetInitialValues(const SettingsModel& model)
{
  m_settings_editor->SetInitialValues(model);
}

void SettingsEditorDialog::keyPressEvent(QKeyEvent* event)
{
  // Prevent dialog from closing on enter-key-event. This is necessary since some of underlying
  // widgets might have own ideas and might forward this key event back.

  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    return;
  }

  QDialog::keyPressEvent(event);
}

void SettingsEditorDialog::ReadSettings()
{
  const QSettings settings;
  const auto default_size = QSize(mvvm::utils::UnitSize(30), mvvm::utils::UnitSize(30));
  resize(settings.value(kWindowSizeSettingName, default_size).toSize());
}

void SettingsEditorDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(kWindowSizeSettingName, size());
}

}  // namespace sequencergui
