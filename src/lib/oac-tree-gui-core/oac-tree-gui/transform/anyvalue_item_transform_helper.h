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

#ifndef SEQUENCERGUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform AnyValueItem from/to domain.

#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <string>

namespace mvvm
{
class CompoundItem;
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{

class VariableItem;
class InstructionItem;
class ProcedurePreambleItem;

/**
 * @brief Sets AnyValueItem on board of variable_item using given anyvalue.
 *
 * @details If AnyValueItem already exist, it will be replaced.
 */
void SetAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

/**
 * @brief Sets AnyValueItem on board of instruction item using given anyvalue.
 *
 * @param anyvalue Domain AnyValue.
 * @param item Instruction item capable of storing AnyValueItem.
 *
 * @details Works for ChannelAccessWriteInstructionItem, PvAccessWriteInstructionItem and
 * RPCClientInstructi. If AnyValueItem already exist, it will be replaced.
 */
void SetAnyValue(const anyvalue_t& anyvalue, InstructionItem& item);

/**
 * @brief Sets AnyValueItem on board of variable item using domain variable.
 */
void SetAnyValueFromDomainVariable(const variable_t& variable, VariableItem& variable_item,
                                   const anytype_registry_t* registry = nullptr);

/**
 * @brief Sets AnyValueItem on board of instruction item using domain instruction.
 *
 * @param instruction Domain instruction with JSON type and value attribute defined.
 * @param item Instruction item with tag intended for AnyValueItem.
 */
void SetAnyValueFromDomainInstruction(const instruction_t& instruction, InstructionItem& item);

/**
 * @brief Updates existing AnyValueItem on board of variable_item using given anyvalue.
 *
 * @details It is expected that the layout of existing AnyValueItem matches the layout of anyvalue.
 */
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

/**
 * @brief Register tag in instruction item to be able to add other instructions as children.
 *
 * @param instruction The domain instruction is used to deduce category (compound, decorator).
 * @param item An item where tag will be registered.
 */
void RegisterChildrenTag(const instruction_t& instruction, mvvm::CompoundItem& item);

/**
 * @brief Populates domain preamble from ProcedurePreambleItem.
 */
void PopulateProcedurePreamble(const ProcedurePreambleItem& item, preamble_t& preamble);

/**
 * @brief Returns domain AnyValue stored in given variable item.
 */
sup::dto::AnyValue GetAnyValue(const VariableItem& item);

/**
 * @brief Returns AnyValue from workspace variable.
 *
 * Requires the setup of the workspace. Will throw if variables doesn't exist, of workspace wasn't
 * setup properly.
 */
sup::dto::AnyValue GetAnyValue(const std::string& var_name,
                               const sup::oac_tree::Workspace& workspace);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_
