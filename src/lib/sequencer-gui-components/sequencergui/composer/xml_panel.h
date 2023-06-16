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

#ifndef SEQUENCERGUI_COMPOSER_XML_PANEL_H_
#define SEQUENCERGUI_COMPOSER_XML_PANEL_H_

#include <QWidget>
#include <mvvm/signals/event_types.h>

namespace mvvm
{
class SessionModelInterface;
class ModelHasChangedController;
template <typename T>
class ModelListener;
}  // namespace mvvm

namespace sup::gui
{
class CodeView;
}

namespace sequencergui
{

class ProcedureItem;

//! A simple read only text view to show XML content of the procedure.

class XmlPanel : public QWidget
{
  Q_OBJECT

public:
  using listener_t = mvvm::ModelListener<mvvm::SessionModelInterface>;

  explicit XmlPanel(mvvm::SessionModelInterface* model = nullptr, QWidget* parent = nullptr);
  ~XmlPanel() override;

  void SetModel(mvvm::SessionModelInterface* model);

  void SetProcedure(ProcedureItem* procedure);

private:
  void OnModelEvent(const mvvm::ItemRemovedEvent& event);
  void OnModelEvent(const mvvm::ItemInsertedEvent& event);
  void OnModelEvent(const mvvm::DataChangedEvent& event);

  void UpdateXml();

  sup::gui::CodeView* m_xml_view{nullptr};
  mvvm::SessionModelInterface* m_model{nullptr};
  std::unique_ptr<mvvm::ModelHasChangedController> m_model_changed_controller;
  std::unique_ptr<listener_t> m_listener;
  ProcedureItem* m_procedure{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPOSER_XML_PANEL_H_
