/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
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

#ifndef OAC_TREE_GUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_
#define OAC_TREE_GUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_

#include <mvvm/model/compound_item.h>

namespace mvvm
{
class ContainerItem;
}

namespace oac_tree_gui
{

//! Represents TypeRegistrationInfo.

class TypeRegistrationItem : public mvvm::CompoundItem
{
public:
  TypeRegistrationItem();

  std::unique_ptr<SessionItem> Clone() const override;

  int GetRegistrationMode() const;
  void SetRegistrationMode(int value);

  std::string GetRegistrationString() const;
  void SetRegistrationString(const std::string& value);
};

//! Represents ProcedurePreamble.

class ProcedurePreambleItem : public mvvm::CompoundItem
{
public:
  ProcedurePreambleItem();

  std::unique_ptr<SessionItem> Clone() const override;

  mvvm::ContainerItem* GetTypeRegistrationContainer() const;

  mvvm::ContainerItem* GetPluginContainer() const;

  void AddTypeRegistration(int mode, const std::string& str);
  std::vector<std::pair<int, std::string>> GetTypeRegistrations() const;

  std::vector<std::string> GetPluginPaths() const;
  void AddPluginPath(const std::string& value);

  /**
   * @brief Sets plugin names, all previous names will be removed.
   */
  void SetPluginPaths(const std::vector<std::string>& plugin_paths);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::TypeRegistrationItem>
{
  static constexpr std::string_view type_name() noexcept { return "TypeRegistrationItem"; }
};

template <>
struct item_traits<oac_tree_gui::ProcedurePreambleItem>
{
  static constexpr std::string_view type_name() noexcept { return "ProcedurePreamble"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_PROCEDURE_PREAMBLE_ITEMS_H_
