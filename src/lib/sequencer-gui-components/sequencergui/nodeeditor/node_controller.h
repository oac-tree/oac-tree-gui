/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODE_CONTROLLER_H_
#define SEQUENCERGUI_NODEEDITOR_NODE_CONTROLLER_H_

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QObject>

class QGraphicsScene;
class QGraphicsSceneMouseEvent;

namespace sequencergui
{
class NodeConnection;
class NodePort;
class ConnectableView;

//! The main controller of NodeEditor machinery. Listens to mouse events of the graphics scene,
//! updates connections between ports.

class NodeController : public QObject
{
  Q_OBJECT

public:
  explicit NodeController(QGraphicsScene* scene);

  bool eventFilter(QObject* object, QEvent* event) override;

  NodePort* findPort(const QPointF& pos);

signals:
  void connectionRequest(sequencergui::ConnectableView* childView,
                         sequencergui::ConnectableView* parentView);
  void selectionModeChangeRequest(int);

private:
  bool processMousePress(QGraphicsSceneMouseEvent* event);
  bool processMouseMove(QGraphicsSceneMouseEvent* event);
  bool processMouseRelease(QGraphicsSceneMouseEvent* event);
  void resetConnection();

  QGraphicsScene* m_scene{nullptr};
  NodeConnection* m_conn{nullptr};  //! Currently processed connection.
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODE_CONTROLLER_H_
