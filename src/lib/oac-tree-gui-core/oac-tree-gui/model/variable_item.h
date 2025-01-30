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

#ifndef OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_
#define OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_

#include <oac-tree-gui/domain/sequencer_types_fwd.h>

#include <sup/gui/core/dto_types_fwd.h>

#include <mvvm/model/compound_item.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace oac_tree_gui
{

/**
 * @brief The VariableItem class is a base for all variable items.
 */
class VariableItem : public mvvm::CompoundItem
{
public:
  using CompoundItem::CompoundItem;
  explicit VariableItem(const std::string& item_type);

  /**
   * @brief Returns domain type.
   */
  virtual std::string GetDomainType() const = 0;

  /**
   * @brief Inits given item from the domain variable.
   *
   * @param variable The domain variable.
   * @param registry Optional type registry containing already existing domain types.
   */
  void InitFromDomain(const variable_t* variable, const anytype_registry_t* registry = nullptr);

  /**
   * @brief Creates domain variable corresponding to a given item.
   */
  std::unique_ptr<variable_t> CreateDomainVariable() const;

  /**
   * @brief Returns variable name.
   */
  std::string GetName() const;

  /**
   * @brief Sets the name of the variable.
   */
  void SetName(const std::string& value);

  /**
   * @brief Returns underlying AnyValueItem if exists, or nullptr if no item has been set.
   */
  sup::gui::AnyValueItem* GetAnyValueItem() const;

  /**
   * @brief Checks if variable is available.
   *
   * @details This method just returns the value of the corersponding flag. It is the job of
   * workspace syncronizer to update flag status on domain variable actual status change.
   */
  virtual bool IsAvailable() const;

  /**
   * @brief Sets availability flag to a given value.
   */
  virtual void SetIsAvailable(bool value);

protected:
  /**
   * @brief Provides tag registration for AnyValueItem insertion.
   *
   * @details The method is protected to allow derived classes to call it along with other tag
   * creation, and so manipulate the order of properties as they appear in editors.
   */
  void RegisterAnyValueItemTag();

private:
  /**
   * @brief Implementation to provide to init this VariableItem from the domain variable.
   */
  virtual void InitFromDomainImpl(const variable_t* variable,
                                  const anytype_registry_t* registry) = 0;

  /**
   * @brief Implementation to provide to setup domain variable from this VariableItem.
   */
  virtual void SetupDomainImpl(variable_t* variable) const = 0;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_VARIABLE_ITEM_H_
