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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_OAC_TREE_COMPOSER_ACTIONS_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_OAC_TREE_COMPOSER_ACTIONS_H_

#include <QObject>

class QAction;
class QWidget;

namespace sup::gui
{
class AppContext;
}

namespace sequencergui
{

class ProcedureItem;
class SequencerModel;

/**
 * @brief The SequencerComposerActions class is container for several procedure-related actions.
 */
class SequencerComposerActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerComposerActions(QWidget* parent_widget = nullptr);
  ~SequencerComposerActions() override;

  void SetModel(SequencerModel* model);

  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Registers actions for given context.
   *
   * The context is normally provided by the parent widget.
   */
  void RegisterActionsForContext(const sup::gui::AppContext& context);

private:
  void SetupActions();

  QAction* m_validate_procedure_action{nullptr};
  QAction* m_export_xml_action{nullptr};
  QAction* m_undo_action{nullptr};
  QAction* m_redo_action{nullptr};

  ProcedureItem* m_procedure_item{nullptr};
  SequencerModel* m_model{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_OAC_TREE_COMPOSER_ACTIONS_H_
