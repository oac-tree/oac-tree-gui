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

#ifndef ANYVALUEEDITOR_ANYVALUEITEMBUILDER_H
#define ANYVALUEEDITOR_ANYVALUEITEMBUILDER_H

#include "BasicScalarTypes.h"
#include "IAnySerializer.h"
#include "anyvalue_type.h"

namespace anyvalueeditor
{

class AnyValueItem;

class AnyValueItemBuilder : public sup::dto::IAnySerializer<sup::dto::AnyValue>
{
public:
  explicit AnyValueItemBuilder(AnyValueItem* item);

  void AddEmptyProlog(const anyvalue_t* anyvalue) override;
  void AddEmptyEpilog(const anyvalue_t* anyvalue) override;

  void AddStructProlog(const anyvalue_t* anyvalue) override;
  void AddStructMemberSeparator() override;
  void AddStructEpilog(const anyvalue_t* anyvalue) override;

  void AddMemberProlog(const anyvalue_t* anyvalue, const std::string& member_name) override;
  void AddMemberEpilog(const anyvalue_t* anyvalue, const std::string& member_name) override;

  void AddArrayProlog(const anyvalue_t* anyvalue) override;
  void AddArrayElementSeparator() override;
  void AddArrayEpilog(const anyvalue_t* anyvalue) override;

  void AddScalarProlog(const anyvalue_t* anyvalue) override;
  void AddScalarEpilog(const anyvalue_t* anyvalue) override;

private:
  AnyValueItem* m_item{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEITEMBUILDER_H
