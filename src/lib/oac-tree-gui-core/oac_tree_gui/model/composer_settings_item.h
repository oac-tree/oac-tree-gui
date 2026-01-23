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

#ifndef OAC_TREE_GUI_MODEL_COMPOSER_SETTINGS_ITEM_H_
#define OAC_TREE_GUI_MODEL_COMPOSER_SETTINGS_ITEM_H_

#include <mvvm/model/compound_item.h>
#include <oac_tree_gui/model/component_types.h>

namespace oac_tree_gui
{

/**
 * @brief The ComposerSettingsItem class holds settings related to the main composer view.
 *
 * These settings are part of the project file; they hold information related to procedure editing
 * in various widgets.
 */
class ComposerSettingsItem : public mvvm::CompoundItem
{
public:
  static constexpr auto kEditorInfoList = "kEditorInfoList";
  ComposerSettingsItem();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Returns info for opened panels of splittable procedure editor.
   */
  std::vector<ProcedureEditorInfo> GetProcedureEditorInfoList() const;

  /**
   * @brief Sets procedure info list.
   */
  void SetProcedureEditorInfoList(const std::vector<ProcedureEditorInfo>& editor_info);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::ComposerSettingsItem>
{
  static constexpr std::string_view type_name() noexcept { return "ComposerSettingsItem"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_COMPOSER_SETTINGS_ITEM_H_
