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

void AnyValueEditorActions::OnRemoveSelected() {}

void AnyValueEditorActions::SetViewModel(mvvm::ViewModel* view_model)
{
  m_view_model = view_model;
}

void AnyValueEditorActions::SetSelectionModel(QItemSelectionModel* selection_model)
{
  m_selection_model = selection_model;
}

}  // namespace anyvalueeditor
