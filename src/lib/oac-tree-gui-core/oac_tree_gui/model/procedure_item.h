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

#ifndef OAC_TREE_GUI_MODEL_PROCEDURE_ITEM_H_
#define OAC_TREE_GUI_MODEL_PROCEDURE_ITEM_H_

#include <mvvm/model/compound_item.h>

#include <functional>

namespace oac_tree_gui
{

class WorkspaceItem;
class InstructionContainerItem;
class ProcedurePreambleItem;
class DomainObjectTypeRegistry;

//! Represents Procedure.
class ProcedureItem : public mvvm::CompoundItem
{
public:
  ProcedureItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetName() const;

  void SetName(const std::string& value);

  std::string GetDescription() const;

  void SetDescription(const std::string& value);

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  InstructionContainerItem* GetInstructionContainer() const;
  WorkspaceItem* GetWorkspace() const;

  ProcedurePreambleItem* GetPreambleItem() const;
};

/**
 * @brief Collect domain plugin names that are required to run this procedure.
 *
 * @param item The procedure to investigate.
 * @param object_to_plugin_name A callback allowing to retrieve plugin name from object name.
 */
std::vector<std::string> CollectPluginNames(
    const ProcedureItem& item,
    const std::function<std::string(const std::string&)>& object_to_plugin_name);

/**
 * @brief Updates preamble with plugin names using current instruction and variable composition.
 *
 * @param item The procedure to update preamble.
 * @param object_to_plugin_name A callback allowing to retrieve plugin name from object name.
 */
void UpdatePluginNames(const ProcedureItem& item,
                       const std::function<std::string(const std::string&)>& object_to_plugin_name);

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_PROCEDURE_ITEM_H_
