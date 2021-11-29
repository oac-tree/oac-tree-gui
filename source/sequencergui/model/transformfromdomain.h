/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_TRANSFORMFROMDOMAIN_H
#define SEQUENCERGUI_MODEL_TRANSFORMFROMDOMAIN_H

//! @file transformfromdomain.h
//! Collection of utility functions to convert sup::sequencer::Procedure into SessionModel.

#include "sequencergui/model/sequencer_types.h"

#include <memory>
#include <string>

namespace sequi
{
class InstructionItem;
class InstructionContainerItem;
class WorkspaceItem;
class VariableItem;

//! Creates VariableItem from string representing Type of sup::sequencer::Variable.
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

//! Returns item's type corresponding to given domain type
std::string GetInstructionItemType(const std::string& domain_type);

//! Creates InstructionItem from string representing Type of sup::sequencer::Instruction.
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

//! Populates empty InstructionContainerItem with the content from sequencer Procedure.
void PopulateInstructionContainerItem(const procedure_t* procedure,
                                      InstructionContainerItem* container);

//! Populates empty WorkspaceItem with the content from sequencer Procedure.
void PopulateWorkspaceItem(const procedure_t* procedure, WorkspaceItem* workspace);

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_TRANSFORMFROMDOMAIN_H
