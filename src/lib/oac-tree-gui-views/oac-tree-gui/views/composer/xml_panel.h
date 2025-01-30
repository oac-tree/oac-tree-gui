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

#ifndef SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_
#define SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_

#include <mvvm/signals/event_types.h>

#include <QWidget>
class QAction;

namespace mvvm
{
class ModelListener;
}

namespace sup::gui
{
class CodeView;
class VisibilityAgentBase;
class MessageHandlerInterface;
}  // namespace sup::gui

namespace oac_tree_gui
{

class ProcedureItem;

/**
 * @brief The XmlPanel class represent a simple read-only text view to show XML content of the
 * procedure.
 */
class XmlPanel : public QWidget
{
  Q_OBJECT

public:
  explicit XmlPanel(QWidget* parent_widget = nullptr);
  ~XmlPanel() override;

  void SetModel(mvvm::ISessionModel* model);

  void SetProcedure(ProcedureItem* procedure);

signals:
  void ExportToFileRequest();

private:
  void SetupActions();

  /**
   * @brief Sets up listener to regenerate XML on model change.
   *
   * It also takes care, that XML is not regenerated, when widget is hidden.
   */
  void SetupListener();

  void OnDataChangedEvent(const mvvm::DataChangedEvent& event);

  /**
   * @brief Regenerates XML in text viewer.
   */
  void UpdateXml();

  /**
   * @brief Notifies the user that XML generation went wrong.
   */
  void SendMessage(const std::string& what) const;

  QAction* m_export_action{nullptr};

  sup::gui::CodeView* m_xml_view{nullptr};
  mvvm::ISessionModel* m_model{nullptr};
  std::unique_ptr<mvvm::ModelListener> m_listener;
  ProcedureItem* m_procedure{nullptr};
  sup::gui::VisibilityAgentBase* m_visibility_agent{nullptr};
   std::unique_ptr<sup::gui::MessageHandlerInterface> m_message_handler;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_VIEWS_COMPOSER_XML_PANEL_H_
