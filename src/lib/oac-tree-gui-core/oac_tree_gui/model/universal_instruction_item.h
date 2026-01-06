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

#ifndef OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_

//! Collection of classes to represent oac-tree instructions.

#include <oac_tree_gui/model/instruction_item.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

/**
 * @brief The UniversalInstructionItem class represents domain Instruction.
 */

class UniversalInstructionItem : public InstructionItem
{
public:
  /**
   * @brief Main c-tor.
   *
   * Constructed item can be used either after SetDomainType call or after InitFromDomain call.
   */
  UniversalInstructionItem();

  /**
   * @brief The parameterized c-tor for UniversalInstructionItem.
   *
   * @param item_type The item type.
   *
   * It is expected, that item_type coincides with the name of the registered domain instruction.
   * Temporary domain instruction is created to initialize item properties.
   */
  explicit UniversalInstructionItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Set the domain type and performs attribute initialization.
   *
   * @details This function can be called only once.
   */
  void SetDomainType(const std::string& domain_type) override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;
  void SetDomainTypeImpl(const std::string& domain_type);

  struct Attribute
  {
    std::string domain_name;                //! domain attribute name
    sup::gui::AnyValueItem* item{nullptr};  //! item representing domain attribute
  };

  /**
   * @brief Returns items representing domain attributes.
   */
  std::vector<Attribute> GetAttributeItems() const;

  void SetupFromDomain(const instruction_t* instruction);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::UniversalInstructionItem>
{
  static constexpr std::string_view type_name() noexcept { return "UniversalInstruction"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
