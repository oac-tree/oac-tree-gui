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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_TYPE_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_TYPE_H_

//! @file dto_types_fwd.h
//! Forward declaration of AnyValue related types.

namespace sup::dto
{
class AnyValue;
class AnyType;
enum class TypeCode;
}  // namespace sup::dto

using anyvalue_t = sup::dto::AnyValue;

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_TYPE_H_
