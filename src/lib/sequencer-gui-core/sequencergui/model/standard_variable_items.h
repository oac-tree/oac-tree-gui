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
#include <sequencergui/model/universal_variable_item.h>

namespace sequencergui
{

//! Represent EPICS variable.
class ConnectableVariableItem : public VariableItem
{
public:
  using VariableItem::VariableItem;
  explicit ConnectableVariableItem(const std::string& variable_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  bool IsAvailable() const override;

  void SetIsAvailable(bool value) override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
};

//! Represent ChannelAccessVariable.
class ChannelAccessVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = "ChannelAccessClient";

  using ConnectableVariableItem::ConnectableVariableItem;
  ChannelAccessVariableItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent FileVariable.
class FileVariableItem : public sequencergui::UniversalVariableItem
{
  public:
  static inline const std::string Type = "File";

  using sequencergui::UniversalVariableItem::UniversalVariableItem;
  FileVariableItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& name);
};

//! Represent LocalVariable.
class LocalVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "Local";

  using VariableItem::VariableItem;
  LocalVariableItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent PvAccessClientVariable.
class PvAccessClientVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = "PvAccessClient";

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  using ConnectableVariableItem::ConnectableVariableItem;
  PvAccessClientVariableItem();

  std::string GetDomainType() const override;

private:
  void SetupDomainImpl(variable_t* variable) const override;
};

//! Represent PvAccessServerVariable.
class PvAccessServerVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = "PvAccessServer";

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  using ConnectableVariableItem::ConnectableVariableItem;
  PvAccessServerVariableItem();

  std::string GetDomainType() const override;

private:
  void SetupDomainImpl(variable_t* variable) const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
