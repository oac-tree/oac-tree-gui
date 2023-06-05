/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
#define SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_

#include <QWidget>
#include <memory>

class QSplitter;
class QShowEvent;

namespace sequencergui
{
class SequencerModel;
class ComposerWidgetPanel;
class ComposerPanel;
class ProcedureItem;
class SequencerComposerActions;

//! Main widget for model editing. Contains up to 3 vertical panels:
//! - a narrow panel on the left with procedure selector and instruction toolbox
//! - instruction tree view in the center
//! - workspace tree on the right
//! Belongs to MainWindow.

class SequencerComposerView : public QWidget
{
  Q_OBJECT

public:
  explicit SequencerComposerView(QWidget* parent = nullptr);
  ~SequencerComposerView() override;

  void SetModel(SequencerModel* model);

protected:
  void showEvent(QShowEvent* event) override;

private:
  void SetupConnections();

  ProcedureItem* GetFirstProcedure();

  ComposerPanel* m_composer_panel{nullptr};
  ComposerWidgetPanel* m_central_panel{nullptr};
  ComposerWidgetPanel* m_right_panel{nullptr};

  QSplitter* m_splitter{nullptr};
  SequencerModel* m_model{nullptr};

  SequencerComposerActions* m_composer_actions{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_SEQUENCER_COMPOSER_VIEW_H_
