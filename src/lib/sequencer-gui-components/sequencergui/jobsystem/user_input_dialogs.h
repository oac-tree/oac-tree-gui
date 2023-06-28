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

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_INPUT_DIALOGS_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_INPUT_DIALOGS_H_

//! Helper functions to create dialogs supporting UserConfirmation and UserChoices instructions.

#include <sequencergui/jobsystem/request_types.h>

namespace sequencergui
{

/**
 * @brief Summons user choice dialog appropriate for given input arguments and reports result to the
 * caller.
 */
UserChoiceResult GetUserChoiceDialogResult(const UserChoiceArgs &args);

/**
 * @brief Summons dialog for text selection and reports results to the caller.
 */
UserChoiceResult GetTextSelectionDialogResult(const UserChoiceArgs &args);

/**
 * @brief Summons confirmation dialog and reports results to the caller.
 */
UserChoiceResult GetConfirmationDialogResult(const UserChoiceArgs &args);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_INPUT_PROVIDER_H_
