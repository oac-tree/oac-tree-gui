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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_

#include <QWidget>

#include <memory>

namespace sup::gui
{
class CustomSplitter;
class AppContext;
}  // namespace sup::gui

namespace oac_tree_gui
{

class SequencerModel;
class ComposerWidgetPanel;
class ComposerPanel;
class ProcedureItem;
class SequencerComposerActions;
class ProcedurePluginController;
class ProcedureEditor;

/**
 * @brief The SequencerComposerView class is a main widget for model editing.
 *
 * It is a second main view of sequencer main window. Contains 3 vertical panels:
 * - a narrow panel on the left with procedure selector and instruction toolbox
 * - instruction tree view in the center
 * - workspace tree on the right
 */
class SequencerComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerComposerView(QWidget* parent_widget = nullptr);
  ~SequencerComposerView() override;

  void SetModel(SequencerModel* model);

  /**
   * @brief Registers actions for given context.
   */
  void RegisterActionsForContext(const sup::gui::AppContext& context);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void SetupWidgetActions();

  QAction* m_toggle_left_sidebar{nullptr};
  QAction* m_toggle_right_sidebar{nullptr};

  std::unique_ptr<ProcedureEditor> m_procedure_editor;

  ComposerPanel* m_composer_panel{nullptr};
  ComposerWidgetPanel* m_central_panel{nullptr};
  ComposerWidgetPanel* m_right_panel{nullptr};

  sup::gui::CustomSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};

  SequencerComposerActions* m_composer_actions{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
