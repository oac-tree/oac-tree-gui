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

#ifndef ANYVALUEEDITOR_ANYVALUEUTILS_H
#define ANYVALUEEDITOR_ANYVALUEUTILS_H

#include "mvvm/core/variant.h"

#include <vector>
#include <string>

namespace anyvalueeditor
{

struct FieldContext
{
  std::string name;     //! Name of the field
  std::string type;     //! Major type (scalar, array, struct)
  std::string subtype;  //! Scalae type
};

std::vector<std::string> GetScalarTypeNames();

//! Returns variant carrying defaut constructed type corresponding to AnyValue type name.
//!
variant_t GetVariantForAnyValueTypeName(const std::string& type_name);

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEUTILS_H
