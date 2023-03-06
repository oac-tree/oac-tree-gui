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
#include <mvvm/widgets/widget_utils.h>

#include <sup/gui/core/anyvalue_conversion_utils.h>
#include <sup/gui/core/anyvalue_item.h>

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
  AddAnyValueStruct(selected_as_parent ? m_context.get_selected_callback()
                                       : m_model->GetRootItem());
}

void AnyValueEditorActions::OnAddAnyValueArray(bool to_selected)
{
  try
  {
    auto parent = to_selected ? m_context.get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      m_model->InsertItem<sup::gui::AnyValueArrayItem>(parent, mvvm::TagIndex::Append())
          ->SetDisplayName("array");
    }
  }
  catch (const std::exception& ex)
  {
    QMessageBox::warning(nullptr, "Logic error", "Can't insert field into selected item");
  }
}

void AnyValueEditorActions::OnAddAnyValueScalar(const std::string& scalar_type, bool to_selected)
{
  try
  {
    auto parent = to_selected ? m_context.get_selected_callback() : m_model->GetRootItem();
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
    QMessageBox::warning(nullptr, "Logic error", "Can't insert field into selected item");
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
        ->SetDisplayName("struct");
  }
  catch (const std::exception& ex)
  {
    auto message = sup::gui::CreateInvalidOperationMessage("Can't insert structure");
    m_context.send_message_callback(message);
  }
}

}  // namespace anyvalueeditor
