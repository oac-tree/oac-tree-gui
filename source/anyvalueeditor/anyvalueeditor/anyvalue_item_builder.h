/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_BUILDER_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_BUILDER_H_

#include "sup/dto/basic_scalar_types.h"
#include "sup/dto/i_any_visitor.h"
#include "anyvalue_type.h"

namespace anyvalueeditor
{

class AnyValueItem;

class AnyValueItemBuilder : public sup::dto::IAnyVisitor<const sup::dto::AnyValue>
{
public:
  explicit AnyValueItemBuilder(AnyValueItem* item);

  void EmptyProlog(const anyvalue_t* anyvalue) override;
  void EmptyEpilog(const anyvalue_t* anyvalue) override;

  void StructProlog(const anyvalue_t* anyvalue) override;
  void StructMemberSeparator() override;
  void StructEpilog(const anyvalue_t* anyvalue) override;

  void MemberProlog(const anyvalue_t* anyvalue, const std::string& member_name) override;
  void MemberEpilog(const anyvalue_t* anyvalue, const std::string& member_name) override;

  void ArrayProlog(const anyvalue_t* anyvalue) override;
  void ArrayElementSeparator() override;
  void ArrayEpilog(const anyvalue_t* anyvalue) override;

  void ScalarProlog(const anyvalue_t* anyvalue) override;
  void ScalarEpilog(const anyvalue_t* anyvalue) override;

  void UnboundedArrayProlog(const anyvalue_t* val) override;
  void UnboundedArrayEpilog(const anyvalue_t* val) override;

private:
  AnyValueItem* m_item{nullptr};
  int m_index{0};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_BUILDER_H_
