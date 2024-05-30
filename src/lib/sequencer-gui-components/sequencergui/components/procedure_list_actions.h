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

#ifndef SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTIONS_H_
#define SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTIONS_H_

#include <QObject>

#include <sequencergui/components/action_map.h>

class QMenu;

namespace sequencergui
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
  enum class ActionKey
  {
    kCreateNew,
    kRemoveSelected,
    kCut,
    kCopy,
    kPaste,
    kTotalCount
  };

  explicit ProcedureListActions(QObject* parent = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys);

  /**
   * @brief Setup cut/copy/paste actions in given menu.
   *
   * @param External, possibly empty, menu.
   * @param handler An action handler to retrieve status of enabled/disabled actions.
   */
  void SetupMenu(QMenu& menu, const ProcedureListActionHandler* handler);

signals:
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest();
  void CutRequest();
  void CopyRequest();
  void PasteRequest();

private:
  /**
   * @brief Registers actions that has to be available globally via shortcut, or main menubar.
   */
  void RegisterGlobalActions();

  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_selected_action{nullptr};
  QAction* m_cut_action{nullptr};
  QAction* m_copy_action{nullptr};
  QAction* m_paste_action{nullptr};

  ActionMap<ActionKey> m_action_map;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTIONS_H_
