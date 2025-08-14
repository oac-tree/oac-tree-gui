/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>

class QMenu;

namespace sup::gui
{
class AppCommandContext;
}

namespace oac_tree_gui
{

class ProcedureListActionHandler;

/**
 * @brief The ProcedureListActions class defines actions for toolbars and context menus of
 * ProcedureListWidget.
 */
class ProcedureListActions : public QObject
{
  Q_OBJECT

public:
  enum class ActionKey : std::uint8_t
  {
    kCreateNew,
    kRemoveSelected,
    kCut,
    kCopy,
    kPaste,
    kTotalCount
  };

  explicit ProcedureListActions(QObject* parent_object = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  /**
   * @brief Setup cut/copy/paste actions in given menu.
   *
   * @param External, possibly empty, menu.
   * @param handler An action handler to retrieve status of enabled/disabled actions.
   */
  void SetupMenu(QMenu& menu, const ProcedureListActionHandler* handler);

  /**
   * @brief Registers actions for given context.
   *
   * The context is normally provided by the parent widget.
   */
  void RegisterActionsForContext(const sup::gui::AppCommandContext &context);

signals:
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest();
  void CutRequest();
  void CopyRequest();
  void PasteRequest();

private:
  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_action{nullptr};
  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_action{nullptr};

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_PROCEDURE_LIST_ACTIONS_H_
