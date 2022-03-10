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

namespace anyvalueeditor
{

AnyValueEditorActions::AnyValueEditorActions(mvvm::ApplicationModel* model, QObject* parent)
    : QObject(parent), m_model(model)
{
}

void AnyValueEditorActions::OnAddAnyValue()
{

}

void AnyValueEditorActions::OnAddField()
{

}

void AnyValueEditorActions::OnInsertField()
{

}

void AnyValueEditorActions::OnRemoveSelected()
{

}

}  // namespace anyvalueeditor
