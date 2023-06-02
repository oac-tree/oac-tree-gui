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

#include "scene_utils.h"

#include <sequencergui/core/exceptions.h>
#include <sequencergui/domain/domain_utils.h>
#include <sequencergui/model/aggregate_factory.h>
#include <sequencergui/model/instruction_container_item.h>
#include <sequencergui/model/sequencer_model.h>
#include <sequencergui/model/sequencer_utils.h>
#include <sequencergui/model/standard_instruction_items.h>
#include <sequencergui/transform/transform_from_domain.h>

#include <mvvm/core/exceptions.h>
#include <mvvm/widgets/widget_utils.h>

#include <QDebug>
#include <QLinearGradient>
#include <QRectF>
#include <cctype>
#include <numeric>

namespace
{

sequencergui::InstructionItem* AddInstruction(std::unique_ptr<sequencergui::InstructionItem> item,
                                              sequencergui::InstructionContainerItem* container)
{
  auto model = container->GetModel();
  if (!model)
  {
    throw sequencergui::RuntimeException("Item is not the part of the model");
  }
  auto instruction_item = item.get();
  model->InsertItem(std::move(item), container, {});
  return instruction_item;
}

}  // namespace

namespace sequencergui
{

QSizeF GetGraphicsViewportSize()
{
  return {6000.0, 6000.0};
}

QPointF GetGraphicsViewportOrigin()
{
  return {-3000.0, 0.0};
}

QPointF GetGraphicsViewportCenter()
{
  // y-axis on graphics scene is pointing downwards. Our scene's corners looks like this:
  // (-3000, 0)     (3000, 0)
  // (-3000, 6000)  (3000, 6000)
  const auto origin = GetGraphicsViewportOrigin();
  const auto size = GetGraphicsViewportSize();
  return {origin.x() + size.width() / 2, origin.y() + size.height() / 2};
}

double GetAlignmentGridWidth()
{
  return sequencergui::ConnectableViewRectangle().width() * 1.4;
}

double GetAlignmentGridHeight()
{
  return sequencergui::ConnectableViewRectangle().height() * 1.4;
}

QGradient ConnectableViewGradient(const QColor& color, const QRectF& rect)
{
  QLinearGradient result(rect.x() + rect.width() / 2, rect.y(), rect.x() + rect.width() / 2,
                         rect.y() + rect.height());
  result.setColorAt(0, color);
  result.setColorAt(0.5, color.lighter(150));
  result.setColorAt(1, color);
  return result;
}

QRectF ConnectableViewRectangle()
{
  // make size of rectangle depending on 'M'-letter size to address scaling issues
  static QRectF result = QRectF(0, 0, mvvm::utils::UnitSize(8.2), mvvm::utils::UnitSize(10.0));
  return result;
}

std::vector<QPointF> GetPositions(const QPointF& reference, int n_points, double width)
{
  std::vector<QPointF> result;
  if (n_points < 0)
  {
    throw std::runtime_error("Error in GetPositions: invalid number of points");
  }

  double xpos = reference.x() - (n_points - 1) * width / 2;
  for (int i = 0; i < n_points; ++i)
  {
    result.emplace_back(QPointF(xpos, reference.y()));
    xpos += width;
  }
  return result;
}

QColor GetBaseColor(const InstructionItem* instruction)
{
  if (IsDecoratorInstruction(instruction))
  {
    return {"lightseagreen"};
  }

  if (IsCompoundInstruction(instruction))
  {
    return {"royalblue"};
  }

  return {Qt::lightGray};
}

InstructionItem* AddSingleInstruction(const std::string& domain_type,
                                      InstructionContainerItem* container)
{
  return AddInstruction(CreateInstructionItem(domain_type), container);
}

InstructionItem* AddAggregate(const std::string& aggregate_name,
                              InstructionContainerItem* container)
{
  static AggregateFactory factory;
  return AddInstruction(factory.Create(aggregate_name), container);
}

std::string InsertSpaceAtCamelCase(std::string str)
{
  if (str.empty())
  {
    return {};
  }

  auto pos = std::next(str.begin());
  while (pos != str.end())
  {
    // find next lower-case char followed by upper-case char
    if (std::islower(*(pos - 1)) && std::isupper(*pos))
    {
      pos = str.insert(pos, ' ');
    }
    ++pos;
  }

  return str;
}

}  // namespace sequencergui
