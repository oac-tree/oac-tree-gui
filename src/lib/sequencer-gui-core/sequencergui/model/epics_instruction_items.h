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

#ifndef SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
#define SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer instructions from sup-sequencer-plugin-epics.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/universal_instruction_item.h>

namespace sequencergui
{

//! Base for CA/PV read instructions.
class EpicsReadInstructionItem : public UniversalInstructionItem
{
public:
  using UniversalInstructionItem::UniversalInstructionItem;
  EpicsReadInstructionItem(const std::string& instruction_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

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
  using UniversalInstructionItem::UniversalInstructionItem;
  EpicsWriteInstructionItem(const std::string& instruction_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetVariableName() const;

  void SetVariableName(const std::string& value);

  std::string GetChannel() const;

  void SetChannel(const std::string& value);

  double GetTimeout() const;

  void SetTimeout(double value);

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);
};

//! Represents ChannelAccessReadInstruction.
class ChannelAccessReadInstructionItem : public EpicsReadInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kChannelAccessReadInstructionType;

  using EpicsReadInstructionItem::EpicsReadInstructionItem;
  ChannelAccessReadInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represents ChannelAccessWriteInstruction.
class ChannelAccessWriteInstructionItem : public EpicsWriteInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kChannelAccessWriteInstructionType;

  using EpicsWriteInstructionItem::EpicsWriteInstructionItem;
  ChannelAccessWriteInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represents PVAccessReadInstruction.
class PvAccessReadInstructionItem : public EpicsReadInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kPvAccessReadInstructionType;

  using EpicsReadInstructionItem::EpicsReadInstructionItem;
  PvAccessReadInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represents PVAccessWriteInstruction.
class PvAccessWriteInstructionItem : public EpicsWriteInstructionItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kPvAccessWriteInstructionType;

  using EpicsWriteInstructionItem::EpicsWriteInstructionItem;
  PvAccessWriteInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represents RPCClientInstruction.
class RPCClientInstruction : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kRPCClientInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  RPCClientInstruction();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetService() const;

  void SetService(const std::string& value);

  std::string GetRequestVar() const;

  void SetRequestVar(const std::string& value);

  double GetTimeout() const;

  void SetTimeout(double value);

  std::string GetJsonType() const;

  void SetJsonType(const std::string& value);

  std::string GetJsonValue() const;

  void SetJsonValue(const std::string& value);

  std::string GetOutput() const;

  void SetOutput(const std::string& value);
};

//! SystemCall instruction.
class SystemCallInstructionItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kSystemCallInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  SystemCallInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetCommand() const;

  void SetCommand(const std::string& value);
};

//! LogInstructionItem instruction.
class LogInstructionItem : public UniversalInstructionItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kLogInstructionType;

  using UniversalInstructionItem::UniversalInstructionItem;
  LogInstructionItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetMessage() const;

  void SetMessage(const std::string& value);

  std::string GetInput() const;

  void SetInput(const std::string& value);

  std::string GetSeverity() const;

  void SetSeverity(const std::string& value);
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_EPICS_INSTRUCTION_ITEMS_H_
