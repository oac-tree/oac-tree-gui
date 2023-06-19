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

#ifndef SEQUENCERGUI_COMPONENTS_VISIBILITY_AGENT_BASE_H_
#define SEQUENCERGUI_COMPONENTS_VISIBILITY_AGENT_BASE_H_

#include <QObject>
#include <functional>

namespace sequencergui
{

/**
 * @brief The VisibilityAgentBase class calls two different callbacks depending on whether the
 * parent widget is visible or not.
 *
 * @details It is used to unsubscribe from model notifications when widget is hidden.
 */

class VisibilityAgentBase : public QObject
{
  Q_OBJECT

public:
  using callback_t = std::function<void()>;

  VisibilityAgentBase(QObject* parent, callback_t subscribe, callback_t unsubscribe);

protected:
  bool eventFilter(QObject* obj, QEvent* event) override;

private:
  callback_t m_subscribe_callback;
  callback_t m_unsubscribe_callback;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_VISIBILITY_AGENT_BASE_H_
