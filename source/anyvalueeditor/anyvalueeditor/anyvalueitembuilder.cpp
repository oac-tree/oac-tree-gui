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

#include "anyvalueeditor/anyvalueitembuilder.h"

namespace anyvalueeditor
{

AnyValueItemBuilder::AnyValueItemBuilder(AnyValueItem *item) : m_item(item) {}

void AnyValueItemBuilder::AddEmptyProlog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddEmptyEpilog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddStructProlog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddStructMemberSeparator() {}

void AnyValueItemBuilder::AddStructEpilog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddMemberProlog(const anyvalue_t *anyvalue,
                                          const std::string &member_name)
{
}

void AnyValueItemBuilder::AddMemberEpilog(const anyvalue_t *anyvalue,
                                          const std::string &member_name)
{
}

void AnyValueItemBuilder::AddArrayProlog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddArrayElementSeparator() {}

void AnyValueItemBuilder::AddArrayEpilog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddScalarProlog(const anyvalue_t *anyvalue) {}

void AnyValueItemBuilder::AddScalarEpilog(const anyvalue_t *anyvalue) {}

}  // namespace anyvalueeditor

#include "anyvalueeditor/anyvalueitembuilder.h"
