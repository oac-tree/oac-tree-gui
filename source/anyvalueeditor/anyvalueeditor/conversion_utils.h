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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_CONVERSION_UTILS_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_CONVERSION_UTILS_H_

#include <anyvalueeditor/dto_types_fwd.h>

#include <memory>

namespace anyvalueeditor
{

class AnyValueItem;

std::unique_ptr<AnyValueItem> CreateItem(const sup::dto::AnyValue& any_value);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_CONVERSION_UTILS_H_
