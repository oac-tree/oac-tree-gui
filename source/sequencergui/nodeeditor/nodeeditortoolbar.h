/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H
#define SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H

#include <QToolBar>

class QToolBar;
class QButtonGroup;
class QToolButton;
class QMenu;

namespace sequi
{
//! Instrument tool bar on top of NodeEditor

class NodeEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  NodeEditorToolBar(QWidget* parent = nullptr);
  ~NodeEditorToolBar();

  void onViewSelectionMode(int mode);

signals:
  void selectionMode(int);
  void changeScale(double);
  void centerView();
  void alignSelectedRequest();

private:
  void AddDotsMenu();
  void InsertStrech();
  std::unique_ptr<QMenu> CreateZoomMenu();

  QButtonGroup* m_pointer_mode_group{nullptr};
  QToolButton* m_pointer_button{nullptr};
  QToolButton* m_pan_button{nullptr};
  QToolButton* m_center_button{nullptr};
  QToolButton* m_zoom_button{nullptr};
  QToolButton* m_align_button{nullptr};
  std::unique_ptr<QMenu> m_zoom_menu;
};

}  // namespace sequi

#endif  // SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H
