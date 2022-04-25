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

#ifndef SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H
#define SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H

#include <QObject>

class QGraphicsView;

namespace sequencergui
{
class GraphicsViewMessageController : public QObject
{
  Q_OBJECT

public:
  explicit GraphicsViewMessageController(QGraphicsView* view);

  void AddMessage(const QString& text);

private:
  QGraphicsView* m_graphics_view{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_WIDGETS_GRAPHICSVIEWMESSAGECONTROLLER_H
