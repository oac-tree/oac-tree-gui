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

namespace sup::dto
{
class AnyValue;
}

namespace anyvalueeditor
{

class AnyValueItem;

class AnyValueItemBuilder : public sup::dto::IAnySerializer<sup::dto::AnyValue>
{
public:
  explicit AnyValueItemBuilder(AnyValueItem* item);

private:
  AnyValueItem* m_item{nullptr};
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEITEMBUILDER_H
