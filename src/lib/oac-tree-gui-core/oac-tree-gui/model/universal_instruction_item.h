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

#ifndef OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
#define OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_

//! Collection of classes to represent oac-tree instructions.

#include <oac-tree-gui/model/instruction_item.h>

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
   * @brief The parameterized c-tor for UniversalInstructionItem.
   *
   * @param item_type The item type.
   *
   * If item_type coincides with the existing domain name, the resulting object will be initialised
   * with corresponding domain attributes.
   *
   * If item_type is empty, the resulting object can be used either after SetDomainType
   * call or after InitFromDomain call.
   */
  explicit UniversalInstructionItem(const std::string& item_type = GetStaticType());

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  /**
   * @brief Set the domain type and performs attribute initialization.
   *
   * @details This function can be called only once.
   */
  void SetDomainType(const std::string& domain_type);

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const instruction_t* instruction) override;
  void SetupDomainImpl(instruction_t* instruction) const override;

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

#endif  // OAC_TREE_GUI_MODEL_UNIVERSAL_INSTRUCTION_ITEM_H_
