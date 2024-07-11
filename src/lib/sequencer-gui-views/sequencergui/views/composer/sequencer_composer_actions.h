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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_ACTIONS_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_ACTIONS_H_

#include <QObject>

class QAction;
class QWidget;

namespace sup::gui{
class AppContext;
}

namespace sequencergui
{

class ProcedureItem;

//! Container for several procedure-related actions.

class SequencerComposerActions : public QObject
{
  Q_OBJECT

public:
  explicit SequencerComposerActions(QWidget* parent = nullptr);
  ~SequencerComposerActions() override;

  QList<QAction*> GetMenuActions();

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

  ProcedureItem* m_procedure_item{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_ACTIONS_H_
