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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_
#define SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace oac_tree_gui
{
class WorkspaceItem;
class InstructionContainerItem;
class ProcedurePreambleItem;

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
 */
std::vector<std::string> CollectPluginNames(const ProcedureItem& item);

/**
 * @brief Updates preamble with plugin names using current instruction and variable composition.
 */
void UpdatePluginNames(const ProcedureItem& item);

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_MODEL_PROCEDURE_ITEM_H_
