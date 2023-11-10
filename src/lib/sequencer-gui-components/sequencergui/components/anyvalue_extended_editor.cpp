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

#include "anyvalue_extended_editor.h"

#include <sup/gui/anyvalueeditor/anyvalue_editor.h>
#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/item_utils.h>

#include <QLabel>
#include <QVBoxLayout>

namespace sequencergui
{

AnyValueExtendedEditor::AnyValueExtendedEditor(QWidget *parent)
    : AbstractAnyValueEditor(parent), m_editor(new sup::gui::AnyValueEditor)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);
  layout->addWidget(m_editor);
}

AnyValueExtendedEditor::~AnyValueExtendedEditor() = default;

void AnyValueExtendedEditor::SetInitialValue(const sup::gui::AnyValueItem *item)
{
  if (item)
  {
    m_editor->SetInitialValue(*item);
  }
}

std::unique_ptr<sup::gui::AnyValueItem> AnyValueExtendedEditor::GetResult()
{
  if (auto top_item = m_editor->GetTopItem(); top_item)
  {
    return mvvm::utils::CloneItem(*top_item);
  }

  return {};
}

}  // namespace sequencergui
