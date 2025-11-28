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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_

#include <QWidget>

class QSplitter;
class QAction;

namespace sup::gui
{
class IAppCommandService;
class CustomSplitter;
}  // namespace sup::gui

namespace oac_tree_gui
{

class SequencerModel;
class ComposerToolsPanel;
class SequencerModel;
class ProcedureSplittableEditorWidget;
class SequencerComposerActions;

/**
 * @brief The SequencerComposerView class is a main widget for procedure editing.
 *
 * It is a second main view of sequencer main window. Contains 3 vertical panels:
 * - a narrow panel on the left with procedure selector and instruction toolbox
 * - a splittable area in the center with procedure editors
 * - optional extra sidebar on the right
 */
class SequencerComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerComposerView(sup::gui::IAppCommandService& command_service,
                                 QWidget* parent_widget = nullptr);
  ~SequencerComposerView() override;

  void SetModel(SequencerModel* model);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();
  void SetupWidgetActions();

  sup::gui::IAppCommandService& m_command_service;
  QAction* m_toggle_left_sidebar{nullptr};
  SequencerComposerActions* m_composer_actions{nullptr};

  sup::gui::CustomSplitter* m_splitter{nullptr};
  ComposerToolsPanel* m_composer_tools_panel{nullptr};
  ProcedureSplittableEditorWidget* m_splittable_editor_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
