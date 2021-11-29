/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#ifndef MVVM_MONITOR_MESSAGEPANEL_H
#define MVVM_MONITOR_MESSAGEPANEL_H

#include "sequencergui/mainwindow/collapsiblewidget.h"

class QTextEdit;

namespace sequi
{

class MessagePanel : public CollapsibleWidget
{
public:
  explicit MessagePanel(QWidget* parent = nullptr);

public slots:
    void onClearLog();
    void onMessage(const QString& message, const QColor& color = QColor(Qt::black));

private:
  QTextEdit* m_text_edit{nullptr};
};

}  // namespace sequi

#endif  // MVVM_MONITOR_MESSAGEPANEL_H
