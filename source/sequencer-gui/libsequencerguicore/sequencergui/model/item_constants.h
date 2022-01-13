/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Creation, execution and monitoring of the Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2020 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_ITEM_CONSTANTS_H
#define SEQUENCERGUI_MODEL_ITEM_CONSTANTS_H

//! @file item_constants.h
//! Collection of constants for sequencer items.

#include <string>

namespace sequencergui::ItemConstants
{

//! Constants to register property items.
const std::string kName = "kName";
const std::string kStatus = "kStatus";
const std::string kDescription = "kDescription";
const std::string kXpos = "kXpos";
const std::string kYpos = "kYpos";
const std::string kChildInstructions = "kChildInstructions";
const std::string kIsRoot = "kIsRoot";

}  // namespace sequi::ItemConstants

#endif  // SEQUENCERGUI_MODEL_ITEM_CONSTANTS_H
