/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/composer/instructionpanel.h"

#include "sequencergui/nodeeditor/instructionlistwidget.h"

#include <QVBoxLayout>

namespace sequi
{


InstructionPanel::InstructionPanel(QWidget *parent)
    : CollapsibleWidget(parent), m_list_widget(new InstructionListWidget)
{
  SetText("INSTRUCTIONS");

  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setMargin(0);
  layout->setSpacing(0);
  layout->addWidget(m_list_widget);


}

}  // namespace sequi
