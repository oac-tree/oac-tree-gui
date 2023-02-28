/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_VARIABLE_ITEM_H_
#define SEQUENCERGUI_MODEL_VARIABLE_ITEM_H_

#include <sequencergui/domain/sequencer_types_fwd.h>

#include <mvvm/model/compound_item.h>

namespace sup::gui
{
class AnyValueItem;
}

namespace sequencergui
{
//! Base class for all variable items.
class VariableItem : public mvvm::CompoundItem
{
public:
  explicit VariableItem(const std::string& item_type);

  virtual std::string GetDomainType() const = 0;

  //! Init given item from the domain variable.
  void InitFromDomain(const variable_t* variable);

  //! Creates domain variable corresponding to given item.
  std::unique_ptr<variable_t> CreateDomainVariable() const;

  std::string GetName() const;

  void SetName(const std::string& value);

  sup::gui::AnyValueItem* GetAnyValueItem() const;

  virtual bool IsAvailable() const;
  virtual void SetIsAvailable(bool value);

protected:
  void RegisterAnyValueItemTag();

private:
  virtual void InitFromDomainImpl(const variable_t* variable) = 0;
  virtual void SetupDomainImpl(variable_t* variable) const = 0;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_VARIABLE_ITEM_H_
