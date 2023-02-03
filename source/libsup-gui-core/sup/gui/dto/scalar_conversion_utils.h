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

#ifndef GUI_DTO_SCALAR_CONVERSION_UTILS_H_
#define GUI_DTO_SCALAR_CONVERSION_UTILS_H_

//! Utility functions to convert scalar AnyValue to AnyValueItem and back.

#include <sup/gui/dto/dto_types_fwd.h>

namespace sup::gui
{

class AnyValueItem;

void SetDataFromScalar(const anyvalue_t& value, AnyValueItem& item);

//! Returns scalar AnyValue from AnyValueItem containing a scalar.
sup::dto::AnyValue GetAnyValueFromScalar(const AnyValueItem& item);

}  // namespace sup::gui

#endif  // GUI_DTO_SCALAR_CONVERSION_UTILS_H_
