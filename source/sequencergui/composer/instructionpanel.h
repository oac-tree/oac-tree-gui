/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H
#define SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

namespace sequi
{
class InstructionListWidget;

//! Collapsible list with instructions available to drag and drop on graphics scene.

class InstructionPanel : public CollapsibleWidget
{
  Q_OBJECT

public:
  InstructionPanel(QWidget* parent = nullptr);

private:
  InstructionListWidget* m_list_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_INSTRUCTIONPANEL_H
