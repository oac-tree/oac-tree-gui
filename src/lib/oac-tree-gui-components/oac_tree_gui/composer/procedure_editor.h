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

#ifndef OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_
#define OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_

#include <QObject>
#include <memory>
#include <vector>

namespace mvvm
{
class SessionItem;
}

namespace oac_tree_gui
{

class ProcedureItem;
class IInstructionEditorActionHandler;
class InstructionEditorContext;
class InstructionItem;

/**
 * @brief The ProcedureEditor class is an envelope around all action handlers participating in
 * procedure editing.
 *
 * For the moment, it contains only logic to build an instruction tree via
 * InstructionEditorActionHandler and is used to handle double clicks in instruction tool box in the
 * same manner, as in other widgets.
 *
 * Other widgets (NodeEditor, InstructionTreeEditor) might contain their instances of
 * InstructionEditorActionHandler for instruction tree manipulation. Later on, they can be cleaned
 * up in the favor of this central class.
 */
class ProcedureEditor : public QObject
{
  Q_OBJECT

public:
  explicit ProcedureEditor(QObject* parent_object = nullptr);
  ~ProcedureEditor() override;

  void SetProcedure(ProcedureItem* procedure_item);

  /**
   * @brief Sets instructions which are currently selected in widgets with active focus.
   *
   * This method can be called from any widget (NodeGraphicsScene, InstructionEditorWidget instances
   * located in left and right panels). Thus, it reflects sort of the last user choice. This
   * information can be used by widgets, which have no own machinery for instruction selection.
   */
  void SetInstructionsInFocus(const std::vector<InstructionItem*>& instructions);

  /**
   * @brief returns selected instructions.
   */
  std::vector<InstructionItem*> GetInstructionInFocus();

  /**
   * @brief Insert instruction from toolbox to the instruction container.
   *
   * Where to insert will depend on the last selection.
   */
  void InsertInstructionFromToolBox(const QString& name);

signals:
  void SelectInstructionRequest(mvvm::SessionItem* item);

private:
  InstructionEditorContext CreateInstructionEditorContext();

  std::vector<InstructionItem*> GetSelectedInstructions();

  ProcedureItem* m_current_procedure{nullptr};
  std::unique_ptr<IInstructionEditorActionHandler> m_action_handler;
  std::vector<InstructionItem*> m_selected_instructions;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_COMPOSER_PROCEDURE_EDITOR_H_
