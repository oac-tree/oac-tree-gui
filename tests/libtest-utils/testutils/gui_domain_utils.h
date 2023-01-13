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

#ifndef TEST_GUI_DOMAIN_UTILS_H
#define TEST_GUI_DOMAIN_UTILS_H

//! @file test_gui_domain_utils.h
//! Collection of utility functions to test gui/domain transformations.

#include <string>

namespace sup::dto
{
class AnyValue;
}

namespace sequencergui
{
class LocalVariableItem;
}

namespace testutils
{

//! Setup LocalVariableItem using given name and initial value.
void SetupVariable(const std::string& name, const sup::dto::AnyValue& initial_value,
                   sequencergui::LocalVariableItem& item);

}  // namespace testutils

#endif  // TEST_GUI_DOMAIN_UTILS_H
