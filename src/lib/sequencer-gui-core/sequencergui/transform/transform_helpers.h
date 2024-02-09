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
}  // namespace mvvm

namespace sup::gui
{
class AnyValueItem;
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
 * @brief Adds non-empty attribute to given domain instruction or variable.
 *
 * @tparam DomainT Sequencer domain type (instruction or variable).
 * @param attribute_name The name of the attribute.
 * @param attribute_value Non empty attribute value.
 * @param domain The domain object (instruction or variable).
 *
 * @details If attribute value is empty, will do nothing.
 */
template <typename DomainT>
void AddNonEmptyAttribute(const std::string& attribute_name, const std::string& attribute_value,
                          DomainT& domain);

extern template void AddNonEmptyAttribute<variable_t>(const std::string& attribute_name,
                                                      const std::string& attribute_value,
                                                      variable_t& domain);
extern template void AddNonEmptyAttribute<instruction_t>(const std::string& attribute_name,
                                                         const std::string& attribute_value,
                                                         instruction_t& domain);

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
 * @brief Checks if given instruction or variable contains attribute definition with the given name.
 *
 * @tparam DomainT Sequencer domain type (instruction or variable).
 *
 * @param domain Domain object to check.
 */
template <typename DomainT>
bool HasAttributeDefinition(const DomainT& domain, const std::string& definition_name);

extern template bool HasAttributeDefinition<variable_t>(const variable_t& domain,
                                                        const std::string& definition_name);
extern template bool HasAttributeDefinition<instruction_t>(const instruction_t& domain,
                                                           const std::string& definition_name);

/**
 * @brief Checks if given instruction or variable contains Anyvalue JSON type and name attributes.
 *
 * @tparam DomainT Sequencer domain type (instruction or variable).
 *
 * @param domain Domain object to check.
 */
template <typename DomainT>
bool HasJsonTypeAndNameAttributes(const DomainT& domain);

extern template bool HasJsonTypeAndNameAttributes<variable_t>(const variable_t& domain);
extern template bool HasJsonTypeAndNameAttributes<instruction_t>(const instruction_t& domain);

/**
 * @brief Sets domain JSON type and value attributes, if necessary, for given domain instruction or
 * variable.
 *
 * @tparam DomainT Sequencer domain type (instruction or variable).
 *
 * @param item AnyValueItem to use to construct JSON attributes
 * @param domain Domain object to set pair of attributes.
 *
 * @details This will first check if attributes are required, then convert given AnyValueItem
 * into AnyValue, and then set a pair of attributes with names "type" and "value" using AnyValue
 * JSON representation.
 */
template <typename DomainT>
void SetJsonAttributesFromItem(const sup::gui::AnyValueItem& item, DomainT& domain);

extern template void SetJsonAttributesFromItem<variable_t>(const sup::gui::AnyValueItem& item,
                                                           variable_t& domain);
extern template void SetJsonAttributesFromItem<instruction_t>(const sup::gui::AnyValueItem& item,
                                                              instruction_t& domain);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_TRANSFORM_HELPERS_H_
