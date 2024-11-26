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

#ifndef SEQUENCERGUI_MODEL_SETTINGS_ITEM_H_
#define SEQUENCERGUI_MODEL_SETTINGS_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{
class PropertyChangedEvent;
}

namespace sequencergui
{

/**
 * @brief The CommonSettingsItem class represents a group of individual application settings.
 *
 * It's a compound item, where each property corresponds to an individual application settings.
 */
class CommonSettingsItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "CommonSettings";

  CommonSettingsItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @details Activates listening on own properties change. It will enable/disable status of undo
   * limit property, when undo flag changes.
   */
  void Activate() override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_SETTINGS_ITEMS_H_
