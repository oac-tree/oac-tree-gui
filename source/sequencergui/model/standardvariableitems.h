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
//! Represent unknown Variable.
class FileVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "FileVariable";
  FileVariableItem();

  std::string GetDomainType() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& name);

private:
  virtual void InitFromDomainImpl(const variable_t* variable) override;
  virtual void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent LocalVariable.
class LocalVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "Local";
  LocalVariableItem();

  std::string GetDomainType() const override;

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);

private:
  virtual void InitFromDomainImpl(const variable_t* variable) override;
  virtual void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent unknown Variable.
class UnknownVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "UnknownVariable";
  UnknownVariableItem();

  std::string GetDomainType() const override;

private:
  virtual void InitFromDomainImpl(const variable_t* variable) override;
  virtual void SetupDomainImpl(variable_t* variable) const override;

  std::string m_domain_name;
  std::vector<std::string> m_domain_attributes;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_MODEL_STANDARDVARIABLEITEMS_H
