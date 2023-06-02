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

#ifndef LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_
#define LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_

//! Collection of classes to represent Sequencer equality instructions.

#include <sequencergui/domain/domain_constants.h>
#include <sequencergui/model/universal_instruction_item.h>

namespace testutils
{

//! Base instruction for all equality instructions.
class ComparisonItem : public sequencergui::UniversalInstructionItem
{
public:
  using UniversalInstructionItem::UniversalInstructionItem;
  explicit ComparisonItem(const std::string& item_type);

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetLeftHandSide() const;
  void SetLeftHandSide(const std::string& value);

  std::string GetRightHandSide() const;
  void SetRightHandSide(const std::string& value);
};

//! Represent LessThan instruction.
class LessThanItem : public ComparisonItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kLessThanInstructionType;

  using ComparisonItem::ComparisonItem;
  LessThanItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent LessThanOrEqual instruction.
class LessThanOrEqualItem : public ComparisonItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kLessThanOrEqualInstructionType;

  using ComparisonItem::ComparisonItem;
  LessThanOrEqualItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent Equals instruction.
class EqualsItem : public ComparisonItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kEqualsInstructionType;

  using ComparisonItem::ComparisonItem;
  EqualsItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent GreaterThanOrEqual instruction.
class GreaterThanOrEqualItem : public ComparisonItem
{
public:
  static inline const std::string Type =
      sequencergui::domainconstants::kGreaterThanOrEqualInstructionType;

  using ComparisonItem::ComparisonItem;
  GreaterThanOrEqualItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

//! Represent GreaterThan instruction.
class GreaterThanItem : public ComparisonItem
{
public:
  static inline const std::string Type = sequencergui::domainconstants::kGreaterThanInstructionType;

  using ComparisonItem::ComparisonItem;
  GreaterThanItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;
};

}  // namespace testutils

#endif  // LIBTEST_UTILS_TESTUTILS_EQUALITY_INSTRUCTION_ITEMS_H_
