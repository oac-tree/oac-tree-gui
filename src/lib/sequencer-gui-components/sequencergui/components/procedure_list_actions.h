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
#include <map>

class QAction;

namespace sequencergui
{

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
    kTotalCount
  };

  explicit ProcedureListActions(QObject* parent = nullptr);

  /**
   * @brief Returns list of actions according to provided flags.
   */
  QList<QAction*> GetActions(const std::vector<ActionKey>& actions);

signals:
  void CreateNewProcedureRequest();
  void RemoveProcedureRequest();

private:
  QAction* m_new_procedure_action{nullptr};
  QAction* m_remove_selected_action{nullptr};

  std::map<ActionKey, QAction*> m_actions;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_PROCEDURE_LIST_ACTIONS_H_
