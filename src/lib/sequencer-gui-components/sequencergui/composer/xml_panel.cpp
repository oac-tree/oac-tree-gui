
#include "xml_panel.h"

#include <sequencergui/model/xml_utils.h>
#include <sup/gui/codeeditor/code_view.h>

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/project/model_has_changed_controller.h>

#include <sup/dto/anyvalue.h>

#include <QVBoxLayout>

namespace sequencergui
{

XmlPanel::XmlPanel(mvvm::SessionModelInterface *model, QWidget *parent)
    : QWidget(parent), m_xml_view(new sup::gui::CodeView(sup::gui::CodeView::kXML))
{
  setWindowTitle("XML View");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_xml_view);

  SetModel(model);
}

void XmlPanel::SetModel(mvvm::SessionModelInterface *model)
{
  if (model == m_model)
  {
    return;
  }

  if (m_model)
  {
    m_model_changed_controller.reset();
  }

  m_model = model;

  if (!m_model)
  {
    return;
  }

  auto on_model_changed = [this]() { UpdateXml(); };
  m_model_changed_controller =
      std::make_unique<mvvm::ModelHasChangedController>(m_model, on_model_changed);
}

void XmlPanel::SetProcedure(ProcedureItem *procedure)
{
  m_procedure = procedure;
  UpdateXml();
}

XmlPanel::~XmlPanel() = default;

void XmlPanel::UpdateXml()
{
  if (m_procedure)
  {
    try
    {
      m_xml_view->SetContent(QString::fromStdString(ExportToXMLString(m_procedure)));
    }
    catch (const std::exception &ex)
    {
      // Procedure is in inconsistent state. For example, variable items all have the same names
      // which makes domain's Workspace unhappy.
      m_xml_view->ClearText();
    }
  }
}

}  // namespace sequencergui
