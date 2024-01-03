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

#ifndef SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPERS_H_
#define SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPERS_H_

//! @file
//! Collection of helper functions to transform AnyValueItem from/to domain.

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sup/gui/core/dto_types_fwd.h>

#include <memory>
#include <string>

namespace mvvm
{
class CompoundItem;
}

namespace sequencergui
{

class VariableItem;
class ProcedurePreambleItem;

/**
 * @brief Sets AnyValueItem on board of variable_item using given anyvalue.
 *
 * @details If AnyValueItem already exist, it will be replaced.
 */
void SetAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

/**
 * @brief Sets AnyValueItem on board of variable_item using json_type provided.
 */
void SetAnyValueFromJsonType(const std::string& json_type, VariableItem& variable_item);

/**
 * @brief Sets AnyValueItem on board of variable item using domain variable.
 */
void SetAnyValueFromDomainVariable(const variable_t& variable, VariableItem& variable_item,
                                   const anytype_registry_t* registry = nullptr);

/**
 * @brief Updates existing AnyValueItem on board of variable_item using given anyvalue.
 *
 * @details It is expected that the layout of existing AnyValueItem matches the layout of anyvalue.
 */
void UpdateAnyValue(const anyvalue_t& anyvalue, VariableItem& variable_item);

/**
 * @brief Adds an attribute to given value. If the value is empty string, do nothing.
 */
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          variable_t& variable);

/**
 * @brief Sets JSON type attribute from given item to the domain variable.
 * @param item
 * @param variable
 */
void SetJsonTypeAttribute(const VariableItem& item, variable_t& variable);

/**
 * @brief Sets JSON value attribute from given item to the domain variable.
 */
void SetJsonValueAttribute(const VariableItem& item, variable_t& variable);

/**
 * @brief Adds an attribute to given value. If the value is empty string, do nothing.
 */
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          instruction_t& instruction);

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

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPERS_H_
