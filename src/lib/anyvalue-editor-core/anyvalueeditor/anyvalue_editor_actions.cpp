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

#include <sup/gui/core/anyvalue_conversion_utils.h>
#include <sup/gui/core/anyvalue_item.h>
#include <sup/gui/core/anyvalue_item_utils.h>

#include <QMainWindow>
#include <QMessageBox>

namespace anyvalueeditor
{

AnyValueEditorActions::AnyValueEditorActions(AnyValueEditorContext context,
                                             mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model), m_context(context)
{
}



void AnyValueEditorActions::OnAddAnyValueStruct(bool selected_as_parent)
{
  // only one top level item is allowed
  if (!selected_as_parent && m_model->GetRootItem()->GetTotalItemCount() > 0)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Only one top AnyValue is allowed");
    m_context.send_message_callback(message);
    return;
  }

  auto parent = selected_as_parent ? m_context.get_selected_callback() : m_model->GetRootItem();
  if (auto result = AddAnyValueItem<sup::gui::AnyValueStructItem>(parent); result)
  {
    result->SetDisplayName(::sup::gui::kStructTypeName);
  }
}

void AnyValueEditorActions::OnAddAnyValueArray(bool selected_as_parent)
{
  // only one top level item is allowed
  if (!selected_as_parent && m_model->GetRootItem()->GetTotalItemCount() > 0)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Only one top AnyValue is allowed");
    m_context.send_message_callback(message);
    return;
  }

  try
  {
    auto parent = selected_as_parent ? m_context.get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      m_model->InsertItem<sup::gui::AnyValueArrayItem>(parent, mvvm::TagIndex::Append())
          ->SetDisplayName(::sup::gui::kArrayTypeName);
    }
  }
  catch (const std::exception& ex)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Can't insert structure");
    m_context.send_message_callback(message);
  }
}

void AnyValueEditorActions::OnAddAnyValueScalar(const std::string& scalar_type,
                                                bool selected_as_parent)
{
  // only one top level item is allowed
  if (!selected_as_parent && m_model->GetRootItem()->GetTotalItemCount() > 0)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Only one top AnyValue is allowed");
    m_context.send_message_callback(message);
    return;
  }

  if (selected_as_parent)
  {
    if (auto array_item = mvvm::utils::GetTopItem<sup::gui::AnyValueArrayItem>(m_model);
        array_item)
    {
      if (!sup::gui::IsSuitableScalarType(*array_item, scalar_type))
      {
        auto message = sup::gui::CreateInvalidOperationMessage("Array element mismatch");
        m_context.send_message_callback(message);
        return;
      }
    }
  }

  try
  {
    auto parent = selected_as_parent ? m_context.get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      auto scalar =
          m_model->InsertItem<sup::gui::AnyValueScalarItem>(parent, mvvm::TagIndex::Append());
      scalar->SetAnyTypeName(scalar_type);
      scalar->SetDisplayName(scalar_type);
    }
  }
  catch (const std::exception& ex)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Can't insert structure");
    m_context.send_message_callback(message);
  }
}

void AnyValueEditorActions::OnRemoveSelected()
{
  m_model->RemoveItem(m_context.get_selected_callback());
}

void AnyValueEditorActions::AddAnyValueStruct(mvvm::SessionItem* parent)
{
  if (!parent)
  {
    return;
  }

  try
  {
    m_model->InsertItem<sup::gui::AnyValueStructItem>(parent, mvvm::TagIndex::Append())
        ->SetDisplayName(::sup::gui::kStructTypeName);
  }
  catch (const std::exception& ex)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Can't insert structure");
    m_context.send_message_callback(message);
  }
}

}  // namespace anyvalueeditor
