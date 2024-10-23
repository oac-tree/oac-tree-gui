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

#ifndef SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_AUTOMATION_BUILDER_H_
#define SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_AUTOMATION_BUILDER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>
#include <vector>

namespace sequencergui
{

class ProcedureItem;
class InstructionTree;
class InstructionItem;
class VariableItem;

/**
 * @brief The ProcedureItemAutomationBuilder class creates ProcedureItem from automation server
 * information.
 *
 * Contains machinery to get instruction and variable pointers from automation indexes.
 */
class ProcedureItemAutomationBuilder
{
public:
  ProcedureItemAutomationBuilder();

  /**
   * @brief Creates procedure item from given automation server information.
   */
  std::unique_ptr<ProcedureItem> CreateProcedureItem(const sup::sequencer::JobInfo &job_info);

  /**
   * @brief Returns instruction item for given automation index.
   */
  const InstructionItem* GetInstruction(size_t index) const;

  /**
   * @brief Returns variable item for given automation index.
   */
  const VariableItem* GetVariable(size_t index) const;

private:
  std::vector<const InstructionItem*> m_instruction_indexes;
  std::vector<const VariableItem*> m_variable_indexes;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_AUTOMATION_BUILDER_H_
