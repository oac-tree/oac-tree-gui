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

#ifndef SEQUENCERGUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
#define SEQUENCERGUI_MODEL_STANDARD_VARIABLE_ITEMS_H_

//! Collection of classes to represent Sequencer variables.

#include <sequencergui/model/variable_item.h>

namespace sequencergui
{

//! Represent ChannelAccessVariable.
class ChannelAccessVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "ChannelAccessVariable";
  ChannelAccessVariableItem();

  std::string GetDomainType() const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);

  bool IsAvailable() const override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent FileVariable.
class FileVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "File";
  FileVariableItem();

  std::string GetDomainType() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& name);

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
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
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent PVClientVariable.
class PVClientVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "PVClientVariable";
  PVClientVariableItem();

  std::string GetDomainType() const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);

  bool IsAvailable() const override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent PVServerVariable.
class PVServerVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "PVServerVariable";
  PVServerVariableItem();

  std::string GetDomainType() const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);

  bool IsAvailable() const override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent unknown Variable.
class UnknownVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "UnknownVariable";
  UnknownVariableItem();

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;

  std::string m_domain_name;
  std::vector<std::string> m_domain_attributes;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
