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

#include "anyvalue_editor_dialog.h"

#include "abstract_anyvalue_editor.h"

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

namespace
{

QString GetDialogSizeSettingName(const QString& editor_name)
{
  return QString("AnyValueEditorDialog") + "/" + editor_name + "window_size";
}

//! Creates layout with OK/Cancel buttons.
std::unique_ptr<QBoxLayout> CreateButtonLayout(QDialog* dialog)
{
  std::unique_ptr<QBoxLayout> result = std::make_unique<QVBoxLayout>();

  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton("Set AnyValue", QDialogButtonBox::AcceptRole);
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

namespace sequencergui
{

AnyValueEditorDialog::AnyValueEditorDialog(std::unique_ptr<AbstractAnyValueEditor> editor,
                                           QWidget* parent)
    : QDialog(parent), m_anyvalue_editor(editor.release())
{
  setWindowTitle("Input request");
  ReadSettings();

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_anyvalue_editor);
  layout->addLayout(CreateButtonLayout(this).release());
}

AnyValueEditorDialog::~AnyValueEditorDialog()
{
  WriteSettings();
}

void AnyValueEditorDialog::SetInitialValue(const sup::gui::AnyValueItem* item)
{
  m_anyvalue_editor->SetInitialValue(item);
}

//! Returns result of the editing.
//! It is supposed to be used after the dialog close.

std::unique_ptr<sup::gui::AnyValueItem> AnyValueEditorDialog::GetResult()
{
  return m_anyvalue_editor->GetResult();
}

void AnyValueEditorDialog::SetDescription(const QString& description)
{
  m_anyvalue_editor->SetDescription(description);
}

//! Prevent dialog from closing on enter-key-event. This is necessary since some of underlying
//! widgets might have own ideas and might forward this key event back.

void AnyValueEditorDialog::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
  {
    return;
  }

  QDialog::keyPressEvent(event);
}

//! Loads persistence widget settings from disk.
void AnyValueEditorDialog::ReadSettings()
{
  const QSettings settings;
  const auto setting_name = GetDialogSizeSettingName(m_anyvalue_editor->windowTitle());
  if (settings.contains(setting_name))
  {
    resize(settings.value(setting_name).toSize());
  }
}

//! Writes persistence widget settings on disk.
void AnyValueEditorDialog::WriteSettings()
{
  QSettings settings;
  settings.setValue(GetDialogSizeSettingName(m_anyvalue_editor->windowTitle()), size());
}

}  // namespace sequencergui
