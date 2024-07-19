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

#ifndef SEQUENCERGUI_JOBSYSTEM_USER_INPUT_DIALOGS_H_
#define SEQUENCERGUI_JOBSYSTEM_USER_INPUT_DIALOGS_H_

//! Helper functions to create dialogs supporting UserConfirmation and UserChoices instructions.

#include <sequencergui/jobsystem/request_types.h>

class QWidget;

namespace sequencergui
{

/**
 * @brief Summons user choice dialog appropriate for given input arguments and reports result to the
 * caller.
 *
 * @param args Input arguments to form the question.
 * @param parent Parent widget to center given dialog.
 *
 * @return Structure representing results of user choice.
 */
UserChoiceResult GetUserChoiceDialogResult(const UserChoiceArgs& args, QWidget* parent);

/**
 * @brief Summons dialog for text selection and reports results to the caller.
 *
 * @param args Input arguments to form the question.
 * @param parent Parent widget to center given dialog.
 *
 * @return Structure representing results of user choice.
 */
UserChoiceResult GetTextSelectionDialogResult(const UserChoiceArgs& args, QWidget* parent);

/**
 * @brief Summons confirmation dialog and reports results to the caller.
 *
 * @param args Input arguments to form the question.
 * @param parent Parent widget to center given dialog.
 *
 * @return Structure representing results of user choice.
 */
UserChoiceResult GetConfirmationDialogResult(const UserChoiceArgs& args, QWidget* parent);

/**
 * @brief Summons AnyValueEditorDialog and reports result to the caller.
 *
 * @param args Input arguments to form the question.
 * @param parent Parent widget to center given dialog.
 *
 * @return Structure representing results of user input.
 */
UserInputResult GetAnyValueEditorDialogResult(const UserInputArgs& args, QWidget* parent);

}  // namespace sequencergui

#endif  // SEQUENCERGUI_JOBSYSTEM_USER_INPUT_DIALOGS_H_
