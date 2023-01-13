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

#ifndef GUI_DTO_DTO_TYPES_FWD_H_
#define GUI_DTO_DTO_TYPES_FWD_H_

//! @file dto_types_fwd.h
//! Forward declaration of AnyValue related types.

namespace sup::dto
{
class AnyValue;
class AnyType;
enum class TypeCode;
}  // namespace sup::dto

using anyvalue_t = sup::dto::AnyValue;

#endif  // GUI_DTO_DTO_TYPES_FWD_H_
