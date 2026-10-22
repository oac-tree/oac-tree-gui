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

#ifndef OAC_TREE_GUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform AnyValueItem from/to domain.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/oac-tree/attribute_info.h>

#include <memory>
#include <string>
#include <vector>

namespace sup::gui
{
class AnyValueItem;
}  // namespace sup::gui

namespace oac_tree_gui
{

class VariableItem;
class InstructionItem;
class ProcedurePreambleItem;

/**
 * @brief Creates an empty AnyValueItem with the default display name. It means not set.
 */
std::unique_ptr<sup::gui::AnyValueItem> CreateEmptyAnyValueItem();

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
 * @details Works for instructions with JSON type and value attributes: ChannelAccessWrite,
 * PvAccessWrite and RPCClient. If AnyValueItem already exists, it will be replaced.
 */
void SetAnyValue(const anyvalue_t& anyvalue, InstructionItem& item);

/**
 * @brief Sets AnyValueItem on board of variable item from the JSON type and value attributes.
 *
 * Does nothing without the type attribute. Without the value attribute, the value is the default
 * of the type.
 *
 * @param attributes The name/value strings of the domain variable.
 * @param variable_item The variable item.
 * @param registry Optional type registry containing already existing domain types.
 */
void SetAnyValueFromAttributes(const std::vector<sup::oac_tree::AttributeInfo>& attributes,
                               VariableItem& variable_item,
                               const anytype_registry_t* registry = nullptr);

/**
 * @brief Sets AnyValueItem on board of instruction item using JSON type and value strings.
 *
 * @param type_json The JSON type attribute.
 * @param value_json The JSON value attribute.
 * @param item Instruction item with tag intended for AnyValueItem.
 */
void SetAnyValueFromJsonString(const std::string& type_json, const std::string& value_json,
                               InstructionItem& item);

/**
 * @brief Updates existing AnyValueItem on board of variable_item using given anyvalue.
 *
 * @details It is expected that the layout of existing AnyValueItem matches the layout of anyvalue.
 */
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

/**
 * @brief Populates domain preamble from ProcedurePreambleItem.
 */
void PopulateProcedurePreamble(const ProcedurePreambleItem& item, preamble_t& preamble);

/**
 * @brief Returns domain AnyValue stored in given variable item, empty if it has no AnyValueItem.
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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_ANYVALUE_ITEM_TRANSFORM_HELPER_H_
