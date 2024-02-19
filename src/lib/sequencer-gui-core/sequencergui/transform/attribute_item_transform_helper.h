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

#ifndef SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
#define SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_

//! @file
//! Collection of helper functions to transform VariableItem and InstructionItem properties
//! from/to domain attributes.

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <string>

namespace mvvm
{
class CompoundItem;
}  // namespace mvvm

namespace sequencergui
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
   * @brief Returns true if this attribute item is marked as present.
   *
   * The flag determines whether the attribute should be propagated into the domain. The
   * item with present flag set to false appears disabled (in gray) and non-editable.
   *
   * @param attribute_item The item to set attribute.
   * @return Boolean representing the value of IsPresent attribute.
   */
bool IsAttributePresent(const AttributeItem &attribute_item);

/**
   * @brief Sets attribute present flag to a given value.
   *
   * The flag determines whether the attribute should be propagated into the domain. The
   * item with present flag set to false appears disabled (in gray) and non-editable.
   *
   * @param value The value of present flag.
   * @param attribute_item The item to set attribute.
   */
void SetAttributePresentFlag(bool value, AttributeItem& attribute_item);

/**
   * @brief Sets attribute value as string.
   *
   * Internal data will be replaced with a string. This can happen when corresponding
   * domain attribute contains is a placeholder or a reference to variable name.
   *
   * @param value The string wich will be used as new attribute value.
   * @param attribute_item The item to set attribute.
   */
void SetAttributeAsString(const std::string& value, AttributeItem& attribute_item);

/**
   * @brief Sets default constructed attribute value from AnyType name.
   */
void SetAttributeFromTypeName(AttributeItem& attribute_item);

/**
 * @brief Add property to item using sequencer attribute definition.
 *
 * @param attr Attribute definition to use for property construction.
 * @param item Compound item which will get a property on board.
 *
 * @return Returns item representing a new property.
 */
AttributeItem* AddPropertyFromDefinition(const attribute_definition_t& attr,
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
                                    AttributeItem& item);

extern template void SetPropertyFromDomainAttribute<variable_t>(const variable_t& domain,
                                                                const std::string& attribute_name,
                                                                AttributeItem& item);
extern template void SetPropertyFromDomainAttribute<instruction_t>(
    const instruction_t& domain, const std::string& attribute_name, AttributeItem& item);

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
void SetDomainAttribute(const AttributeItem& item, const std::string& attribute_name, T& domain);

extern template void SetDomainAttribute<variable_t>(const AttributeItem& item,
                                                    const std::string& attribute_name,
                                                    variable_t& domain);
extern template void SetDomainAttribute<instruction_t>(const AttributeItem& item,
                                                       const std::string& attribute_name,
                                                       instruction_t& domain);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_TRANSFORM_ATTRIBUTE_ITEM_TRANSFORM_HELPER_H_
