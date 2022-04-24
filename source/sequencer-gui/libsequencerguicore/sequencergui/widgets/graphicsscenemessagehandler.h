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

#ifndef SEQUENCERGUI_WIDGETS_GRAPHICSSCENEMESSAGEHANDLER_H
#define SEQUENCERGUI_WIDGETS_GRAPHICSSCENEMESSAGEHANDLER_H

#include "sequencergui/core/messagehandlerinterface.h"

class QGraphicsScene;

namespace sequencergui
{
class GraphiscSceneMessageHandler : public MessageHandlerInterface
{
public:
  explicit GraphiscSceneMessageHandler(const QGraphicsScene* scene);

  void SendMessage(const std::string& text) override;

private:
  const QGraphicsScene* m_scene{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_GRAPHICSSCENEMESSAGEHANDLER_H
