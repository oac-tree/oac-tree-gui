/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
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
