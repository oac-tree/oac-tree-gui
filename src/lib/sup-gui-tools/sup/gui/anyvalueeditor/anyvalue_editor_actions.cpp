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

#include <sup/dto/anyvalue.h>
#include <sup/dto/anyvalue_helper.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_item_utils.h>
#include <sup/gui/model/anyvalue_utils.h>

namespace sup::gui
{

AnyValueEditorActions::AnyValueEditorActions(AnyValueEditorContext context,
                                             mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model), m_context(std::move(context))
{
}

void AnyValueEditorActions::OnAddAnyValueStruct()
{
  AddAnyValueItem<sup::gui::AnyValueStructItem>(::sup::gui::kStructTypeName);
}

void AnyValueEditorActions::OnAddAnyValueArray()
{
  AddAnyValueItem<sup::gui::AnyValueArrayItem>(::sup::gui::kArrayTypeName);
}

void AnyValueEditorActions::OnAddAnyValueScalar(const std::string& scalar_type)
{
  if (auto array_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(m_model); array_item)
  {
    if (!sup::gui::IsSuitableScalarType(*array_item, scalar_type))
    {
      SendMessage("Array element mismatch");
      return;
    }
  }

  if (auto result = AddAnyValueItem<sup::gui::AnyValueScalarItem>(scalar_type); result)
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

void AnyValueEditorActions::OnImportFromFileRequest(const std::string& file_name)
{
  if (GetTopItem())
  {
    SendMessage("Only one item is allowed");
    return;
  }

  auto anyvalue = sup::gui::AnyValueFromJSONFile(file_name);
  m_model->InsertItem(sup::gui::CreateItem(anyvalue), m_model->GetRootItem(),
                      mvvm::TagIndex::Append());
}

void AnyValueEditorActions::OnExportToFileRequest(const std::string& file_name)
{
  if (!GetTopItem())
  {
    SendMessage("Nothing to save");
    return;
  }

  try
  {
    auto anyvalue = CreateAnyValue(*GetTopItem());
    sup::dto::AnyValueToJSONFile(anyvalue, file_name, /*pretty*/ true);
  }
  catch (const std::exception& ex)
  {
    SendMessage("Can't generate valid JSON presentation from current item", "Exception was thrown",
                ex.what());
  }
}

//! Set initial value. The given value will be cloned inside the editor's model and used as
//! a starting point for editing.

void AnyValueEditorActions::SetInitialValue(const AnyValueItem& item)
{
  if (auto item = GetTopItem(); item)
  {
    SendMessage("Only one top item is allowed");
  }

  // FIXME provide clone, not copy, to preserve identifier of the item being edited
  mvvm::utils::CopyItem(&item, m_model, m_model->GetRootItem(), mvvm::TagIndex::Append());
}

AnyValueItem* AnyValueEditorActions::GetTopItem()
{
  return mvvm::utils::GetTopItem<AnyValueItem>(m_model);
}

AnyValueItem* AnyValueEditorActions::GetSelectedItem() const
{
  return m_context.get_selected_callback ? m_context.get_selected_callback() : nullptr;
}

//! Returns parent item to use for insertion.
mvvm::SessionItem* AnyValueEditorActions::GetParent() const
{
  return GetSelectedItem() ? GetSelectedItem() : m_model->GetRootItem();
}

void AnyValueEditorActions::SendMessage(const std::string& text, const std::string& informative,
                                        const std::string& details)
{
  auto message = sup::gui::CreateInvalidOperationMessage(text, informative, details);
  m_context.send_message_callback(message);
}

}  // namespace sup::gui
