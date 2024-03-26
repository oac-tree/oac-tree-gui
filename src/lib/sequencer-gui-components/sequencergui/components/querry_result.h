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

#ifndef SEQUENCERGUI_COMPONENTS_QUERRY_RESULT_H_
#define SEQUENCERGUI_COMPONENTS_QUERRY_RESULT_H_

#include <sup/gui/components/message_event.h>

namespace sequencergui
{

/**
 * @brief The QuerryResult class is a wrapper around MessageEvent to carry a flag denoting the
 * success of the operation.
 *
 * Unsuccessfull operation normally means that the user notification is necessary.
 */
class QuerryResult
{
public:
  enum class Status
  {
    kSuccess,
    kFailure
  };

  QuerryResult() = delete;

  /**
   * @brief Checks if querry result was successfull.
   */
  bool IsSuccess() const;

  /**
   * @brief A factory method to build an object carrying a failure flag, and the message.
   */
  static QuerryResult Failure(const sup::gui::MessageEvent& message);

  /**
   * @brief A factory method to build an object carrying a success flag.
   */
  static QuerryResult Success();

  /**
   * @brief Returns onboard message.
   */
  sup::gui::MessageEvent GetMessage() const;

private:
  QuerryResult(Status status, sup::gui::MessageEvent message);

  Status m_status{Status::kSuccess};
  sup::gui::MessageEvent m_message;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_QUERRY_RESULT_H_
