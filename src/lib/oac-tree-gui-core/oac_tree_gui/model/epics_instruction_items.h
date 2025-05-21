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

#ifndef OAC_TREE_GUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
#define OAC_TREE_GUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent oac-tree instructions from sup-sequencer-plugin-epics.

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/universal_instruction_item.h>

namespace oac_tree_gui
{

//! Base for CA/PV read instructions.
class EpicsReadInstructionItem : public UniversalInstructionItem
{
public:
  explicit EpicsReadInstructionItem(const std::string& instruction_type);

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);

  double GetTimeout() const;

  void SetTimeout(double value);
};

//! Base for CA/PV read instructions..
class EpicsWriteInstructionItem : public UniversalInstructionItem
{
public:
  explicit EpicsWriteInstructionItem(const std::string& instruction_type);

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  double GetTimeout() const;

  void SetTimeout(double value);
};

//! Represents ChannelAccessReadInstruction.
class ChannelAccessReadInstructionItem : public EpicsReadInstructionItem
{
public:
  ChannelAccessReadInstructionItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represents ChannelAccessWriteInstruction.
class ChannelAccessWriteInstructionItem : public EpicsWriteInstructionItem
{
public:
  ChannelAccessWriteInstructionItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represents PVAccessReadInstruction.
class PvAccessReadInstructionItem : public EpicsReadInstructionItem
{
public:
  PvAccessReadInstructionItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represents PVAccessWriteInstruction.
class PvAccessWriteInstructionItem : public EpicsWriteInstructionItem
{
public:
  PvAccessWriteInstructionItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represents RPCClientInstruction.
class RPCClientInstruction : public UniversalInstructionItem
{
public:
  RPCClientInstruction();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetService() const;

  void SetService(const std::string& value);

  std::string GetRequestVar() const;

  void SetRequestVar(const std::string& value);

  double GetTimeout() const;

  void SetTimeout(double value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);
};

//! LogInstructionItem instruction.
class LogInstructionItem : public UniversalInstructionItem
{
public:
  LogInstructionItem();

  static std::string GetStaticType();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetMessage() const;

  void SetMessage(const std::string& value);

  std::string GetInput() const;

  void SetInput(const std::string& value);

  std::string GetSeverity() const;

  void SetSeverity(const std::string& value);
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
