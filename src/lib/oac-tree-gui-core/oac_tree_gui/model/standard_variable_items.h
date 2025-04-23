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
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
#define OAC_TREE_GUI_MODEL_STANDARD_VARIABLE_ITEMS_H_

//! Collection of classes to represent oac-tree variables.

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/universal_variable_item.h>
#include <oac_tree_gui/model/variable_item.h>

namespace oac_tree_gui
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
  ChannelAccessVariableItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent FileVariable.
class FileVariableItem : public oac_tree_gui::UniversalVariableItem
{
public:
  FileVariableItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& name);
};

//! Represent LocalVariable.
class LocalVariableItem : public oac_tree_gui::UniversalVariableItem
{
public:
  LocalVariableItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent PvAccessClientVariable.
class PvAccessClientVariableItem : public ConnectableVariableItem
{
public:
  PvAccessClientVariableItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent PvAccessServerVariable.
class PvAccessServerVariableItem : public ConnectableVariableItem
{
public:
  PvAccessServerVariableItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_STANDARD_VARIABLE_ITEMS_H_
