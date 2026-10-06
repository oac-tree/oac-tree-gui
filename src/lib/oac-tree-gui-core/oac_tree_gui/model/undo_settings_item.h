/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MODEL_UNDO_SETTINGS_ITEM_H_
#define OAC_TREE_GUI_MODEL_UNDO_SETTINGS_ITEM_H_

#include <oac_tree_gui/model/sequencer_settings_constants.h>

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{

//! Settings group of undo settings.
constexpr auto kUndoSettingsGroup = "Settings/Undo";

/**
 * @brief The UndoInfo struct holds undo/redo settings of the sequencer model.
 */
struct UndoInfo
{
  bool is_enabled{constants::kUseUndoDefault};
  mvvm::int32 undo_limit{constants::kUndoLimitDefault};
};

template <typename Archive>
void Visit(Archive& archive, UndoInfo& info)
{
  archive("is_enabled", info.is_enabled);
  archive("undo_limit", info.undo_limit);
}

/**
 * @brief The UndoSettingsItem class holds undo/redo settings for the settings dialog.
 *
 * The "undo limit" property is disabled while undo is disabled.
 */
class UndoSettingsItem : public mvvm::CompoundItem
{
public:
  UndoSettingsItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns undo settings.
   */
  UndoInfo GetUndoInfo() const;

  /**
   * @brief Sets undo settings.
   */
  void SetUndoInfo(const UndoInfo& info);

protected:
  void ActivateStrategies() override;

private:
  //! Sets strategy updating enabled status of "undo limit" property.
  void ActivateStrategiesImpl();

  //! Sets undo flag and enables or disables "undo limit" property.
  bool OnSetFlag(SessionItem* property, const mvvm::variant_t& variant, mvvm::role_t role);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::UndoSettingsItem>
{
  static constexpr std::string_view type_name() noexcept { return "UndoSettings"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_UNDO_SETTINGS_ITEM_H_
