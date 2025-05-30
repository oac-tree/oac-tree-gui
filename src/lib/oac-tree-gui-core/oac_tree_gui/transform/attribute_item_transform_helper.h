﻿/******************************************************************************
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
//! from/to domain attributes.

#include <oac_tree_gui/domain/sequencer_types_fwd.h>

#include <sup/gui/model/anyvalue_item.h>

#include <mvvm/model/compound_item.h>

#include <string>

namespace oac_tree_gui
{

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
bool GetAttributeExposedFlag(const sup::gui::AnyValueItem& attribute_item);

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
 * @brief Add property to item using sequencer attribute definition.
 *
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @return Returns item representing a new property.
 */
sup::gui::AnyValueItem* AddPropertyFromDefinition(const attribute_definition_t& attr,
                                                  mvvm::CompoundItem& item);

/**
 * @brief Set property item from the domain attribute.
 *
 * @tparam Domain instruction or variable.
 *
 * @param variable Domain object to set from.
 * @param attribute_name The attribute name of the domain object.
 * @param item An item representing an attribute.
 */

template <typename T>
void SetPropertyFromDomainAttribute(const T& domain, const std::string& attribute_name,
                                    sup::gui::AnyValueItem& item);

extern template void SetPropertyFromDomainAttribute<variable_t>(const variable_t& domain,
                                                                const std::string& attribute_name,
                                                                sup::gui::AnyValueItem& item);
extern template void SetPropertyFromDomainAttribute<instruction_t>(
    const instruction_t& domain, const std::string& attribute_name, sup::gui::AnyValueItem& item);

/**
 * @brief Set an attribute for domain variable from AnyValueScalarItem property.
 *
 * @tparam Domain instruction or variable.
 *
 * @param item The AnyValueScalarItem representing domain attribute.
 * @param attribute_name The name of the domain attribute to set.
 * @param variable The domain sequencer variable.
 */

template <typename T>
void SetDomainAttribute(const sup::gui::AnyValueItem& item, const std::string& attribute_name,
                        T& domain);

extern template void SetDomainAttribute<variable_t>(const sup::gui::AnyValueItem& item,
                                                    const std::string& attribute_name,
                                                    variable_t& domain);
extern template void SetDomainAttribute<instruction_t>(const sup::gui::AnyValueItem& item,
                                                       const std::string& attribute_name,
                                                       instruction_t& domain);

/**
 * @brief Sets attribute to given value.

 * Simultaneously marks an attribute as "present" and set the value to it.
 *
 * @param name The name of the attribute.
 * @param value The attribute value
 */
template <typename T>
void SetAttribute(const mvvm::CompoundItem& item, const std::string& name, const T& value)
{
  auto attribute = item.GetItem<sup::gui::AnyValueItem>(name);
  SetAttributeExposedFlag(true, *attribute);
  attribute->SetData(value);
}

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
extern template bool HasAttributeDefinition<instruction_t>(const instruction_t& domain,
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
