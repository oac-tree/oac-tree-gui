/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H
#define SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

class QListWidget;

namespace sequi
{

//! Collapsible list with types of workspace variables.

class WorkspacePanel : public CollapsibleWidget
{
  Q_OBJECT

public:
  WorkspacePanel(QWidget* parent = nullptr);

private:
  QListWidget* m_list_widget{nullptr};
};

}  // namespace sequi

#endif  // SEQUENCERGUI_COMPOSER_WORKSPACEPANEL_H
