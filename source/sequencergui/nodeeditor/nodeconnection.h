/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODECONNECTION_H
#define SEQUENCERGUI_NODEEDITOR_NODECONNECTION_H

//! Node editor: original code is taken from
//! http://algoholic.eu/qnodeseditor-qt-nodesports-based-data-processing-flow-editor/
//! Copyright (c) 2012, STANISLAW ADASZEWSKI

#include <QGraphicsPathItem>

#include "sequencergui/mainwindow/appsettings.h"

namespace sequi
{
class NodePort;
class ConnectableView;

//! Elastic connection to connect ParentPort and ChildPort together.

class NodeConnection : public QGraphicsPathItem
{
public:
  NodeConnection(QGraphicsScene* scene);
  virtual ~NodeConnection();

  void setPos1(const QPointF& pos);
  void setPos2(const QPointF& pos);
  void setPort1(NodePort* port);
  void setPort2(NodePort* port);

  void updatePath();

  NodePort* port1() const;

  bool hasPort(const NodePort* port) const;

  void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

  ConnectableView* parentView() const;

  ConnectableView* childView() const;

private:
  QPointF m_pos1;
  QPointF m_pos2;
  NodePort* m_port1{nullptr};  //! The beginning of the connection.
  NodePort* m_port2{nullptr};  //! Connection end.
  AppSettings m_settings;
};

}  // namespace NodeEditor

#endif // SEQUENCERGUI_NODEEDITOR_NODECONNECTION_H
