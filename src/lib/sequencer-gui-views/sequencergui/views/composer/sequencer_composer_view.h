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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_

#include <QWidget>

namespace sup::gui
{
class CustomSplitter;
}

namespace sequencergui
{

class SequencerModel;
class ComposerWidgetPanel;
class ComposerPanel;
class ProcedureItem;
class SequencerComposerActions;
class ProcedurePluginController;

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
  explicit SequencerComposerView(QWidget* parent = nullptr);
  ~SequencerComposerView() override;

  void SetModel(SequencerModel* model);

private:
  void ReadSettings();
  void WriteSettings();
  void SetupConnections();

  ComposerPanel* m_composer_panel{nullptr};
  ComposerWidgetPanel* m_central_panel{nullptr};
  ComposerWidgetPanel* m_right_panel{nullptr};

  sup::gui::CustomSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};

  SequencerComposerActions* m_composer_actions{nullptr};
  std::unique_ptr<ProcedurePluginController> m_plugin_controller;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
