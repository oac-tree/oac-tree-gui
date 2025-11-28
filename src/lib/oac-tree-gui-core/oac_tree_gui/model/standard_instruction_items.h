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

#ifndef OAC_TREE_GUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_
#define OAC_TREE_GUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent oac-tree instructions.

#include <oac_tree_gui/domain/domain_constants.h>
#include <oac_tree_gui/model/universal_instruction_item.h>

namespace oac_tree_gui
{

//! Represent IncludeItem instruction.
class IncludeItem : public UniversalInstructionItem
{
public:
  IncludeItem();

  std::unique_ptr<SessionItem> Clone() const override;

  std::string GetFileName() const;

  void SetFileName(const std::string& value);

  std::string GetPath() const;

  void SetPath(const std::string& value);
};

//! Represent ParallelSequence instruction.
class ParallelSequenceItem : public UniversalInstructionItem
{
public:
  ParallelSequenceItem();

  std::unique_ptr<SessionItem> Clone() const override;

  mvvm::uint32 GetSuccessThreshold() const;

  void SetSuccessThreshold(mvvm::uint32 value);

  mvvm::uint32 GetFailureThreshold() const;

  void SetFailureThreshold(mvvm::uint32 value);
};

//! Represent Repeat instruction.
class RepeatItem : public UniversalInstructionItem
{
public:
  RepeatItem();

  std::unique_ptr<SessionItem> Clone() const override;

  mvvm::int32 GetRepeatCount() const;

  void SetRepeatCount(mvvm::int32 value);
};

//! Represent oac-tree instruction.
class SequenceItem : public UniversalInstructionItem
{
public:
  SequenceItem();

  std::unique_ptr<SessionItem> Clone() const override;
};

//! Represent Wait instruction.
class WaitItem : public UniversalInstructionItem
{
public:
  WaitItem();

  std::unique_ptr<SessionItem> Clone() const override;

  void SetTimeout(double value);

  double GetTimeout() const;

  bool IsBlocking() const;

  void SetBlocking(bool value);
};

}  // namespace oac_tree_gui

namespace mvvm
{

template <>
struct item_traits<oac_tree_gui::IncludeItem>
{
  static constexpr std::string_view type_name() noexcept { return "Include"; }
};

template <>
struct item_traits<oac_tree_gui::ParallelSequenceItem>
{
  static constexpr std::string_view type_name() noexcept { return "ParallelSequence"; }
};

template <>
struct item_traits<oac_tree_gui::RepeatItem>
{
  static constexpr std::string_view type_name() noexcept { return "Repeat"; }
};

template <>
struct item_traits<oac_tree_gui::SequenceItem>
{
  static constexpr std::string_view type_name() noexcept { return "Sequence"; }
};

template <>
struct item_traits<oac_tree_gui::WaitItem>
{
  static constexpr std::string_view type_name() noexcept { return "Wait"; }
};

}  // namespace mvvm

#endif  // OAC_TREE_GUI_MODEL_STANDARD_INSTRUCTION_ITEMS_H_
