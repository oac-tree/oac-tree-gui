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

#include "procedure_item_automation_builder.h"

#include "automation_transform_helper.h"

#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/instruction_item.h>
#include <sequencergui/model/procedure_item.h>

#include <sup/auto-server/instruction_info.h>
#include <sup/auto-server/job_info.h>
#include <sup/auto-server/variable_info.h>
#include <sup/auto-server/workspace_info.h>

namespace sequencergui
{

ProcedureItemAutomationBuilder::ProcedureItemAutomationBuilder() = default;

std::unique_ptr<ProcedureItem> ProcedureItemAutomationBuilder::CreateProcedureItem(
    const sup::auto_server::JobInfo &job_info)
{
  auto result = std::make_unique<ProcedureItem>();

  result->SetDisplayName(job_info.GetPrefix());

  auto instruction_tree = CreateInstructionItemTree(*job_info.GetRootInstructionInfo());
  m_instruction_indexes = std::move(instruction_tree.indexes);
  result->GetInstructionContainer()->InsertItem(std::move(instruction_tree.root),
                                                mvvm::TagIndex::Append());

  return result;
}

const InstructionItem *ProcedureItemAutomationBuilder::GetInstruction(size_t index) const
{
  return index < m_instruction_indexes.size() ? m_instruction_indexes[index] : nullptr;
}

}  // namespace sequencergui
