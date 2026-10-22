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

#ifndef OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
#define OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem and InstructionItem properties
//! from/to domain attributes. Instruction and variable attributes are AttributeItem. The functions
//! on AnyValueItem serve the JSON type/value pair and the exposed flag of the attribute editor.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/gui/model/anyvalue_item.h>

#include <string>

namespace oac_tree_gui
{

class AttributeItem;

/**
 * @brief Returns true if given attribute value is referring to `$placeholder`.
 */
bool IsPlaceholderAttribute(const std::string& attribute_value);

/**
 * @brief Returns true if given attribute value is referring to `@variable`.
 */
bool IsReferenceAttribute(const std::string& attribute_value);

/**
 * @brief Returns true if this attribute item is marked as exposed.
 *
 * The flag determines whether the attribute should be propagated into the domain. The
 * item with exposed flag set to false appears disabled (in gray) and non-editable.
 *
 * @param attribute_item The item to set attribute.
 * @return Boolean representing the value of exposed flag.
 */
bool GetAttributeExposedFlag(const mvvm::SessionItem& attribute_item);

/**
 * @brief Sets attribute exposed flag to a given value.
 *
 * The flag determines whether the attribute should be propagated into the domain. The
 * item with exposedt flag set to false appears disabled (in gray) and non-editable.
 *
 * @param value The value of exposed flag.
 * @param attribute_item The item to set the flag.
 */
void SetAttributeExposedFlag(bool value, sup::gui::AnyValueItem& attribute_item);

/**
 * @brief Sets attribute value as string.
 *
 * Internal data will be replaced with a string. This can happen when corresponding
 * domain attribute contains is a placeholder or a reference to variable name.
 *
 * @param value The string wich will be used as new attribute value.
 * @param attribute_item The item to set attribute.
 */
void SetAttributeAsString(const std::string& value, sup::gui::AnyValueItem& attribute_item);

/**
 * @brief Sets default constructed attribute value from AnyType name.
 */
void SetAttributeFromTypeName(sup::gui::AnyValueItem& attribute_item);

/**
 * @brief Sets the attribute item from the domain attribute string.
 *
 * Placeholders and references are stored as text if the attribute allows text. Other values are
 * parsed by the attribute type; the item is left unchanged if parsing fails.
 *
 * @param attribute_string The attribute value.
 * @param item The attribute item.
 */
void SetAttributeFromString(const std::string& attribute_string, AttributeItem& item);

/**
 * @brief Returns the domain attribute string of the attribute item.
 *
 * Text is returned as is. A value is converted by the oac-tree attribute system.
 *
 * @param item The attribute item.
 * @return The attribute string, empty if the attribute is not set.
 * @throw LogicErrorException if the value can't be converted.
 */
std::string GetAttributeString(const AttributeItem& item);

/**
 * @brief Sets an attribute of the domain object from the attribute item.
 *
 * A not set attribute, an empty string and `isRoot = false` are skipped.
 *
 * @tparam T Domain instruction or variable.
 * @param item The attribute item.
 * @param attribute_name The name of the domain attribute to set.
 * @param domain The domain object.
 */
template <typename T>
void SetDomainAttribute(const AttributeItem& item, const std::string& attribute_name, T& domain);

extern template void SetDomainAttribute<instruction_t>(const AttributeItem& item,
                                                       const std::string& attribute_name,
                                                       instruction_t& domain);
extern template void SetDomainAttribute<variable_t>(const AttributeItem& item,
                                                    const std::string& attribute_name,
                                                    variable_t& domain);

/**
 * @brief Checks if given instruction or variable contains attribute definition with the given name.
 *
 * @tparam DomainT oac-tree domain type (instruction or variable).
 *
 * @param domain Domain object to check.
 */
template <typename DomainT>
bool HasAttributeDefinition(const DomainT& domain, const std::string& definition_name);

extern template bool HasAttributeDefinition<variable_t>(const variable_t& domain,
                                                        const std::string& definition_name);

/**
 * @brief Sets domain JSON type and value attributes, if necessary, for given domain instruction or
 * variable.
 *
 * @tparam DomainT oac-tree domain type (instruction or variable).
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

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
