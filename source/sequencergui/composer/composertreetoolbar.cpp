/******************************************************************************
 *
 * Project       : Operational Applications UI Foundation
 *
 * Description   : The model-view-viewmodel library of generic UI components
 *
 * Author        : Gennady Pospelov <gennady.pospelov@gmail.com>
 *
 *****************************************************************************/

#include "sequencergui/composer/composertreetoolbar.h"

#include "sequencergui/mainwindow/styleutils.h"
#include "sequencergui/model/domainutils.h"

#include "mvvm/widgets/widgetutils.h"

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

namespace sequi
{
ComposerTreeToolBar::ComposerTreeToolBar(QWidget *parent)
    : QToolBar(parent)
    , m_insert_after_button(new QToolButton)
    , m_insert_into_button(new QToolButton)
    , m_remove_button(new QToolButton)
    , m_insert_into_menu(std::make_unique<QMenu>())
    , m_insert_after_menu(std::make_unique<QMenu>())
{
  setIconSize(StyleUtils::ToolBarIconSize());

  SetupMenu();

  m_insert_after_button->setText("After");
  m_insert_after_button->setIcon(QIcon(":/icons/plus-circle-outline.svg"));
  m_insert_after_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_insert_after_button->setPopupMode(QToolButton::InstantPopup);
  m_insert_after_button->setMenu(m_insert_after_menu.get());
  m_insert_after_button->setToolTip(
      "Insert instruction after current selection,\nor simply add to the list if nothing is "
      "selected");
  addWidget(m_insert_after_button);

  m_insert_into_button->setText("Into");
  m_insert_into_button->setIcon(QIcon(":/icons/plus-circle-outline.svg"));
  m_insert_into_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  m_insert_into_button->setPopupMode(QToolButton::InstantPopup);
  m_insert_into_button->setMenu(m_insert_into_menu.get());
  m_insert_into_button->setToolTip(
      "Append instruction as a child to the currently selected instruction");
  addWidget(m_insert_into_button);

  m_remove_button->setText("Into");
  m_remove_button->setIcon(QIcon(":/icons/beaker-remove-outline.svg"));
  m_remove_button->setToolButtonStyle(Qt::ToolButtonIconOnly);
  m_remove_button->setToolTip("Remove currently selected instruction together with its children");
  connect(m_remove_button, &QToolButton::clicked, this,
          &ComposerTreeToolBar::removeSelectedRequest);
  addWidget(m_remove_button);

  AddDotsMenu();
}

ComposerTreeToolBar::~ComposerTreeToolBar() = default;

void ComposerTreeToolBar::AddDotsMenu()
{
  InsertStrech();

  auto button = new QToolButton;
  button->setIcon(QIcon(":/icons/dots-horizontal.svg"));
  button->setIconSize(StyleUtils::ToolBarIconSize());
  addWidget(button);
}

void ComposerTreeToolBar::InsertStrech()
{
  auto empty = new QWidget(this);
  empty->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  addWidget(empty);
}

void ComposerTreeToolBar::SetupMenu()
{
  m_insert_after_menu = CreateInsertAfterMenu();
  m_insert_into_menu = CreateInsertIntoMenu();
}

//! Creates menu with possible instruction choice for selector button.

std::unique_ptr<QMenu> ComposerTreeToolBar::CreateInsertAfterMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = ModelView::Utils::GetStringList(sequi::DomainUtils::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit insertAfterRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

std::unique_ptr<QMenu> ComposerTreeToolBar::CreateInsertIntoMenu()
{
  auto result = std::make_unique<QMenu>();
  result->setToolTipsVisible(true);

  auto names = ModelView::Utils::GetStringList(sequi::DomainUtils::GetDomainInstructionNames());
  for (const auto &name : names)
  {
    auto action = result->addAction(name);
    auto on_action = [this, name]() { emit insertIntoRequest(name); };
    connect(action, &QAction::triggered, this, on_action);
  }

  return result;
}

}  // namespace sequi
