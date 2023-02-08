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

#include "mock_item_listener.h"

#include "mvvm/signals/event_types.h"

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagindex.h>

namespace testutils
{

void MockItemListener::Subscribe()
{
  Connect<mvvm::ItemInsertedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::AboutToRemoveItemEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::ItemRemovedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::DataChangedEvent>(this, &MockItemListener::OnEvent);
  Connect<mvvm::PropertyChangedEvent>(this, &MockItemListener::OnEvent);
}

}  // namespace testutils
