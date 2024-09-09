/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_SETTINGS_CONSTANTS_H_
#define SEQUENCERGUI_MODEL_SETTINGS_CONSTANTS_H_

//! @file
//! Collection of constants for persistent settings machinery.

#include <string>

namespace sequencergui
{

//! Setting names. These names are used to retrieve the value of the setting from the model and to
//! store settings in QSetting machinery.

const std::string kUseUndoSetting = "kUseUndoSetting";
const std::string kUndoLimitSetting = "kUndoLimitSetting";

//! Default values of some settings.

const bool kUseUndoDefault = true;
const int kUndoLimitDefault = 100;
}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_SETTINGS_ITEMS_H_
