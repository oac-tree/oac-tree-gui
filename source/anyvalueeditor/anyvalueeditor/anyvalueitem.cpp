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

#include "anyvalueeditor/anyvalueitem.h"

namespace anyvalueeditor
{

static inline const std::string kChildren = "kChildren";

AnyValueItem::AnyValueItem() : CompoundItem(Type)
{
  RegisterTag(mvvm::TagInfo::CreateUniversalTag(kChildren), /*as_default*/ true);
}

//!
void AnyValueItem::SetFieldTypeName(const std::string &type_name)
{

}

}  // namespace anyvalueeditor
