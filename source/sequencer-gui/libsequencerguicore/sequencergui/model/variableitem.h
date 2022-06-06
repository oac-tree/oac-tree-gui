/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_VARIABLEITEM_H
#define SEQUENCERGUI_MODEL_VARIABLEITEM_H

#include "sequencergui/model/sequencer_types.h"

#include "mvvm/model/compound_item.h"

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

private:
  virtual void InitFromDomainImpl(const variable_t* variable) = 0;
  virtual void SetupDomainImpl(variable_t* variable) const = 0;
};
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_VARIABLEITEM_H
