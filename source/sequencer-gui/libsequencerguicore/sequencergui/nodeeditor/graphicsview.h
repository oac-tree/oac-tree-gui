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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEW_H
#define SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEW_H

#include <QGraphicsView>

class QKeyEvent;

namespace sequencergui
{
class GraphicsScene;

//! Graphics view for the graphics scene.

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  enum ESelectionModes
  {
    kSimpleSelection,
    kRubberSelection,
    kHandDrag,
    kUnknownSelection
  };

  GraphicsView(GraphicsScene* scene, QWidget* parent);

  void onCenterView();

  ESelectionModes GetSelectionModes();

  QSize sizeHint() const override;

  QSize minimumSizeHint() const override;

public slots:
    void onSelectionMode(int);
    void onChangeScale(double new_scale);

signals:
  void deleteSelectedRequest();
  void selectionModeChanged(int);

protected:
  void keyPressEvent(QKeyEvent* event);
  void keyReleaseEvent(QKeyEvent* event);
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEW_H
