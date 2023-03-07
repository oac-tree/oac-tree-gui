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

#include "anyvalue_editor_textpanel.h"

#include "highlighter/qsourcehighliter.h"

#include <mvvm/model/application_model.h>
#include <mvvm/model/model_utils.h>
#include <mvvm/project/model_has_changed_controller.h>

#include <sup/dto/anyvalue.h>
#include <sup/gui/model/anyvalue_conversion_utils.h>
#include <sup/gui/model/anyvalue_item.h>
#include <sup/gui/model/anyvalue_utils.h>

#include <QTextEdit>
#include <QVBoxLayout>

namespace sup::gui
{

AnyValueEditorTextPanel::AnyValueEditorTextPanel(mvvm::ApplicationModel *model, QWidget *parent)
    : QWidget(parent), m_text_edit(new QTextEdit), m_model(model)
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(0);

  layout->addWidget(m_text_edit);

  QFont textFont("Monospace");
  m_text_edit->setFont(textFont);
  m_text_edit->setLineWrapMode(QTextEdit::NoWrap);

  auto highlighter = new QSourceHighlite::QSourceHighliter(m_text_edit->document());
  highlighter->setCurrentLanguage(QSourceHighlite::QSourceHighliter::CodeJSON);

  auto on_model_changed = [this]() { UpdateJson(); };
  m_model_changed_controller =
      std::make_unique<mvvm::ModelHasChangedController>(m_model, on_model_changed);
}

AnyValueEditorTextPanel::~AnyValueEditorTextPanel() = default;

void AnyValueEditorTextPanel::UpdateJson()
{
  if (auto item = mvvm::utils::GetTopItem<sup::gui::AnyValueItem>(m_model); item)
  {
    try
    {
      auto any_value = sup::gui::CreateAnyValue(*item);
      auto str = sup::gui::GetAnyValueToJSONString(&any_value, true);
      m_text_edit->setText(QString::fromStdString(str));
    }
    catch (const std::exception &ex)
    {
      // Current simplified approach calls the method `UpdateJson` on every
      // model change. If model is inconsistent, CreateAnyValue method will fail.
      m_text_edit->clear();
    }
  }
  else
  {
    m_text_edit->clear();
  }
}

}  // namespace sup::gui
