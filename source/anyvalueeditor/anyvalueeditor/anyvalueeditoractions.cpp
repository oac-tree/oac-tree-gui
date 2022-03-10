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

#include "anyvalueeditor/anyvalueeditoractions.h"

#include "anyvalueeditor/anyvalueitem.h"

#include "mvvm/model/applicationmodel.h"

namespace anyvalueeditor
{

AnyValueEditorActions::AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
}

void AnyValueEditorActions::OnAddAnyValue()
{
  m_model->InsertItem<AnyValueItem>();
}

void AnyValueEditorActions::OnAddField() {}

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
