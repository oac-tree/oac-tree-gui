/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H
#define SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H

#include <QToolBar>

class QToolBar;
class QButtonGroup;
class QToolButton;
class QMenu;

namespace sequencergui
{
//! Instrument tool bar on top of NodeEditor

class NodeEditorToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit NodeEditorToolBar(QWidget* parent = nullptr);
  ~NodeEditorToolBar() override;

  void onViewSelectionMode(int mode);

signals:
  void selectionMode(int);
  void changeScale(double);
  void centerView();
  void alignSelectedRequest();
  void alignSelectedRequestV2();

private:
  void AddDotsMenu();
  void InsertStrech();
  std::unique_ptr<QMenu> CreateZoomMenu();

  QButtonGroup* m_pointer_mode_group{nullptr};
  QToolButton* m_pointer_button{nullptr};
  QToolButton* m_pan_button{nullptr};
  QToolButton* m_center_button{nullptr};
  QToolButton* m_zoom_button{nullptr};
  QToolButton* m_align_button{nullptr};
  QToolButton* m_align_button_v2{nullptr};
  std::unique_ptr<QMenu> m_zoom_menu;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_NODEEDITOR_NODEEDITORTOOLBAR_H
