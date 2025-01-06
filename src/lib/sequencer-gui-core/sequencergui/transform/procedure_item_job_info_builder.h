/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
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

#ifndef SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_
#define SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/transform/i_procedure_item_builder.h>

#include <memory>
#include <vector>

namespace sequencergui
{

class ProcedureItem;
class InstructionTree;
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
  std::unique_ptr<ProcedureItem> CreateProcedureItem(const sup::sequencer::JobInfo& job_info);

  InstructionItem* GetInstruction(size_t index) const override;

  size_t GetIndex(const InstructionItem* item) const override;

  VariableItem* GetVariable(size_t index) const override;

private:
  std::vector<const InstructionItem*> m_index_to_instruction;
  std::vector<const VariableItem*> m_index_to_variable;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_PROCEDURE_ITEM_JOB_INFO_BUILDER_H_
