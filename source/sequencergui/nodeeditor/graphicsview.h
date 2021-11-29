/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
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

#ifndef SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEW_H
#define SEQUENCERGUI_NODEEDITOR_GRAPHICSVIEW_H

#include <QGraphicsView>

class QKeyEvent;

namespace sequi
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
