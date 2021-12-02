/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
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

#include "mvvm/model/compounditem.h"

namespace sequi
{
//! Base class for all variable items.
class VariableItem : public ModelView::CompoundItem
{
public:
  VariableItem(const std::string& item_type);

  virtual std::unique_ptr<variable_t> CreateDomainVariable() const;

  //! Init given item from the domain variable.
  virtual void InitFromDomain(const variable_t* variable);

  std::string GetName() const;

  void SetName(const std::string& value);
};
}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_VARIABLEITEM_H
