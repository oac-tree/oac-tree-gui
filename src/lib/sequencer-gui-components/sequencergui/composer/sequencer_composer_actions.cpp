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

#include "sequencer_composer_actions.h"

#include <QAction>
#include <QWidget>

namespace sequencergui
{

SequencerComposerActions::SequencerComposerActions(QWidget *parent) : QObject(parent)
{
  SetupActions();
}

QList<QAction*> SequencerComposerActions::GetMenuActions()
{
  return {m_validate_procedure_action, m_export_xml_action};
}

void SequencerComposerActions::SetupActions()
{
  m_validate_procedure_action = new QAction("Validate procedure", this);
  m_validate_procedure_action->setToolTip(
      "Performs validation if currently selected procedure\n"
      "can be executed");

  m_export_xml_action = new QAction("Export to XML", this);
  m_export_xml_action->setToolTip("Exports currently selected procedure to Sequencer XML file");
}

}  // namespace sequencergui
