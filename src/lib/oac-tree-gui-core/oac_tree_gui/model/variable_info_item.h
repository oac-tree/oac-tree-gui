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

#ifndef OAC_TREE_GUI_MODEL_VARIABLE_INFO_ITEM_H_
#define OAC_TREE_GUI_MODEL_VARIABLE_INFO_ITEM_H_

#include <oac_tree_gui/model/variable_item.h>

namespace oac_tree_gui
{

/**
 * @brief The VariableInfoItem is a simplified variable to monitor job execution.
 *
 * It is build from JobInfo domain information and do not requires plugin load.
 */
class VariableInfoItem : public VariableItem
{
public:
  VariableInfoItem();

  std::unique_ptr<SessionItem> Clone() const override;

  void InitFromDomainInfo(const sup::oac_tree::VariableInfo& info);

private:
  void InitFromDomainImpl(const variable_t* variable, const anytype_registry_t* registry) override;
  void SetupDomainImpl(variable_t* variable) const override;

  void SetupFromDomain(const sup::oac_tree::VariableInfo& info);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::VariableInfoItem>
{
  static constexpr std::string_view type_name() noexcept { return "VariableInfoItem"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_VARIABLE_INFO_ITEM_H_
