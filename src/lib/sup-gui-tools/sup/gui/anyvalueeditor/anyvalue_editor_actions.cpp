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

#include "anyvalue_editor_actions.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>

namespace sup::gui
{

AnyValueEditorActions::AnyValueEditorActions(AnyValueEditorContext context,
                                             mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model), m_context(context)
{
}

void AnyValueEditorActions::OnAddAnyValueStruct(bool selected_as_parent)
{
  AddAnyValueItem<sup::gui::AnyValueStructItem>(selected_as_parent, ::sup::gui::kStructTypeName);
}

void AnyValueEditorActions::OnAddAnyValueArray(bool selected_as_parent)
{
  AddAnyValueItem<sup::gui::AnyValueArrayItem>(selected_as_parent, ::sup::gui::kArrayTypeName);
}

void AnyValueEditorActions::OnAddAnyValueScalar(const std::string& scalar_type,
                                                bool selected_as_parent)
{
  if (selected_as_parent)
  {
    if (auto array_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(m_model); array_item)
    {
      if (!sup::gui::IsSuitableScalarType(*array_item, scalar_type))
      {
        SendMessage("Array element mismatch");
        return;
      }
    }
  }

  if (auto result = AddAnyValueItem<sup::gui::AnyValueScalarItem>(selected_as_parent, scalar_type);
      result)
  {
    result->SetAnyTypeName(scalar_type);
  }
}

void AnyValueEditorActions::OnRemoveSelected()
{
  if (auto selected = m_context.get_selected_callback(); selected)
  {
    m_model->RemoveItem(selected);
  }
}

//! Returns parent item to use for insertion.
mvvm::SessionItem* AnyValueEditorActions::GetParent(bool selected_as_parent) const
{
  return selected_as_parent ? m_context.get_selected_callback() : m_model->GetRootItem();
}

void AnyValueEditorActions::SendMessage(const std::string& text, const std::string& informative,
                                        const std::string& details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

}  // namespace anyvalueeditor
