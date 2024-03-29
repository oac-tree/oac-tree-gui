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

#ifndef SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_ACTIONS_H_
#define SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_ACTIONS_H_

#include <QObject>
#include <memory>

class QToolBar;
class QButtonGroup;
class QToolButton;
class QMenu;
class QAction;
class QWidgetAction;

namespace sup::gui
{
class ActionMenu;
}

namespace sequencergui
{

//! Instrument tool bar on top of NodeEditor

class NodeEditorActions : public QObject
{
  Q_OBJECT

public:
  enum class ActionKey
  {
    kPointer,
    kPan,
    kZoom,
    kCenter,
    kAlign
  };

  explicit NodeEditorActions(QWidget* parent = nullptr);
  ~NodeEditorActions() override;

  void onViewSelectionMode(int mode);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys);

  /**
   * @brief Returns action for given key.
   */
  QAction* GetAction(ActionKey key) const;

signals:
  void selectionMode(int);
  void changeScale(double);
  void centerView();
  void alignSelectedRequest();

private:
  std::unique_ptr<QMenu> CreateZoomMenu();

  QButtonGroup* m_pointer_mode_group{nullptr};
  QToolButton* m_pointer_button{nullptr};
  QWidgetAction* m_pointer_action{nullptr};
  QToolButton* m_pan_button{nullptr};
  QWidgetAction* m_pan_action{nullptr};
  QAction* m_center_action{nullptr};
  sup::gui::ActionMenu* m_zoom_action{nullptr};
  QAction* m_align_action{nullptr};

  std::unique_ptr<QMenu> m_zoom_menu;

  std::map<ActionKey, QAction*> m_action_map;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODE_EDITOR_ACTIONS_H_
