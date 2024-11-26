/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
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

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/universal_variable_item.h>
#include <sequencergui/model/variable_item.h>

namespace sequencergui
{

//! Represent EPICS variable.
class ConnectableVariableItem : public UniversalVariableItem
{
public:
  explicit ConnectableVariableItem(const std::string& variable_type);

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  bool IsAvailable() const override;

  void SetIsAvailable(bool value) override;
};

//! Represent ChannelAccessVariable.
class ChannelAccessVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kChannelAccessVariableType;
  ChannelAccessVariableItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent FileVariable.
class FileVariableItem : public sequencergui::UniversalVariableItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kFileVariableType;
  FileVariableItem();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& name);
};

//! Represent LocalVariable.
class LocalVariableItem : public sequencergui::UniversalVariableItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kLocalVariableType;
  LocalVariableItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent PvAccessClientVariable.
class PvAccessClientVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kPvAccessClientVariableType;
  PvAccessClientVariableItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent PvAccessServerVariable.
class PvAccessServerVariableItem : public ConnectableVariableItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kPvAccessServerVariableType;

  PvAccessServerVariableItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
