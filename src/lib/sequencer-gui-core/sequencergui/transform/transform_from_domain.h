/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_
#define SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_

//! Collection of utility functions to convert sup::sequencer::Procedure into SessionModel.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <memory>
#include <string>

namespace sequencergui
{
class InstructionItem;
class InstructionContainerItem;
class VariableItem;
class ProcedureItem;
class ProcedurePreambleItem;

//! Translates domain's type name to the model type of corresponding GUI item.
std::string GetItemType(const std::string& domain_type);

//! Creates VariableItem from string representing Type of sup::sequencer::Variable.
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

//! Creates InstructionItem from string representing Type of sup::sequencer::Instruction.
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

//! Creates ProcedureItem from given domain procedure. If flag `root_only` is true, will use process
//! one top-level root instruction.
std::unique_ptr<ProcedureItem> CreateProcedureItem(const procedure_t* procedure, bool root_only);

/**
 * @brief Populates empty PreambleItem with the domain information.
 */
void PopulateProcedurePreambleItem(const preamble_t& preamble, ProcedurePreambleItem& item);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_TRANSFORM_FROM_DOMAIN_H_
