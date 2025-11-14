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

#ifndef OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_V2_H_
#define OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_V2_H_

#include <QWidget>

class QSplitter;

namespace sup::gui
{
class IAppCommandService;
}  // namespace sup::gui

namespace oac_tree_gui
{

class SplittableWidget;

/**
 * @brief The SequencerComposerViewV2 class is a main widget for procedure editing.
 *
 * It is a second main view of sequencer main window. Contains 3 vertical panels:
 * - a narrow panel on the left with procedure selector and instruction toolbox
 * - a splittable area in the center with procedure editors
 * - optional extra sidebar on the right
 */

class SequencerComposerViewV2 : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerComposerViewV2(sup::gui::IAppCommandService& command_service,
                                   QWidget* parent_widget = nullptr);
  ~SequencerComposerViewV2() override;

private:
  /**
   * @brief Creates a new procedure editor widget.
   */
  std::unique_ptr<QWidget> CreateProcedureEditor();

  /**
   * @brief Returns a callback to create procedure editor widgets.
   */
  std::function<std::unique_ptr<QWidget>()> CreateProcedureEditorCallback();

  QSplitter* m_splitter{nullptr};
  SplittableWidget* m_procedure_editor_area_widget{nullptr};
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_V2_H_
