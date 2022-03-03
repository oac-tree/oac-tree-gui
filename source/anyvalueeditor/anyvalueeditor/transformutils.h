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

#ifndef ANYVALUEEDITOR_TRANSFORMUTILS_H
#define ANYVALUEEDITOR_TRANSFORMUTILS_H

#include "anyvalue_type.h"

namespace anyvalueeditor
{

class AnyValueItem;

void ScalarToItem(const anyvalue_t& value, AnyValueItem& item);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_TRANSFORMUTILS_H
