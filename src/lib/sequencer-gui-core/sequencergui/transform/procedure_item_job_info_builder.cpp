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

#include "procedure_item_job_info_builder.h"

#include "instruction_item_transform_helper.h"
#include "variable_item_transform_helper.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>

#include <sup/sequencer/instruction_info.h>
#include <sup/sequencer/job_info.h>
#include <sup/sequencer/variable_info.h>
#include <sup/sequencer/workspace_info.h>

namespace sequencergui
{

ProcedureItemJobInfoBuilder::ProcedureItemJobInfoBuilder() = default;

std::unique_ptr<ProcedureItem> ProcedureItemJobInfoBuilder::CreateProcedureItem(
    const sup::sequencer::JobInfo &job_info)
{
  auto result = std::make_unique<ProcedureItem>();

  result->SetDisplayName(job_info.GetProcedureName());

  auto instruction_tree = CreateInstructionItemTree(*job_info.GetRootInstructionInfo());
  m_instruction_indexes = std::move(instruction_tree.indexes);
  result->GetInstructionContainer()->InsertItem(std::move(instruction_tree.root),
                                                mvvm::TagIndex::Append());

  m_variable_indexes = PopulateWorkspaceItem(job_info.GetWorkspaceInfo(), result->GetWorkspace());

  return result;
}

InstructionItem *ProcedureItemJobInfoBuilder::GetInstruction(size_t index) const
{
  return index < m_instruction_indexes.size()
             ? const_cast<InstructionItem *>(m_instruction_indexes[index])
             : nullptr;
}

size_t ProcedureItemJobInfoBuilder::GetIndex(const InstructionItem *item) const
{
  auto pos = std::find(m_instruction_indexes.begin(), m_instruction_indexes.end(), item);
  if (pos == m_instruction_indexes.end())
  {
    throw RuntimeException("Can't find automation index for given item");
  }

  return std::distance(m_instruction_indexes.begin(), pos);
}

VariableItem *ProcedureItemJobInfoBuilder::GetVariable(size_t index) const
{
  return index < m_variable_indexes.size() ? const_cast<VariableItem *>(m_variable_indexes[index])
                                           : nullptr;
}

}  // namespace sequencergui
