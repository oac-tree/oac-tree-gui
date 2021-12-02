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

#ifndef SEQUENCERGUI_MODEL_STANDARDVARIABLEITEMS_H
#define SEQUENCERGUI_MODEL_STANDARDVARIABLEITEMS_H

//! @file standardvariableitems.h
//! Collection of classes to represent Sequencer variables.

#include "sequencergui/model/variableitem.h"

namespace sequi
{
//! Represent LocalVariable.
class LocalVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "Local";
  LocalVariableItem();

  std::unique_ptr<variable_t> CreateDomainVariable() const override;

  //! Init given item from the domain variable.
  void InitFromDomain(const variable_t* variable) override;

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);
};

//! Represent unknown Variable.
class UnknownVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "UnknownVariable";
  UnknownVariableItem();

  //! Init given item from the domain variable.
  void InitFromDomain(const variable_t* variable) override;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_STANDARDVARIABLEITEMS_H
