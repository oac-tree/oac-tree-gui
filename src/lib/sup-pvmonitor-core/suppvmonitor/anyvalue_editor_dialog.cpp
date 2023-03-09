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

#include "anyvalue_editor_dialog.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor.h>

#include <mvvm/widgets/widget_utils.h>

#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

namespace suppvmonitor
{

AnyValueEditorDialog::AnyValueEditorDialog(QWidget* parent)
    : QDialog(parent), m_anyvalue_editor(new sup::gui::AnyValueEditor)
{
  setWindowTitle("AnyValueEditor");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_anyvalue_editor);
  layout->addLayout(CreateButtonLayout());
}

void AnyValueEditorDialog::SetInitialValue(const sup::gui::AnyValueItem &item)
{
  m_anyvalue_editor->SetInitialValue(item);
}

//! Creates layout with OK/CANCEL buttons.

QBoxLayout* AnyValueEditorDialog::CreateButtonLayout()
{
  auto button_box = new QDialogButtonBox;
  auto button = button_box->addButton("Set AnyValue", QDialogButtonBox::AcceptRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  button = button_box->addButton("Cancel", QDialogButtonBox::RejectRole);
  button->setAutoDefault(false);
  button->setDefault(false);
  connect(button_box, &QDialogButtonBox::accepted, this, &AnyValueEditorDialog::accept);
  connect(button_box, &QDialogButtonBox::rejected, this, &AnyValueEditorDialog::reject);

  auto result = new QVBoxLayout;
  auto gap = mvvm::utils::UnitSize(0.5);
  result->setContentsMargins(gap, gap, gap, gap);
  result->addWidget(button_box);
  return result;
}

}  // namespace suppvmonitor
