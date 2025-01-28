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

#ifndef SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_
#define SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_

#include <oac-tree-gui/domain/sequencer_types_fwd.h>
#include <oac-tree-gui/model/variable_item.h>

namespace sequencergui
{

/**
 * @brief The UniversalVariableItem class represents domain Variable.
 */

class UniversalVariableItem : public VariableItem
{
public:
  /**
   * @brief The parameterized c-tor for UniversalVariableItem.
   *
   * @param item_type The item type.
   *
   * If item_type coincides with the existing domain name, the resulting object will be initialised
   * with corresponding domain attributes.
   *
   * If item_type is empty, the resulting object can be used either after SetDomainType
   * call or after InitFromDomain call.
   */
  explicit UniversalVariableItem(const std::string& item_type = {});

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetDomainType() const override;

  /**
   * @brief Set the domain type and performs attribute initialization.
   *
   * @details This function can be called only once.
   */
  void SetDomainType(const std::string& domain_type);

private:
  struct Attribute
  {
    std::string domain_name;                //! domain attribute name
    sup::gui::AnyValueItem* item{nullptr};  //! item representing domain attribute
  };

  /**
   * @brief Get items representing domain attributes
   */
  std::vector<Attribute> GetAttributeItems() const;

  void InitFromDomainImpl(const variable_t* variable, const anytype_registry_t* registry) override;
  void SetupDomainImpl(variable_t* variable) const override;

  /**
   * @brief Provides initial setup of all attributes from the domain variable.
   * @param variable
   */
  void SetupFromDomain(const variable_t* variable);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_
