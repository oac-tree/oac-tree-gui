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
#include <anyvalueeditor/anyvalue_utils.h>
#include <mvvm/model/application_model.h>
#include <mvvm/widgets/widget_utils.h>

#include <QMainWindow>

namespace anyvalueeditor
{

AnyValueEditorActions::AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
}

void AnyValueEditorActions::OnAddAnyValueStruct(bool to_selected)
{
  auto parent = to_selected ? m_selected_item : m_model->GetRootItem();
  if (parent)
  {
    m_model->InsertItem<AnyValueStructItem>(parent, mvvm::TagIndex::Append())
        ->SetDisplayName("struct");
  }
}

void AnyValueEditorActions::OnAddAnyValueArray(bool to_selected)
{
  auto parent = to_selected ? m_selected_item : m_model->GetRootItem();
  if (parent)
  {
    m_model->InsertItem<AnyValueArrayItem>(parent, mvvm::TagIndex::Append())
        ->SetDisplayName("array");
  }
}

void AnyValueEditorActions::OnAddAnyValueScalar(const std::string& scalar_type, bool to_selected)
{
  auto parent = to_selected ? m_selected_item : m_model->GetRootItem();
  if (parent)
  {
    auto scalar = m_model->InsertItem<AnyValueScalarItem>(parent, mvvm::TagIndex::Append());
    scalar->SetAnyTypeName(scalar_type);
    scalar->SetDisplayName(scalar_type);
  }
}

void AnyValueEditorActions::OnAddField()
{
  if (!m_selected_item)
  {
    return;
  }

  // FIXME restore
  //  AddFieldDialog dialog(mvvm::utils::FindMainWindow());
  //  if (dialog.exec() == QDialog::Accepted)
  //  {
  //    auto context = dialog.GetFieldContext();
  //    auto parent = m_selected_item->GetParent();
  //    auto field = m_model->InsertItem<AnyValueItem>(parent,
  //    m_selected_item->GetTagIndex().Next()); field->SetDisplayName(context.name);
  //    field->SetAnyTypeName(context.subtype);
  //  }
}

void AnyValueEditorActions::OnInsertField() {}

void AnyValueEditorActions::OnRemoveSelected()
{
  if (!m_selected_item)
  {
    return;
  }
  m_model->RemoveItem(m_selected_item);
}

void AnyValueEditorActions::SetSelectedItem(AnyValueItem* item)
{
  m_selected_item = item;
}

}  // namespace anyvalueeditor
