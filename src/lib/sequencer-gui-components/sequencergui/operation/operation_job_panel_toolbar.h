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

#ifndef SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_
#define SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_

#include <QToolBar>

namespace sequencergui
{

class OperationJobPanelToolBar : public QToolBar
{
  Q_OBJECT

public:
  explicit OperationJobPanelToolBar(QWidget* parent = nullptr);

signals:
  void ImportJobRequest();
  void RegenerateJobRequest();
  void RemoveJobRequest();
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_OPERATION_OPERATION_JOB_PANEL_TOOLBAR_H_
