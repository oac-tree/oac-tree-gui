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

#ifndef OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_ACTIONS_H_
#define OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_ACTIONS_H_

#include <sup/gui/components/action_map.h>

#include <QObject>
#include <functional>
#include <memory>

class QMenu;
class QAction;

namespace sup::gui
{
class ActionMenu;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The OperationJobActions class contains set of actions for OperationJobPanel.
 */
class OperationJobActions : public QObject
{
  Q_OBJECT

public:
  /**
   * @brief The ActionKey enum defines keys for all available actions.
   */
  enum class ActionKey : std::uint8_t
  {
    kImportProcedure,
    kSubmitJob,
    kRegenerateJob,
    kRemoveJob,
    kConnect,
    kTotalCount
  };

  using callback_t = std::function<std::vector<ProcedureItem*>()>;

  explicit OperationJobActions(QObject* parent_object = nullptr);
  ~OperationJobActions() override;

  OperationJobActions(const OperationJobActions&) = delete;
  OperationJobActions& operator=(const OperationJobActions&) = delete;
  OperationJobActions(OperationJobActions&&) = delete;
  OperationJobActions& operator=(OperationJobActions&&) = delete;

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& action_keys) const;

  void SetAvailableProcedures(callback_t available_procedures);

signals:
  void SubmitProcedureRequest(oac_tree_gui::ProcedureItem* item);
  void ImportJobRequest();
  void RegenerateJobRequest();
  void RemoveJobRequest();
  void ConnectRequest();

private:
  std::unique_ptr<QMenu> CreateSubmitProcedureMenu();
  void OnAboutToShowMenu();

  std::unique_ptr<QMenu> m_submit_procedure_menu;

  QAction* m_import_action{nullptr};
  sup::gui::ActionMenu* m_submit_action{nullptr};
  QAction* m_regenerate_action{nullptr};
  QAction* m_remove_action{nullptr};
  QAction* m_connect_to_remote_action{nullptr};

  callback_t m_available_procedures;

  sup::gui::ActionMap<ActionKey> m_action_map;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_OPERATION_OPERATION_JOB_ACTIONS_H_
