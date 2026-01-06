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

#ifndef OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_
#define OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_

#include <oac_tree_gui/domain/sequencer_types_fwd.h>
#include <oac_tree_gui/transform/i_procedure_item_builder.h>

#include <memory>
#include <vector>

namespace oac_tree_gui
{

class ProcedureItem;
struct InstructionTree;
class InstructionItem;
class VariableItem;

/**
 * @brief The ProcedureItemJobInfoBuilder class creates ProcedureItem from automation server
 * information.
 *
 * Contains machinery to get instruction and variable pointers from automation indexes.
 */
class ProcedureItemJobInfoBuilder : public IProcedureItemBuilder
{
public:
  ProcedureItemJobInfoBuilder();

  /**
   * @brief Creates procedure item from given automation server information.
   */
  std::unique_ptr<ProcedureItem> CreateProcedureItem(const sup::oac_tree::JobInfo& job_info);

  InstructionItem* GetInstruction(std::size_t index) const override;

  std::size_t GetIndex(const InstructionItem* item) const override;

  VariableItem* GetVariable(std::size_t index) const override;

private:
  std::vector<const InstructionItem*> m_index_to_instruction;
  std::vector<const VariableItem*> m_index_to_variable;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_
