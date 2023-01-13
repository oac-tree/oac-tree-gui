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

#include <mvvm/model/sessionitem.h>
#include <mvvm/model/tagindex.h>

namespace testutils
{

void MockItemListener::Subscribe()
{
  auto on_item_inserted = [this](mvvm::SessionItem* item, const mvvm::TagIndex& tag_index)
  { OnItemInserted(item, tag_index); };
  SetOnItemInserted(on_item_inserted);

  auto on_about_to_remove_item = [this](mvvm::SessionItem* item, const mvvm::TagIndex& tag_index)
  { OnAboutToRemoveItem(item, tag_index); };
  SetOnAboutToRemoveItem(on_about_to_remove_item);

  auto on_item_removed = [this](mvvm::SessionItem* item, const mvvm::TagIndex& tag_index)
  { OnItemRemoved(item, tag_index); };
  SetOnItemRemoved(on_item_removed);

  auto on_data_changed = [this](mvvm::SessionItem* item, int role) { OnDataChanged(item, role); };
  SetOnDataChanged(on_data_changed);

  auto on_property_changed = [this](mvvm::SessionItem* item, const std::string& name)
  { OnPropertyChanged(item, name); };
  SetOnPropertyChanged(on_property_changed);
}

}  // namespace testutils
