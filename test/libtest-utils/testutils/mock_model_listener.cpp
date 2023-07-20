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

#include "mock_model_listener.h"

namespace testutils
{

MockModelListener::MockModelListener(const mvvm::SessionModelInterface *model)
    : ModelListener(model)
{
  Connect<mvvm::DataChangedEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::AboutToInsertItemEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::ItemInsertedEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::ModelAboutToBeResetEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::ModelResetEvent>(this, &MockModelListener::OnEvent);
  Connect<mvvm::ModelAboutToBeDestroyedEvent>(this, &MockModelListener::OnEvent);
}

MockModelListenerV2::MockModelListenerV2(const mvvm::SessionModelInterface *model)
    : ModelListener(model)
{
  Connect<mvvm::DataChangedEvent>(this, &MockModelListenerV2::OnDataChangedEvent);

  Connect<mvvm::AboutToInsertItemEvent>(this, &MockModelListenerV2::OnAboutToInsertItemEvent);
  Connect<mvvm::ItemInsertedEvent>(this, &MockModelListenerV2::OnItemInsertedEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockModelListenerV2::OnAboutToRemoveItemEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockModelListenerV2::OnItemRemovedEvent);

  Connect<mvvm::ModelAboutToBeResetEvent>(this, &MockModelListenerV2::OnModelAboutToBeResetEvent);
  Connect<mvvm::ModelResetEvent>(this, &MockModelListenerV2::OnModelResetEvent);

  Connect<mvvm::ModelAboutToBeDestroyedEvent>(this,
                                              &MockModelListenerV2::OnModelAboutToBeDestroyedEvent);
}

}  // namespace testutils
