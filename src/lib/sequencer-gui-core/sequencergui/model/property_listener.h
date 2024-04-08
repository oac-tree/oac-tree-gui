/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_PROPERTY_LISTENER_H_
#define SEQUENCERGUI_MODEL_PROPERTY_LISTENER_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/item_listener.h>

namespace sequencergui
{

//! FIXME Temporary class for listening item property, remove after ItemListener refactoring.

template <typename ItemT>
class PropertyListener : public mvvm::ItemListener<ItemT>
{
public:
  explicit PropertyListener(ItemT* item) { this->SetItem(item); }
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_PROPERTY_LISTENER_H_
