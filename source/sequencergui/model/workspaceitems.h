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

#ifndef SEQUENCERGUI_MODEL_WORKSPACEITEMS_H
#define SEQUENCERGUI_MODEL_WORKSPACEITEMS_H

//! @file workspaceitems.h
//! Collection of classes to represent Sequencer workspace and variables.

#include "mvvm/model/compounditem.h"
#include "mvvm/model/sessionmodel.h"
#include "sequencergui/model/sequencer_types.h"

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

//! Represents a Workspace.

class WorkspaceItem : public ModelView::CompoundItem
{
public:
  static inline const std::string Type = "Workspace";
  WorkspaceItem();

  std::vector<VariableItem*> GetVariables() const;

};

}  // namespace sequi

#endif // SEQUENCERGUI_MODEL_WORKSPACEITEMS_H
