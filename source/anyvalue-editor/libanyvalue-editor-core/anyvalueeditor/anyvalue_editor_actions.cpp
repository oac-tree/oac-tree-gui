/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "anyvalueeditor/anyvalue_editor_actions.h"

#include <anyvalueeditor/add_field_dialog.h>
#include <anyvalueeditor/anyvalue_item.h>
#include <anyvalueeditor/conversion_utils.h>
#include <mvvm/model/application_model.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>
#include <QMessageBox>

namespace anyvalueeditor
{

AnyValueEditorActions::AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent,
                                             callback_t get_selected_callback)
    : QObject(parent), m_model(model), m_get_selected_callback(get_selected_callback)
{
}

void AnyValueEditorActions::OnAddAnyValueStruct(bool to_selected)
{
  try
  {
    auto parent = to_selected ? m_get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      m_model->InsertItem<AnyValueStructItem>(parent, mvvm::TagIndex::Append())
          ->SetDisplayName("struct");
    }
  }
  catch (const std::exception& ex)
  {
    QMessageBox::warning(nullptr, "Logic error", "Can't insert field into selected item");
  }
}

void AnyValueEditorActions::OnAddAnyValueArray(bool to_selected)
{
  try
  {
    auto parent = to_selected ? m_get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      m_model->InsertItem<AnyValueArrayItem>(parent, mvvm::TagIndex::Append())
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
    auto parent = to_selected ? m_get_selected_callback() : m_model->GetRootItem();
    if (parent)
    {
      auto scalar = m_model->InsertItem<AnyValueScalarItem>(parent, mvvm::TagIndex::Append());
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
  if (!m_get_selected_callback())
  {
    return;
  }
  m_model->RemoveItem(m_get_selected_callback());
}

}  // namespace anyvalueeditor
