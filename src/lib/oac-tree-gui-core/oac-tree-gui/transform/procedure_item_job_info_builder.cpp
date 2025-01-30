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

#include "procedure_item_job_info_builder.h"

#include "instruction_item_transform_helper.h"
#include "variable_item_transform_helper.h"

#include <oac-tree-gui/core/exceptions.h>
#include <oac-tree-gui/model/instruction_container_item.h>
#include <oac-tree-gui/model/instruction_item.h>
#include <oac-tree-gui/model/procedure_item.h>

#include <sup/oac-tree/instruction_info.h>
#include <sup/oac-tree/job_info.h>
#include <sup/oac-tree/variable_info.h>
#include <sup/oac-tree/workspace_info.h>

namespace oac_tree_gui
{

ProcedureItemJobInfoBuilder::ProcedureItemJobInfoBuilder() = default;

std::unique_ptr<ProcedureItem> ProcedureItemJobInfoBuilder::CreateProcedureItem(
    const sup::oac_tree::JobInfo &job_info)
{
  auto result = std::make_unique<ProcedureItem>();

  result->SetDisplayName(job_info.GetProcedureName());

  auto instruction_tree = CreateInstructionItemTree(*job_info.GetRootInstructionInfo());
  m_index_to_instruction = std::move(instruction_tree.indexes);
  result->GetInstructionContainer()->InsertItem(std::move(instruction_tree.root),
                                                mvvm::TagIndex::Append());

  m_index_to_variable = PopulateWorkspaceItem(job_info.GetWorkspaceInfo(), result->GetWorkspace());

  return result;
}

InstructionItem *ProcedureItemJobInfoBuilder::GetInstruction(std::size_t index) const
{
  return index < m_index_to_instruction.size()
             ? const_cast<InstructionItem *>(m_index_to_instruction[index])
             : nullptr;
}

std::size_t ProcedureItemJobInfoBuilder::GetIndex(const InstructionItem *item) const
{
  auto pos = std::find(m_index_to_instruction.begin(), m_index_to_instruction.end(), item);
  if (pos == m_index_to_instruction.end())
  {
    throw RuntimeException("Can't find automation index for given item");
  }

  return std::distance(m_index_to_instruction.begin(), pos);
}

VariableItem *ProcedureItemJobInfoBuilder::GetVariable(std::size_t index) const
{
  return index < m_index_to_variable.size() ? const_cast<VariableItem *>(m_index_to_variable[index])
                                            : nullptr;
}

}  // namespace oac_tree_gui
