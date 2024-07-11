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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_

#include <mvvm/signals/event_types.h>
#include <mvvm/signals/model_listener_fwd.h>

#include <QWidget>

namespace sup::gui
{
class CodeView;
class VisibilityAgentBase;
}  // namespace sup::gui

namespace sequencergui
{

class ProcedureItem;
class VisibilityAgentBase;

//! A simple read only text view to show XML content of the procedure.

class XmlPanel : public QWidget
{
  Q_OBJECT

public:
  explicit XmlPanel(QWidget* parent = nullptr);
  ~XmlPanel() override;

  void SetModel(mvvm::ISessionModel* model);

  void SetProcedure(ProcedureItem* procedure);

private:
  void OnModelEvent(const mvvm::DataChangedEvent& event);

  void UpdateXml();

  sup::gui::CodeView* m_xml_view{nullptr};
  mvvm::ISessionModel* m_model{nullptr};
  std::unique_ptr<mvvm::ModelListener<>> m_listener;
  ProcedureItem* m_procedure{nullptr};
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_
