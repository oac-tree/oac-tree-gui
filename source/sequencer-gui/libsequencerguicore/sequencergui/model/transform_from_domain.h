/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_TRANSFORM_FROM_DOMAIN_H_
#define SEQUENCERGUI_MODEL_TRANSFORM_FROM_DOMAIN_H_

//! @file transformfromdomain.h
//! Collection of utility functions to convert sup::sequencer::Procedure into SessionModel.

#include <sequencergui/model/sequencer_types.h>

#include <memory>
#include <string>

namespace sequencergui
{
class InstructionItem;
class InstructionContainerItem;
class WorkspaceItem;
class VariableItem;
class ProcedureItem;

//! Translates instruction's `domain_type` name to UI `model_type` name.
std::string GetItemType(const std::string& domain_type);

//! Creates VariableItem from string representing Type of sup::sequencer::Variable.
std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

//! Creates InstructionItem from string representing Type of sup::sequencer::Instruction.
std::unique_ptr<InstructionItem> CreateInstructionItem(const std::string& domain_type);

//! Populates empty InstructionContainerItem with the content from sequencer Procedure.
void PopulateInstructionContainerItem(const procedure_t* procedure,
                                      InstructionContainerItem* container, bool root_only);

//! Populates empty WorkspaceItem with the content from sequencer Procedure.
void PopulateWorkspaceItem(const procedure_t* procedure, WorkspaceItem* workspace);

//! Populates empty ProcedureItem with the content from sequencer Procedure.
void PopulateProcedureItem(const procedure_t* procedure, ProcedureItem* procedure_item,
                           bool root_only);

//! Creates UnknownInstructionItem from the domain type. The instruction will be initialised with
//! the default attribute map from the domain. See explanations in the implementation.
std::unique_ptr<InstructionItem> CreateUnknownInstructionItem(const std::string& domain_type);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_TRANSFORM_FROM_DOMAIN_H_
